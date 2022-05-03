#ifndef UTIL_H
#define UTIL_H
#include "util.hpp"
#include <iostream>

void net_run(std::function<sf::Socket::Status(void)> f, std::string error_message)
{
    sf::Socket::Status status = f();
    if (status != sf::Socket::Done)
    {
        std::cerr << "Failed to " << error_message << ": " << status << std::endl;
        perror(error_message.c_str());
        exit(0);
    }
}
#endif
