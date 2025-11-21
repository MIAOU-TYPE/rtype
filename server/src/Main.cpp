/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Main
*/

#include <iostream>
#include <string>
#include "PosixHandler.hpp"
#include "UDPServer.hpp"

int main(void)
{
    std::string ip = "127.0.0.1";
    uint16_t port = 8080;

    std::shared_ptr<Server::IServer> server = std::make_shared<Server::UDPServer>();
    Signal::PosixHandler signalHandler;
    signalHandler.start();
    signalHandler.registerCallback(Signal::SignalType::Interrupt, [&server]() {
        if (server->isRunning())
            server->setRunning(false);
    });
    try {
        server->configure(ip, port);
        server->start();
        while (server->isRunning())
            server->readPackets();
        server->stop();
    } catch (const Server::ServerError &e) {
        std::cerr << "{Main}" << e.what() << std::endl;
        return 1;
    }
    return 0;
}
