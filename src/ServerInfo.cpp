#include <ServerInfo.hpp>

using namespace WebServerExceptions;

ServerInfo::ServerInfo(
    int _port, 
    int _clientMaxBodySize, 
    std::string _index,
    std::string _name, 
    std::string _root, 
    std::string _host, 
    std::string _defaultErrorPage
) : 
    port(_port), 
    clientMaxBodySize(_clientMaxBodySize), 
    index(_index),
    name(_name), 
    root(_root), 
    host(_host), 
    defaultErrorPage(_defaultErrorPage) {}


void ServerInfo::InitInfo() {}

ServerInfo::~ServerInfo() {}
