#include "netcore.h"
#include "netlist.h"

#include <string>
#include <fstream>
#include <streambuf>

static netlist net;
static std::map< uint32_t, std::vector<uint32_t> > result;

int main(int argc, char *argv[])
{
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

    return mainloop(port);
}
