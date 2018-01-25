#include <sys/eventfd.h>
#include <pthread.h>
#include <unistd.h>

#include <event2/event.h>

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "base/task_runner.h"

typedef void (*workthread_setup)(struct event_base *base);

static task_runner runner;
static pthread_t thread_id;

static void event_read(evutil_socket_t fd, short event, void *arg);
static void *threadentry(void *argv);

void workthread_start(workthread_setup setup)
{
    pthread_create(&thread_id, NULL, threadentry, (void *)setup);
}

static void
event_read(evutil_socket_t fd, short event, void *arg)
{
    uint64_t count = 0;
    int ret = 0;

    do {
        ret = read(fd, &count, 8);
    } while ((ret < 0) && (EAGAIN == errno));

    if (ret != read(fd, &count, 8)) {
        while (runner.isRunning()) {
            runner.runTask();
        }
    }
}

static void *threadentry(void *argv)
{
    struct event_base *base;
    int event;
    workthread_setup setup = (workthread_setup)argv;

    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return NULL;
    }

    event = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);

    if (-1 == event) {
        event_base_free(base);
        return NULL;
    }

    struct event *ev = event_new(base, event, EV_READ|EV_PERSIST, event_read, event_self_cbarg());

    if (ev) {
        event_add(ev, NULL);
    }

    if (setup) {
        setup(base);
    }

    event_base_dispatch(base);

    event_del(ev);

    event_base_free(base);

    return NULL;
}
