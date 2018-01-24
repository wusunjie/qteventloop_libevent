#include <string.h>
#include <stdlib.h>

#include <string>
#include <fstream>
#include <streambuf>
#include <utility>
#include <algorithm>
#include <iostream>

#include "netlist.h"


netlist::netlist(void)
{
    memset(&infos, 0, sizeof(infos));
}

int netlist::parse(const char *data)
{
    size_t len = 0;
    char *buffer = NULL;
    char *pch = NULL;

    if (data) {
        len = strlen(data);
        if (len > 0) {
            buffer = (char *)malloc(len + 1);
            if (buffer) {
                strncpy(buffer, data, len);
                buffer[len] = '\0';

                pch = strtok(buffer, "\n");
                while (NULL != pch) {
                    char item_name[50] = {0};
                    uint32_t val = 0;
                    int ret = sscanf(pch, "%s%*[ \t\n\v\f\r]%u", item_name, &val);
                    if (ret > 0) {
                        if (strstr(item_name, "First_End")) {
                            infos.first_end = val;
                        }
                        else if (strstr(item_name, "Last_End")) {
                            infos.last_end = val;
                        }
                        else if (strstr(item_name, "Isolation")) {
                            infos.isolation = val;
                        }
                        else if (strstr(item_name, "Voltage")) {
                            infos.voltage = val;
                        }
                        else if (strstr(item_name, "Continuity")) {
                            infos.connectivity = val;
                        }
                        else if (strstr(item_name, "Unit")) {
                            infos.unit = val;
                        }
                        else if (strstr(item_name, "Rubber_Step")) {
                            infos.rubber_step = val;
                        }
                        else if (strstr(item_name, "Begin")) {
                            pch = strtok(NULL, "\n");
                            break;
                        }
                    }
                    pch = strtok(NULL, "\n");
                }

                if (pch) {
                    while (!strstr(pch, "End")) {
                        if ('*' == pch[0]) {
                            list.push_back(std::vector<uint32_t>());
                        }
                        if (!list.empty()) {
                            uint32_t val = 0, status = 0;
                            for (size_t i = 0;i < strlen(pch); i++) {
                                if (!status) {
                                    if (!isspace(pch[i])) {
                                        if (1 == sscanf(pch + i, "%d", &val)) {
                                            list.back().push_back(val);
                                            status = 1;
                                        }
                                    }
                                }
                                else if (isspace(pch[i])) {
                                    status = 0;
                                }
                            }
                        }

                        if (NULL == (pch = strtok(NULL, "\n"))) {
                            break;
                        }
                    }
                }

                free(buffer);
            }
        }
    }

    return list.empty() ? -1:0;
}

int netlist::partition(uint32_t buckets, std::map<uint32_t, std::vector<uint32_t> > &result)
{
    typedef std::pair<std::vector< std::vector<uint32_t> >::size_type, uint32_t> optimize_type;

    std::vector< optimize_type > weights;

    std::for_each(list.begin(), list.end(),
        [&](const std::vector<uint32_t> &i)
        {
            static std::vector< std::vector<uint32_t> >::size_type s = 0;
            if (i.size() > 1) {
                weights.push_back(std::make_pair(s, i.size() - 1));
            }
            else {
                weights.push_back(std::make_pair(s, 1));
            }
            s++;
        });

    std::sort(weights.begin(), weights.end(),
        [](const optimize_type &i,
            const optimize_type &j)
        {
            return i.second > j.second;
        });

    uint32_t *optimize_opt = (uint32_t *)calloc(buckets, sizeof(*optimize_opt));

    if (optimize_opt) {
        std::for_each(weights.begin(), weights.end(),
            [&](const optimize_type &i)
            {
                uint32_t min = 0;

                for (uint32_t cur = 0; cur < buckets; cur++) {
                    if (optimize_opt[min] > optimize_opt[cur]) {
                        min = cur;
                    }
                }

                result[min].push_back(i.first);
                optimize_opt[min] += i.second;
            });
        free(optimize_opt);
        return 0;
    }

    return -1;
}
