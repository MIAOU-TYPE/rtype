/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputSystem
*/

#pragma once

#include "Registry.hpp"

namespace Ecs
{
    /**
     * @class InputSystem
     * @brief Converts InputComponent state into Velocity.
     *
     * This system reads input state and writes velocity,
     * allowing MovementSystem to apply physics independently.
     */
    class InputSystem {
        public:
        /**
         * @brief Update velocities based on input state.
         * @param registry The ECS registry containing components.
         */
        static void update(Registry &registry, float deltaTime);
    };
} // namespace Ecs