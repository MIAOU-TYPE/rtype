/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** World
*/

#pragma once

#include <iostream>
#include "Drawable.hpp"
#include "Health.hpp"
#include "IGameWorld.hpp"
#include "InputComponent.hpp"
#include "Position.hpp"
#include "Velocity.hpp"

namespace Game
{
    /**
     * @brief Concrete implementation of IGameWorld.
     *
     * World encapsulates a single ECS registry and exposes
     * game-specific entity creation/destruction routines.
     *
     * Responsibilities:
     * - register all components required by the game
     * - create/destroy gameplay entities (e.g., players)
     * - expose ECS registry for systems to operate on
     */
    class World final : public IGameWorld {
      public:
        /**
         * @brief Construct a new World.
         *
         * Registers every component type used in the ECS.
         */
        World();

        /**
         * @brief Access the underlying ECS registry.
         * @return Reference to the registry containing all components/entities.
         */
        [[nodiscard]] Ecs::Registry &registry() override;

        /**
         * @brief Create a new player entity with default components.
         *
         * Initializes:
         * - Position
         * - Velocity
         * - Health
         * - InputComponent
         *
         * @return The newly created ECS entity.
         */
        [[nodiscard]] Ecs::Entity createPlayer() override;

        /**
         * @brief Destroy an entity and all its components.
         *
         * @param ent The entity to remove from the ECS.
         */
        void destroyEntity(Ecs::Entity ent) override;

        /**
         * @brief Copy the state from another IGameWorld instance.
         * @param other The other IGameWorld to copy from.
         */
        void copyFrom(IGameWorld &other) override;

      private:
        Ecs::Registry _registry; ///> The ECS registry (component storage).
    };
} // namespace Game
