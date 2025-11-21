/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Main
*/

#include <iostream>
#include <string>
#include "SignalHandler.hpp"
#include "UDPServer.hpp"

int main(void)
{
    std::string ip = "127.0.0.1";
    uint16_t port = 8080;

    std::shared_ptr<Server::IServer> server = std::make_shared<Server::UDPServer>();
    Signal::SignalHandler signalHandler;
    signalHandler.start();
    signalHandler.registerCallback(Signal::SignalType::Interrupt, [&server]() {
        if (server->isRunning())
            server->setRunning(false);
        std::cout << "\n{Main} Interrupt signal received. Shutting down the server..." << std::endl;
    });
    try {
        server->configure(ip, port);
        server->start();
        while (server->isRunning())
            server->readPackets();
        server->stop();
    } catch (const Server::ServerError &e) {
        std::cerr << "{Main}" << e.what() << std::endl;
        signalHandler.stop();
        return 1;
    }
    signalHandler.stop();
    return 0;
}
