/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** Main
*/

#include <exception>
#include <iostream>
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
        std::cout << "\nInterrupt signal received. Stopping client..." << std::endl;
        runtime.stop();
    });
    return signalHandler;
}

int main()
{
    try {
        auto renderer = std::make_shared<Graphics::SFMLRenderer>();
        auto textureManager = std::make_shared<Graphics::SFMLTextureManager>();
        const auto client = std::make_shared<Network::NetClient>();
        const auto scene = std::make_shared<Game::GameScene>(renderer, textureManager);

        Thread::ClientRuntime runtime(client, scene);
        const auto signalHandler = startSignalHandler(runtime);

        client->configure("127.0.0.1", 8080);
        runtime.start();
        runtime.wait();
        signalHandler->stop();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "R-Type Client System Error: " << e.what() << std::endl;
        return 1;
    }
}