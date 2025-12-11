/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetworkSendSystem
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
            int8_t dx = 0;
            int8_t dy = 0;

            if (input.left)
                dx -= 1;
            if (input.right)
                dx += 1;
            if (input.up)
                dy -= 1;
            if (input.down)
                dy += 1;

            _netClient->sendInputPacket(dx, dy, input.shoot);
        });
    }

} // namespace Ecs