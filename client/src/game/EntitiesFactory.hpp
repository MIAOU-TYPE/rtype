/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntitiesFactory
*/

#pragma once

#include "Health.hpp"
#include "Velocity.hpp"
#include "Damage.hpp"
#include "Controllable.hpp"
#include "Damageable.hpp"
#include "Direction.hpp"
#include "Position.hpp"
#include "Renderable.hpp"
#include "IEntitiesFactory.hpp"

namespace Ecs
{
    /**
     * @class EntitiesFactory
     * @brief Concrete implementation of IEntitiesFactory for creating and managing game entities.
     */
    class EntitiesFactory : public IEntitiesFactory {
      public:
        /**
         * @brief Constructor for EntitiesFactory.
         * @param registry Reference to the ECS registry.
         * @param textureManager Shared pointer to the texture manager.
         */
        EntitiesFactory(Registry &registry, std::shared_ptr<Graphics::ITextureManager> textureManager);

        ~EntitiesFactory() = default;

        /**
         * @brief Creates an entity based on data received from the server.
         * @param id Unique identifier for the entity.
         * @param x X position of the entity.
         * @param y Y position of the entity.
         * @param sprite Sprite identifier for the entity.
         */
        void createEntityFromServer(u_int64_t id, float x, float y, uint16_t sprite) override;

        /**
         * @brief Destroys an entity based on data received from the server.
         * @param id Unique identifier for the entity to be destroyed.
         */
        void destroyEntityFromServer(u_int64_t id) override;

        /**
         * @brief Updates an entity's damage based on data received from the server.
         * @param id Unique identifier for the entity.
         * @param amount Amount of damage to apply to the entity.
         */
        void updateEntityDamage(u_int32_t id, uint16_t amount) override;

        /**
         * @brief Updates an entity's position based on data received from the server.
         * @param id Unique identifier for the entity.
         * @param x New X position of the entity.
         * @param y New Y position of the entity.
         */
        // void updateEntityPosition(u_int32_t id, float x, float y) override;

        /**
         * @brief Retrieves an entity by its unique identifier.
         * @param id Unique identifier for the entity.
         * @return An optional containing the entity if found, or std::nullopt if not found.
         */
        std::optional<Entity> getEntityById(u_int64_t id) const override;

      private:
        Registry &_registry;                                        ///> Reference to the ECS registry
        std::shared_ptr<Graphics::ITextureManager> _textureManager; ///> Shared pointer to the texture manager
        std::unordered_map<u_int64_t, Entity> _entityMap;           ///> Map of unique IDs to entities

        /** Helper methods to create specific entity types */
        void createPlayer(Entity entity, float x, float y, uint16_t sprite);
        void createEnemy(Entity entity, float x, float y, uint16_t sprite);
        void createProjectile(Entity entity, float x, float y, uint16_t sprite);
    };
} // namespace Ecs