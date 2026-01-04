/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Main
*/

#include <string>
#include "ArgParser.hpp"
#include "ServerRuntime.hpp"
#include "SignalHandler.hpp"
#include "TCPServer.hpp"
#include "UDPServer.hpp"

namespace
{
    std::shared_ptr<Signal::SignalHandler> startSignalHandler(Net::Thread::ServerRuntime &runtime)
    {
        auto signalHandler = std::make_shared<Signal::SignalHandler>();

        signalHandler->start();
        signalHandler->registerCallback(Signal::SignalType::Interrupt, [&runtime]() {
            runtime.stop();
        });
        return signalHandler;
    }
} // namespace

int main(const int argc, char **argv)
{
    Utils::ArgParser parser(argc, argv);
    if (const Utils::ArgParseResult result = parser.parse(); result == Utils::ArgParseResult::HelpDisplayed) {
        return 0;
    } else if (result == Utils::ArgParseResult::Error) {
        std::cerr << "{main}: Error parsing arguments. Use --help for usage information." << std::endl;
        return 84;
    }
    const int port = parser.getPort();
    auto host = parser.getHost();
    try {
        const auto udpServer = std::make_shared<Net::Server::UDPServer>();
        const auto tcpServer = std::make_shared<Net::Server::TCPServer>();

        Net::Thread::ServerRuntime runtime(udpServer, tcpServer);
        const auto signalHandler = startSignalHandler(runtime);

        tcpServer->configure(host, port);
        udpServer->configure(host, port + 1);
        runtime.start();
        runtime.wait();
        signalHandler->stop();
    } catch (const std::exception &e) {
        std::cerr << "{main}: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
