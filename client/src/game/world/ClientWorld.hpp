/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#pragma once
#include <chrono>
#include <iostream>
#include <memory>
#include "AnimationSystem.hpp"
#include "Registry.hpp"
#include "RenderSystem.hpp"
#include "SoundRegistry.hpp"
#include "SpriteRegistry.hpp"
#include "WorldCommand.hpp"
#include <unordered_map>
#include <unordered_set>

namespace World
{
    /**
     * @class ClientWorld
     * @brief Represents the client-side world, managing entities and their components.
     * Responsible for updating the world state and rendering entities.
     */
    class ClientWorld {
      public:
        /**
         * @brief Constructs a ClientWorld with the given SpriteRegistry and SoundRegistry.
         * @param spriteRegistry Shared pointer to the SpriteRegistry used for rendering sprites.
         * @param soundRegistry Shared pointer to the SoundRegistry used for playing sounds.
         */
        explicit ClientWorld(std::shared_ptr<const Engine::SpriteRegistry> spriteRegistry,
            std::shared_ptr<Engine::SoundRegistry> soundRegistry);

        /**
         * @brief Advances the world state by a given delta time.
         * @param dt Delta time since the last update.
         */
        void step(float dt);

        /**
         * @brief Provides access to the entity registry.
         * @return Reference to the entity registry.
         */
        Ecs::Registry &registry();

        /**
         * @brief Applies a world command to the client world.
         * @param cmd The world command to apply.
         */
        void applyCommand(const WorldCommand &cmd);

        /**
         * @brief Gets the current score.
         * @return The current score.
         */
        uint32_t getScore() const;

        /**
         * @brief Applies a snapshot of entities to the client world.
         * @param entities Vector of snapshot entities to apply.
         */
        void applySnapshot(const std::vector<SnapshotEntity> &entities);

        /**
         * @brief Applies damage to an entity and plays hit sound if applicable.
         * @param damageInfo Information about the damage event.
         */
        void applyDamage(const DamageInfo &damageInfo);

        /**
         * @brief Applies a destroy entity command to the client world.
         * @param entityId The ID of the entity to be destroyed.
         * @param wasKilled True if entity was killed (play sound), false otherwise.
         */
        void applyDestroy(size_t entityId, bool wasKilled);

      private:
        /**
         * @struct EntityCreate
         * @brief Data structure for creating a new entity in the client world.
         */
        struct EntityCreate {
            size_t id;             ///> Entity ID
            float x;               ///> X position
            float y;               ///> Y position
            unsigned int spriteId; ///> Sprite identifier
        };

        Ecs::Registry _registry; ///> Entity registry managing entities and their components
        std::shared_ptr<const Engine::SpriteRegistry>
            _spriteRegistry; ///> Shared pointer to the SpriteRegistry for sprite management
        std::shared_ptr<Engine::SoundRegistry>
            _soundRegistry; ///> Shared pointer to the SoundRegistry for sound management

        std::unordered_map<size_t, Ecs::Entity> _entityMap; ///> Maps network entity IDs to local entity IDs

        uint32_t _score = 0; ///> Current player score

        /**
         * @brief Applies a create entity command to the client world.
         * @param data The data for the entity to be created.
         */
        void applyCreate(const EntityCreate &data);

        /**
         * @brief Applies a single snapshot entity update to the client world.
         * @param entity The snapshot entity data to apply.
         */
        void applySingleSnapshot(const SnapshotEntity &entity);

        std::unordered_map<size_t, std::chrono::time_point<std::chrono::steady_clock>>
            _entityLastSeen; ///> Tracks the last seen time for each entity
    };
} // namespace World
