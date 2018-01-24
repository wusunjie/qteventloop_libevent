#include "netcore.h"
#include "netlist.h"

#include <string>
#include <fstream>
#include <streambuf>

static netlist net;
static std::map< uint32_t, std::vector<uint32_t> > result;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        return -1;
    }

    std::ifstream t(argv[1]);
    std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

    if (-1 == net.parse(str.c_str())) {
        return -1;
    }

    return mainloop(9995);
}
