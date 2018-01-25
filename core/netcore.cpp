#include <string.h>
#include <errno.h>
#include <stdio.h>
#ifndef _WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>

#include <list>
#include <algorithm>

static void listener_cb(struct evconnlistener *, evutil_socket_t,
    struct sockaddr *, int socklen, void *);
static void conn_readcb(struct bufferevent *, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);

struct conn {
    int fd;
    struct sockaddr addr;
    struct bufferevent *bev;
};

static std::list<struct conn> conn_list;

static struct evconnlistener *listener;

int
netcore_start(struct event_base *base, unsigned short port)
{
    struct sockaddr_in sin;
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));

    if (!listener) {
        fprintf(stderr, "Could not create a listener!\n");
        return -1;
    }

    return 0;
}

int
netcore_free(void)
{
    std::for_each(conn_list.begin(), conn_list.end(),
        [](const struct conn &i)
        {
            bufferevent_free(i.bev);
        });
    conn_list.clear();
    evconnlistener_free(listener);
}

unsigned int
conn_count(void)
{
    return conn_list.size();
}

static void
listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    struct event_base *base = (struct event_base *)user_data;
    struct bufferevent *bev;
    struct conn con;

    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(base);
        return;
    }
    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_WRITE);
    bufferevent_enable(bev, EV_READ);

    con.fd = fd;
    con.addr = *sa;
    con.bev = bev;

    conn_list.push_back(con);
}

static void
conn_readcb(struct bufferevent *bev, void *user_data)
{
    struct evbuffer *output = bufferevent_get_output(bev);
    printf("read count %lu\n", evbuffer_get_length(output));
}

static void
conn_writecb(struct bufferevent *bev, void *user_data)
{
    struct evbuffer *output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0) {
        printf("flushed answer\n");
    }
}

static void
conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF) {
        printf("Connection closed.\n");
    } else if (events & BEV_EVENT_ERROR) {
        printf("Got an error on the connection: %s\n",
            strerror(errno));/*XXX win32*/
    }
    /* None of the other events can happen here, since we haven't enabled
     * timeouts */
    bufferevent_free(bev);
}
