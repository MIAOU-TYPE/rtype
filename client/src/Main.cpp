/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** Main
*/

#include <exception>
#include <iostream>
#include "ClientRuntime.hpp"
#include "NetClient.hpp"

int main()
{
    try {
        auto server = std::make_shared<Network::NetClient>();
        Thread::ClientRuntime runtime(server);

        server->configure("127.0.0.1", 8080);
        runtime.start();
        runtime.wait();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "R-Type Client System Error: " << e.what() << std::endl;
        return 1;
    }
}