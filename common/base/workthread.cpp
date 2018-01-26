#include <sys/eventfd.h>
#include <unistd.h>

#include <event2/event.h>

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

#include "base/task_runner.h"
#include "base/workthread.h"

static task_runner runner;

static std::thread thread_id;
static std::mutex mutex;
static std::condition_variable cond;

static int event = -1;
static int running = 0;
static struct eventloop local_loop;

static void event_read(evutil_socket_t fd, short event, void *arg);
static void threadentry(void);

void workthread_start(struct eventloop loop)
{
    running = 1;
    local_loop = loop;
    thread_id = std::thread(threadentry);
}

void workthread_post(const Closure& task)
{
    uint64_t count = 1;
    int ret = 0;

    if (-1 == event) {
        return;
    }

    runner.postTask(task);

    do {
        ret = write(event, &count, 8);
    } while ((ret < 0) && (EAGAIN == errno));
}

void workthread_stop(void)
{
    uint64_t count = 1;
    int ret = 0;

    if (-1 == event) {
        return;
    }

    runner.postStopTask();

    do {
        ret = write(event, &count, 8);
    } while ((ret < 0) && (EAGAIN == errno));
}

void workthread_wait(void)
{
    std::unique_lock<std::mutex> lck(mutex);
    while (running) {
        cond.wait(lck);
    }
}

static void
event_read(evutil_socket_t fd, short event, void *arg)
{
    uint64_t count = 0;
    int ret = 0;

    do {
        ret = read(fd, &count, 8);
    } while ((ret < 0) && (EAGAIN == errno));

    if (-1 != ret) {
        runner.runTask();
        if (!runner.isRunning()) {
            struct event_base *base = (struct event_base *)arg;
            struct timeval delay = { 2, 0 };
            event_base_loopexit(base, &delay);
        }
    }
}

static void threadentry(void)
{
    struct event_base *base;

    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return;
    }

    event = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);

    if (-1 == event) {
        event_base_free(base);
        return;
    }

    struct event *ev = event_new(base, event, EV_READ|EV_PERSIST, event_read, base);

    if (ev) {
        event_add(ev, NULL);
    }

    if (local_loop.loop_setup) {
        local_loop.loop_setup(base);
    }

    event_base_dispatch(base);

    event_base_free(base);

    event_del(ev);

    close(event);

    if (local_loop.loop_free) {
        local_loop.loop_free();
    }

    std::unique_lock<std::mutex> lck(mutex);
    running = 0;
    cond.notify_one();
}
