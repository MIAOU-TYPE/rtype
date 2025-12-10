/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetworkReceiveSystem
*/

#include "NetworkReceiveSystem.hpp"

namespace Ecs
{
    NetworkReceiveSystem::NetworkReceiveSystem(Game::NetClient &client) : _netClient(client)
    {
    }

    void NetworkReceiveSystem::update(float deltaTime)
    {
        _netClient.receivePackets();
    }
} // namespace Ecs