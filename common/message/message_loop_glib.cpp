#include "message_loop_glib.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cerrno>

/**
 * @struct MessageLoopSource
 */
struct MessageLoopSource
{
    GSource source;         // GSource
    task_runner *runner;    // task runner
    GPollFD event;          // GPollFD
    bool have_work;         // working?
};

static gboolean prepare(GSource *source, gint *timeout_);

static gboolean check(GSource *source);

static gboolean dispatch(GSource *source,
                         GSourceFunc callback, gpointer user_data);

static void finalize(GSource *source);

GSourceFuncs source_funcs = {
    .prepare = prepare,
    .check = check,
    .dispatch = dispatch,
    .finalize = finalize,
};

message_loop_glib::message_loop_glib():
    m_source(0)
{
    m_event = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
}

message_loop_glib::~message_loop_glib()
{
    if (m_source) {
        g_source_remove(m_source);
    }
    if (-1 != m_event) {
        close(m_event);
        m_event = -1;
    }
}

bool message_loop_glib::attach()
{
    if (-1 != m_event) {
        MessageLoopSource *source =
                reinterpret_cast<MessageLoopSource *>(g_source_new(&source_funcs,
                                                  sizeof(*source)));
        source->runner = &m_runner;
        source->event.fd = m_event;
        source->event.events = G_IO_IN;
        source->have_work = false;
        g_source_add_poll((GSource *)source, &source->event);
        m_source = g_source_attach((GSource *)source, g_main_context_get_thread_default());
        g_source_unref((GSource *)source);
        return true;
    }
    return false;
}

// TODO: use stop task to detach.

bool message_loop_glib::detach()
{
    guint64 count = 1;
    if (m_source) {
        m_runner.postStopTask();
        if (-1 != write(m_event, &count, 8)) {
            m_event = -1;
            return true;
        }
        else if (EAGAIN == errno) {
            // the message queue is full, just
            // just discard this post now, post
            // will become valid after some task processed.
        }
        m_source = 0;
    }
    return false;
}

bool message_loop_glib::postTask(const Closure& task)
{
    guint64 count = 1;
    if (m_source) {
        // notify glib mainloop that there is one task
        // to be called.
        if (-1 != m_event) {
            m_runner.postTask(task);
            if (-1 != write(m_event, &count, 8)) {
                return true;
            }
            else if (EAGAIN == errno) {
                // the message queue is full, just
                // just discard this post now, post
                // will become valid after some task processed.
            }
        }
    }
    return false;
}

static gboolean prepare(GSource *source, gint *timeout_)
{
    MessageLoopSource *loop = reinterpret_cast<MessageLoopSource *>(source);
    if (timeout_) {
        *timeout_ = loop->have_work ? 0:-1;
    }
    return FALSE;
}

static gboolean check(GSource *source)
{
    guint64 count = 0;
    MessageLoopSource *loop = reinterpret_cast<MessageLoopSource *>(source);
    if (G_IO_IN & loop->event.revents) {
        if (-1 != read(loop->event.fd, &count, 8)) {
            loop->have_work = true;
        }
        else if (EAGAIN == errno) {
            // poll success on G_IO_IN implies read on evenfd counter
            // is greater than 0, so this case will not reach.
        }
    }
    return loop->have_work;
}

// TODO: remove source when receive stop task.
static gboolean dispatch(GSource *source,
                  GSourceFunc callback, gpointer user_data)
{
    MessageLoopSource *loop = reinterpret_cast<MessageLoopSource *>(source);
    (void)callback;
    (void)user_data;
    loop->have_work = false;
    loop->runner->runTask();
    return loop->runner->isRunning();
}

static void finalize(GSource *source)
{
    (void)source;
    // do nothing.
    // 1. eventfd descriptor needn't close,
    //    because glib do that for me.
    // 2. source memory is managed by glib(reference count policy).
}

/* EOF */
