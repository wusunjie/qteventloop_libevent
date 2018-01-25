#include <sys/eventfd.h>
#include <pthread.h>
#include <unistd.h>

#include <event2/event.h>

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "base/task_runner.h"
#include "base/workthread.h"

static task_runner runner;
static pthread_t thread_id;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int event = -1;
static int running = 0;
static struct eventloop local_loop;

static void event_read(evutil_socket_t fd, short event, void *arg);
static void *threadentry(void *argv);

void workthread_start(struct eventloop loop)
{
    running = 1;
    local_loop = loop;
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&thread_id, NULL, threadentry, NULL);
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
    pthread_mutex_lock(&mutex);
    while (running) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
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

static void *threadentry(void *argv)
{
    struct event_base *base;

    pthread_detach(pthread_self());

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

    pthread_mutex_lock(&mutex);
    running = 0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}
