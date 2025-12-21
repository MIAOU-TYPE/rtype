/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Main
*/
#include <iostream>
#include "ArgParser.hpp"
#include "ClientRuntime.hpp"
#include "EventBus.hpp"
#include "EventRegistry.hpp"
#include "SfmlGraphics.hpp"
#include "UDPClient.hpp"

namespace
{
    Utils::ArgParseResult parseArgs(Utils::ArgParser &argParser)
    {
        try {
            const auto status = argParser.parse();
            return status;
        } catch (std::exception &e) {
            std::cerr << "{Main}: " << e.what() << std::endl;
            return Utils::ArgParseResult::Error;
        }
    }

    void createEventHandler(Core::EventRegistry &eventRegistry, Thread::ClientRuntime &clientRuntime,
        const std::shared_ptr<Core::EventBus> &eventBus)
    {
        eventRegistry.onKeyPressed(Core::Key::Escape, [&]() {
            clientRuntime.stop();
        });

        eventBus->on<Core::WindowClosed>([&](const Core::WindowClosed &) {
            clientRuntime.stop();
        });
    }
} // namespace

int main(int argc, char **argv)
{
    Utils::ArgParser argParser(argc, argv);

    if (const auto result = parseArgs(argParser); result != Utils::ArgParseResult::Success)
        return result == Utils::ArgParseResult::HelpDisplayed ? 0 : 84;

    const auto graphics = std::make_shared<Graphics::SfmlGraphics>();
    const auto client = std::make_shared<Network::UDPClient>();

    Thread::ClientRuntime clientRuntime(graphics, client);
    const auto eventBus = clientRuntime.getEventBus();
    Core::EventRegistry eventRegistry(eventBus);

    createEventHandler(eventRegistry, clientRuntime, eventBus);
    std::cout << "Connecting to " << argParser.getHost() << ":" << argParser.getPort() << "..." << std::endl;
    client->configure(argParser.getHost(), argParser.getPort());

    clientRuntime.start();
    clientRuntime.runDisplay();
    clientRuntime.wait();

    return 0;
}