/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IGameWorld
*/

#pragma once

#include "Entity.hpp"
#include "EventsRegistry.hpp"
#include "Registry.hpp"
#include "GameConfig.hpp"

/**
 * @brief Interface representing an abstract game world.
 *
 * IGameWorld exposes the minimal set of operations required by the
 * GameServer and ECS systems:
 *
 * - access to the ECS registry
 * - creation/removal of gameplay entities
 *
 * This abstraction allows multiple world implementations:
 * - real in-game world (World)
 * - mock world for unit tests
 * - alternate worlds for menu/lobby systems
 */
namespace Game
{
    class IGameWorld {
      public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IGameWorld() = default;

        /**
         * @brief Get the ECS registry manipulated by systems.
         */
        [[nodiscard]] virtual Ecs::Registry &registry() = 0;

        /**
         * @brief Get the Events manager for the world.
         * @return Reference to the Events manager.
         */
        [[nodiscard]] virtual Ecs::EventsRegistry &events() = 0;

        /**
         * @brief Create a new gameplay entity (e.g., player).
         * @param sessionId The session ID associated with the player.
         */
        [[nodiscard]] virtual Ecs::Entity createPlayer(int sessionId) = 0;

        /**
         * @brief Destroy an entity and all its components.
         * @param ent The entity to destroy.
         */
        virtual void destroyEntity(Ecs::Entity ent) = 0;

        /**
         * @brief Create a new generic entity with network ID.
         */
        [[nodiscard]] virtual Ecs::Entity createEntity() = 0;

        /**
         * @brief Copy the state from another IGameWorld instance.
         * @param other The other IGameWorld to copy from.
         */
        virtual void copyFrom(IGameWorld &other) {}

        /**
         * @brief Get the game configuration.
         * @return Reference to the game configuration.
         */
        [[nodiscard]] virtual const Engine::GameConfig &getGameConfig() const = 0;
    };
} // namespace Game