/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntitiesFactory
*/

#include "EntitiesFactory.hpp"

namespace Ecs
{
    EntitiesFactory::EntitiesFactory(std::vector<Network::PacketData> &data, Registry &registry) : _registry(registry)
    {
    }
} // namespace Ecs
