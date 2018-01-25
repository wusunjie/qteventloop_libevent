#ifndef _WORKTHREAD_H
#define _WORKTHREAD_H

#include "callback.h"

typedef void (*workthread_setup)(struct event_base *base);

typedef void (*workthread_free)(void);

struct eventloop {
	workthread_setup loop_setup;
	workthread_free loop_free;
};

extern void workthread_start(struct eventloop loop);

extern void workthread_post(const Closure& task);

extern void workthread_stop(void);

extern void workthread_wait(void);

#endif