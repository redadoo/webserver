#ifndef CLIENTCONFIG_HPP
# define CLIENTCONFIG_HPP

#include <stdint.h>
#include <string>
#include <Port.hpp>

struct ClientConfig
{
    std::string ip;
    Port        port;
};

#endif