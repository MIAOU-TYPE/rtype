/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Main
*/

#include <string>
#include "ServerRuntime.hpp"
#include "SignalHandler.hpp"
#include "UDPServer.hpp"

static std::shared_ptr<Signal::SignalHandler> startSignalHandler(ServerRuntime &runtime)
{
    std::shared_ptr<Signal::SignalHandler> signalHandler = std::make_shared<Signal::SignalHandler>();

    signalHandler->start();
    signalHandler->registerCallback(Signal::SignalType::Interrupt, [&runtime]() {
        runtime.stop();
    });
    return signalHandler;
}

int main(void)
{
    std::shared_ptr<Server::IServer> server = std::make_shared<Server::UDPServer>();
    ServerRuntime runtime(server);
    std::shared_ptr<Signal::SignalHandler> signalHandler = startSignalHandler(runtime);

    try {
        server->configure("127.0.0.1", 8080);
        runtime.start();
        runtime.wait();
        signalHandler->stop();
    } catch (const std::exception &e) {
        std::cerr << "{main}: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
