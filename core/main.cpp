#include "netcore.h"
#include "netlist.h"

#include <signal.h>

#include <string>
#include <fstream>
#include <streambuf>

static netlist net;
static std::map< uint32_t, std::vector<uint32_t> > result;

static void signal_cb(evutil_socket_t, short, void *);

int main(int argc, char *argv[])
{
    struct event_base *base;
    struct event *signal_event;
    unsigned short port = 0;

    if (argc < 2) {
        return -1;
    }

    std::ifstream t(argv[1]);
    std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

    if (3 == argc) {
        sscanf(argv[2], "%hu", &port);
    }

    if (-1 == net.parse(str.c_str())) {
        return -1;
    }

    if (-1 == net.partition(conn_count(), result)) {
        return -1;
    }

    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

    if (!signal_event || event_add(signal_event, NULL)<0) {
        fprintf(stderr, "Could not create/add a signal event!\n");
        return 1;
    }

    netcore_start(base, port);

    event_base_dispatch(base);

    event_free(signal_event);

    event_base_free(base);

    return 0;
}

static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    struct event_base *base = (struct event_base *)user_data;
    struct timeval delay = { 2, 0 };

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    event_base_loopexit(base, &delay);

    netcore_free();
}