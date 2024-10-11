#pragma once
#ifndef CLIENTCONFIG_HPP
# define CLIENTCONFIG_HPP

#include <Port.hpp>

struct ClientConfig
{
    std::string ip;
    Port        port;
};

#endif