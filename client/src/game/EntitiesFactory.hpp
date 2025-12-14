/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntitiesFactory
*/

#pragma once

#include "Controllable.hpp"
#include "Damage.hpp"
#include "Damageable.hpp"
#include "Direction.hpp"
#include "Health.hpp"
#include "NetClient.hpp"
#include "Position.hpp"
#include "Renderable.hpp"
#include "Velocity.hpp"

namespace Ecs
{
    /**
     * @class EntitiesFactory
     * @brief Concrete implementation of IEntitiesFactory for creating and managing game entities.
     */
    class EntitiesFactory {
      public:
        EntitiesFactory();

        ~EntitiesFactory() = default;

        void parseData(const std::vector<Network::PacketData> &data);

      private:
    };
} // namespace Ecs