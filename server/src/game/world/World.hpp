/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** World
*/

#pragma once

#include <iostream>
#include "AIBrain.hpp"
#include "Collision.hpp"
#include "Damage.hpp"
#include "Damageable.hpp"
#include "Drawable.hpp"
#include "EventsRegistry.hpp"
#include "Health.hpp"
#include "IGameWorld.hpp"
#include "InputComponent.hpp"
#include "KillScore.hpp"
#include "Position.hpp"
#include "Projectile.hpp"
#include "Score.hpp"
#include "Velocity.hpp"
#include "Lifetime.hpp"

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
         */
        World();

        /**
         * @brief Access the underlying ECS registry.
         * @return Reference to the registry containing all components/entities.
         */
        [[nodiscard]] Ecs::Registry &registry() override;

        /**
         * @brief Access the Events manager for the world.
         * @return Reference to the Events manager.
         */
        [[nodiscard]] Ecs::EventsRegistry &events() override;

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
        Ecs::Registry _registry;     ///> The ECS registry (component storage).
        Ecs::EventsRegistry _events; ///> Event bus for ECS events.
    };
} // namespace Game
