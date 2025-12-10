/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetworkSystem
*/

#include "NetworkSendSystem.hpp"

namespace Ecs
{

    NetworkSendSystem::NetworkSendSystem(std::shared_ptr<Registry> registry, std::shared_ptr<Game::NetClient> netClient)
        : _registry(registry), _netClient(netClient)
    {
    }

    void NetworkSendSystem::update(float deltaTime)
    {
        _registry->view<InputComponent>([this](Entity entity, const InputComponent &input) {
            float dx = 0.0f;
            float dy = 0.0f;

            if (input.left)
                dx -= 1.0f;
            if (input.right)
                dx += 1.0f;
            if (input.up)
                dy -= 1.0f;
            if (input.down)
                dy += 1.0f;

            _netClient->sendInputPacket(dx, dy, input.shoot);
        });
    }

} // namespace Ecs