/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** EventInit
*/

#include "EventInit.hpp"

using namespace Events;

EventInit::EventInit(std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Network::INetClient> client,
    std::shared_ptr<Network::ClientPacketFactory> packetFactory)
    : _renderer(std::move(renderer)), _client(std::move(client)), _packetFactory(std::move(packetFactory))
{
    if (!_renderer)
        throw EventInitError("Renderer pointer is null");
    if (!_client)
        throw EventInitError("INetClient pointer is null");
    if (!_packetFactory)
        throw EventInitError("ClientPacketFactory pointer is null");
}

void EventInit::run()
{
    if (!_renderer) {
        throw EventInitError("Renderer not properly initialized");
    }

    try {
        std::shared_ptr<Graphics::IEvent> event;

        while (_renderer->pollEvent(event)) {
            if (!event)
                continue;
            auto sfmlEventPtr = std::dynamic_pointer_cast<Graphics::SFMLEvent>(event);
            if (sfmlEventPtr && event->isType(Graphics::EventType::KeyPressed)) {
                const auto &sfEvent = sfmlEventPtr->getSFMLEvent();
                if (auto kp = sfEvent.getIf<sf::Event::KeyPressed>()) {
                    PlayerInput input;
                    if (kp->code == sf::Keyboard::Key::Space) {
                        input.shoot = true;
                    }
                    if (kp->code == sf::Keyboard::Key::Up) {
                        input.up = true;
                    }
                    if (kp->code == sf::Keyboard::Key::Left) {
                        input.left = true;
                    }
                    if (kp->code == sf::Keyboard::Key::Right) {
                        input.right = true;
                    }
                    if (kp->code == sf::Keyboard::Key::Down) {
                        input.down = true;
                    }
                    auto packet = _packetFactory->makePlayerInput(input);
                    if (packet)
                        _client->sendPacket(*packet);
                }
            }

            if (_renderer->isWindowCloseEvent(*event)) {
                _renderer->close();
                break;
            }
        }
    } catch (const std::exception &e) {
        throw EventInitError("Unexpected error during event processing: " + std::string(e.what()));
    }
}

bool EventInit::isWindowOpen() const
{
    return _renderer && _renderer->isOpen();
}
