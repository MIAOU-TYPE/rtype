/*
** EPITECH PROJECT, 2025
** serverCPP
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

    Server::UDPServer udpServer;
    try {
        udpServer.configure(ip, port);
        udpServer.start();
        udpServer.stop();
    } catch (const Server::ServerError &e) {
        std::cerr << "{Main}" << e.what() << std::endl;
        return 1;
    }
    return 0;
}
