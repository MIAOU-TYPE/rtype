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


static std::shared_ptr<Signal::SignalHandler> startSignalHandler(std::shared_ptr<Server::IServer> server)
{
    std::shared_ptr<Signal::SignalHandler> signalHandler = std::make_shared<Signal::SignalHandler>();

    signalHandler->start();
    signalHandler->registerCallback(Signal::SignalType::Interrupt, [&server]() {
        if (server->isRunning())
            server->setRunning(false);
    });
    return signalHandler;
}

int main(void)
{
    std::string ip = "127.0.0.1";
    uint16_t port = 8080;

    std::shared_ptr<Server::IServer> server = std::make_shared<Server::UDPServer>();
    auto signalHandler =  startSignalHandler(server);
    try {
        server->configure(ip, port);
        server->start();
        while (server->isRunning())
            server->readPackets();
        server->stop();
    } catch (const Server::ServerError &e) {
        std::cerr << "{Main}" << e.what() << std::endl;
        signalHandler->stop();
        return 1;
    }
    signalHandler->stop();
    return 0;
}
