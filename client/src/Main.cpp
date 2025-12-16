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
#include "GameScene.hpp"
#include "NetClient.hpp"
#include "SFMLRenderer.hpp"
#include "SFMLTextureManager.hpp"
#include "SignalHandler.hpp"

static std::shared_ptr<Signal::SignalHandler> startSignalHandler(Thread::ClientRuntime &runtime)
{
    std::shared_ptr<Signal::SignalHandler> signalHandler = std::make_shared<Signal::SignalHandler>();

    signalHandler->start();
    signalHandler->registerCallback(Signal::SignalType::Interrupt, [&runtime]() {
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
        auto renderer = std::make_shared<Graphics::SFMLRenderer>();
        auto textureManager = std::make_shared<Graphics::SFMLTextureManager>();
        const auto client = std::make_shared<Network::NetClient>();
        const auto scene = std::make_shared<Game::GameScene>(renderer, textureManager);

        Thread::ClientRuntime runtime(client, scene);
        const auto signalHandler = startSignalHandler(runtime);

        client->configure(host, port);
        runtime.start();
        runtime.wait();
        signalHandler->stop();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "{main}: " << e.what() << std::endl;
        return 1;
    }
}