 #include <stdint.h>
 #include <string>

#ifndef CLIENTINFO_NPP
#define CLIENTINFO_NPP

struct ClientInfo
{
    bool                is_used;
    int                 client_fd;
    std::string         src_ip;
    uint16_t            src_port;
    uint16_t            my_index;
};


#endif