/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** TailFollowerSystem
*/

#pragma once

#include "IGameWorld.hpp"
#include "Position.hpp"
#include "TailFollower.hpp"
#include "Velocity.hpp"

#include <cmath>

namespace Game
{
    /**
     * @class TailFollowerSystem
     * @brief System to update tail follower entities.
     * Entities with the TailFollower component will follow their leader entities.
     */
    class TailFollowerSystem {
      public:
        /**
         * @brief Update the tail follower entities to follow their leaders.
         *
         * This function updates the position of tail follower entities based on
         * the position of their leader entities, maintaining a specified distance
         * and speed.
         *
         * @param world The game world containing entities and components.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game