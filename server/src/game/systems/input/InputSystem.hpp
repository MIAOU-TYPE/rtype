/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputSystem
*/

#pragma once

#include "game/world/World.hpp"

namespace Game
{
    /**
     * @brief System responsible for applying player inputs.
     *
     * Reads InputComponent values and writes updated Velocity components.
     * Does not move entities directly—MovementSystem handles that step.
     */
    class InputSystem {
      public:
        /**
         * @brief Update velocities based on current inputs.
         *
         * @param world The game world containing components.
         */
        static void update(IGameWorld &world);
    };

} // namespace Game
