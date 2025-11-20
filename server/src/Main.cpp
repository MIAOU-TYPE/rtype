/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Main
*/

#include <iostream>
#include <string>
#include "UDPServer.hpp"

int main(void)
{
    std::string ip = "127.0.0.1";
    uint16_t port = 8080;

    std::shared_ptr<Server::IServer> server = std::make_shared<Server::UDPServer>();
    try {
        server->configure(ip, port);
        server->start();
        while (true)
            server->readPackets();
        server->stop();
    } catch (const Server::ServerError &e) {
        std::cerr << "{Main}" << e.what() << std::endl;
        return 1;
    }
    return 0;
}
