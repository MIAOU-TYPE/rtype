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

static std::shared_ptr<Signal::SignalHandler> startSignalHandler(bool &run)
{
    std::shared_ptr<Signal::SignalHandler> signalHandler = std::make_shared<Signal::SignalHandler>();

    signalHandler->start();
    signalHandler->registerCallback(Signal::SignalType::Interrupt, [&run]() {
        run = false;
    });
    return signalHandler;
}

int main(void)
{
    std::string ip = "127.0.0.1";
    uint16_t port = 8080;

    std::shared_ptr<Server::IServer> server = std::make_shared<Server::UDPServer>();
    bool run = false;
    std::shared_ptr<Signal::SignalHandler> signalHandler = startSignalHandler(run);
    try {
        server->configure(ip, port);
        server->start();
        run = server->isRunning();
        std::thread reader ([&]() {
            while (run) {
                server->readPackets();
            }
        });
        while (run) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        server->setRunning(false);
        reader.join();
        server->stop();
    } catch (const Server::ServerError &e) {
        std::cerr << "{Main}" << e.what() << std::endl;
        return 1;
    }
    return 0;
}
