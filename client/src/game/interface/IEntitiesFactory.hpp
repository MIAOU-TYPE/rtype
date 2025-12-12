/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** IEntitiesFactory
*/

#pragma once

#include "ITextureManager.hpp"
#include "Registry.hpp"

namespace Ecs
{
    /**
     * @class IEntitiesFactory
     * @brief Interface for an entities factory that creates and manages game entities.
     */
    class IEntitiesFactory {
      public:
        virtual ~IEntitiesFactory();

        /**
         * @brief Creates an entity based on data received from the server.
         * @param id Unique identifier for the entity.
         * @param x X position of the entity.
         * @param y Y position of the entity.
         * @param sprite Sprite identifier for the entity.
         */
        virtual void createEntityFromServer(u_int64_t id, float x, float y, uint16_t sprite) = 0;

        /**
         * @brief Destroys an entity based on data received from the server.
         * @param id Unique identifier for the entity to be destroyed.
         */
        virtual void destroyEntityFromServer(u_int64_t id) = 0;

        /**
         * @brief Updates an entity's damage based on data received from the server.
         * @param id Unique identifier for the entity.
         * @param amount Amount of damage to apply to the entity.
         */
        virtual void updateEntityDamage(u_int32_t id, uint16_t amount) = 0;

        /**
         * @brief Updates an entity's position based on data received from the server.
         * @param id Unique identifier for the entity.
         * @param x New X position of the entity.
         * @param y New Y position of the entity.
         */
        // virtual void updateEntityPosition(u_int32_t id, float x, float y) = 0;

        /**
         * @brief Retrieves an entity by its unique identifier.
         * @param id Unique identifier for the entity.
         * @return An optional containing the entity if found, or std::nullopt if not found.
         */
        virtual std::optional<Entity> getEntityById(u_int64_t id) const = 0;
    };
} // namespace Ecs