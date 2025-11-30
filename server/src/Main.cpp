/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Main
*/

#include <iostream>
#include <string>
#include <thread>
#include "SignalHandler.hpp"
#include "UDPServer.hpp"
#include "UDPPacket.hpp"

static std::shared_ptr<Signal::SignalHandler> startSignalHandler(const std::shared_ptr<Server::IServer> &server)
{
    std::shared_ptr<Signal::SignalHandler> signalHandler = std::make_shared<Signal::SignalHandler>();

    signalHandler->start();
    signalHandler->registerCallback(Signal::SignalType::Interrupt, [server]() {
        if (server && server->isRunning())
            server->setRunning(false);
    });
    return signalHandler;
}

int main(void)
{
    std::string ip = "127.0.0.1";
    uint16_t port = 8080;

    std::shared_ptr<Server::IServer> server = std::make_shared<Server::UDPServer>();
    std::shared_ptr<Signal::SignalHandler> signalHandler = startSignalHandler(server);
    try {
        server->configure(ip, port);
        server->start();
        std::thread reader([&]() {
            while (server->isRunning()) {
                server->readPackets();
            }
        });
        while (server->isRunning()) {
            std::shared_ptr<Net::IServerPacket> packet;
            if (server->popPacket(packet)) {
                std::cout << "{Main} Received packet: " << *packet;
            }
        }
        server->setRunning(false);
        reader.join();
    } catch (const Server::ServerError &e) {
        std::cerr << "{Main}" << e.what() << std::endl;
        signalHandler->stop();
        server->stop();
        return 1;
    }
    signalHandler->stop();
    server->stop();
    return 0;
}
