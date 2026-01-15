/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#pragma once
#include <chrono>
#include <cstdint>
#include <deque>
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
        static constexpr uint32_t ServerTickRate = 20; ///> Server tick rate in ticks per second
        static constexpr uint32_t InterpDelayMs = 100; ///> Interpolation delay in milliseconds
        static constexpr uint32_t InterpDelayTicks =
            (ServerTickRate * InterpDelayMs) / 1000; ///> Interpolation delay in ticks

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
         * @param batch The snapshot batch containing entity data.
         */
        void applySnapshot(const SnapshotBatch &batch);

        /**
         * @brief Applies damage to an entity and plays hit sound.
         * @param damageInfo Information about the damage event.
         */
        void applyDamage(const DamageInfo &damageInfo);

        /**
         * @brief Applies a destroy entity command to the client world.
         * @param destroyInfo Information about the entity destruction.
         */
        void applyDestroy(const DestroyInfo &destroyInfo);

        /**
         * @brief Updates interpolated positions of entities for smooth rendering.
         */
        void updateInterpolatedPositions();

      private:
        /**
         * @brief Sets the position of an entity.
         * @param e The entity whose position is to be set.
         * @param spriteId The sprite ID to set.
         * @param drawables sparse array of Drawable components.
         * @param anims sparse array of AnimationState components.
         * @param renders sparse array of Render components.
         */
        void refreshSpriteIfChanged(Ecs::Entity e, uint32_t spriteId, Ecs::SparseArray<Ecs::Drawable> drawables,
            Ecs::SparseArray<Ecs::AnimationState> anims, Ecs::SparseArray<Ecs::Render> renders) const;

        /**
         * @brief Purges stale entities that have not been updated within the specified maximum age.
         * @param maxAge The maximum age for an entity to be considered active.
         */
        void purgeStaleEntities(std::chrono::milliseconds maxAge = std::chrono::milliseconds(500));

        /**
         * @struct EntityCreate
         * @brief Data structure for creating a new entity in the client world.
         */
        struct EntityCreate {
            size_t id;             ///> Entity ID
            float x;               ///> X position
            float y;               ///> Y position
            uint8_t z;             ///> Z position
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

        /**
         * @struct NetState
         * @brief Represents the network state of an entity for interpolation.
         */
        struct NetState {
            float x;           ///> X position
            float y;           ///> Y position
            uint8_t z;         ///> Z position
            uint32_t spriteId; ///> Sprite identifier
        };

        struct TickSnapshot {
            uint32_t tick;                                 ///> Server tick number
            std::unordered_map<size_t, NetState> entities; ///> Map of entity IDs to their network states
        };

        std::deque<TickSnapshot> _snapshots; ///> Deque of snapshots for interpolation

        size_t _maxSnapshots = 64; ///> Maximum number of snapshots to store

        std::unordered_set<uint32_t> _destroyed; ///> Set of destroyed entity IDs
    };
} // namespace World
