/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** Main
*/

#include <exception>
#include <iostream>
#include "ArgParser.hpp"
#include "ClientRuntime.hpp"
#include "NetClient.hpp"
#include "SignalHandler.hpp"

static std::shared_ptr<Signal::SignalHandler> startSignalHandler(Thread::ClientRuntime &runtime)
{
    std::shared_ptr<Signal::SignalHandler> signalHandler = std::make_shared<Signal::SignalHandler>();

    signalHandler->start();
    signalHandler->registerCallback(Signal::SignalType::Interrupt, [&runtime]() {
        std::cout << "\nInterrupt signal received. Stopping client..." << std::endl;
        runtime.stop();
    });
    return signalHandler;
}

int main(int argc, char **argv)
{
    Utils::ArgParser parser(argc, argv);
    if (const Utils::ArgParseResult result = parser.parse(); result == Utils::ArgParseResult::HelpDisplayed) {
        return 0;
    } else if (result == Utils::ArgParseResult::Error) {
        std::cerr << "{main}: Error parsing arguments. Use --help for usage information." << std::endl;
        return 84;
    }
    const int port = parser.getPort();
    const std::string host = parser.getHost();
    try {
        auto client = std::make_shared<Network::NetClient>();
        Thread::ClientRuntime runtime(client);
        const auto signalHandler = startSignalHandler(runtime);

        client->configure(host, port);
        runtime.start();
        runtime.wait();
        signalHandler->stop();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "R-Type Client System Error: " << e.what() << std::endl;
        return 1;
    }
}