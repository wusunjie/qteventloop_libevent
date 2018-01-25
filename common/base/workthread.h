#ifndef _WORKTHREAD_H
#define _WORKTHREAD_H

typedef void (*workthread_setup)(struct event_base *base);

extern void workthread_start(workthread_setup setup);

#endif