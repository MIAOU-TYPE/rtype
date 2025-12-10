/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IGameWorld
*/

#pragma once

#include "Entity.hpp"
#include "Registry.hpp"

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
        virtual Ecs::Registry &registry() = 0;

        /**
         * @brief Create a new gameplay entity (e.g., player).
         */
        virtual Ecs::Entity createPlayer() = 0;

        /**
         * @brief Destroy an entity and all its components.
         * @param ent The entity to destroy.
         */
        virtual void destroyEntity(Ecs::Entity ent) = 0;
    };
} // namespace Game