#ifndef _NETLIST_CHECK_NETLIST_H
#define _NETLIST_CHECK_NETLIST_H

#include <stdint.h>

#include <vector>
#include <map>

class netlist {
private:

    struct header_info {
        char part_number[50];
        uint32_t first_end;
        uint32_t last_end;
        uint32_t isolation;
        uint32_t voltage;
        uint32_t connectivity;
        uint32_t type;
        uint32_t unit;
        uint32_t rubber_step;
    };

    struct header_info infos;
    std::vector< std::vector<uint32_t> > list;

public:

    netlist(void);

    int parse(const char *data);

    int partition(uint32_t buckets, std::map< uint32_t, std::vector<uint32_t> > &result);
};

#endif
