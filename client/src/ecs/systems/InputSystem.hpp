/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputSystem
*/

#pragma once

#include "ISystem.hpp"
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
    class InputSystem : public ISystem {
      public:
        /**
         * @brief Constructor.
         * @param registry Reference to the ECS registry.
         */
        explicit InputSystem(Registry &registry);
        /**
         * @brief Update velocities based on input state.
         * @param deltaTime Time elapsed since last update (in seconds).
         */
        void update(float deltaTime) override;

      private:
        Registry &_registry; ///> Reference to the ECS registry.
    };
} // namespace Ecs