#ifndef _NETCORE_H
#define _NETCORE_H

#include <event2/event.h>

extern int netcore_start(struct event_base *base, unsigned short port);

extern int netcore_free(void);

extern unsigned int conn_count(void);

#endif