/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#pragma once
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <utility>
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
        [[nodiscard]] Ecs::Registry &registry();

        /**
         * @brief Applies a world command to the client world.
         * @param cmd The world command to apply.
         */
        void applyCommand(const WorldCommand &cmd);

        /**
         * @brief Gets the current score.
         * @return The current score.
         */
        [[nodiscard]] uint32_t getScore() const noexcept;

        /**
         * @brief get the id of the player entity
         * @return the id of the player entity
         */
        [[nodiscard]] int getEntityPlayerId() const noexcept;

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
         * @brief Applies an accept command to the client world.
         * @param data The session ID received from the server.
         */
        void applyAccept(const uint32_t &data);

        /**
         * @brief Updates interpolated positions of entities for smooth rendering.
         */
        void updateInterpolatedPositions(float dt);

        /**
         * @brief Applies local movement based on input flags for the player entity.
         * @param input Bitmask representing movement directions.
         */
        void applyLocalMovementFromNetId(uint8_t input) noexcept;

        /**
         * @brief Resets the client world, clearing all entities and state.
         */
        void reset();

        /**
         * @brief Gets the scores of all rooms.
         * @return A vector of pairs containing room IDs and their corresponding scores.
         */
        [[nodiscard]] std::vector<std::pair<uint32_t, uint32_t>> getRoomScores() const;

      private:
        /**
         * @brief Sets the position of an entity.
         * @param e The entity whose position is to be set.
         * @param spriteId The sprite ID to set.
         * @param drawables sparse array of Drawable components.
         * @param anims sparse array of AnimationState components.
         * @param renders sparse array of Render components.
         */
        void refreshSpriteIfChanged(Ecs::Entity e, uint32_t spriteId, Ecs::SparseArray<Ecs::Drawable> &drawables,
            Ecs::SparseArray<Ecs::AnimationState> &anims, Ecs::SparseArray<Ecs::Render> &renders) const;

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

        /**
         * @struct TickSnapshot
         * @brief Represents a snapshot of the world state at a specific server tick.
         */
        struct TickSnapshot {
            uint32_t tick;                                     ///> Server tick number
            std::chrono::steady_clock::time_point arrivalTime; ///> Time when the snapshot was received
            std::unordered_map<size_t, NetState> entities;     ///> Map of entity IDs to their network states
        };

        std::deque<TickSnapshot> _snapshots; ///> Deque of snapshots for interpolation

        size_t _maxSnapshots = 64; ///> Maximum number of snapshots to store

        std::unordered_set<uint32_t> _destroyed; ///> Set of destroyed entity IDs

        int _entityPlayerId = -1;                                 ///> Client session ID
        std::unordered_map<uint32_t, uint32_t> _scoresByPlayerId; ///> Map of player IDs to their scores

        /**
         * @brief Reconciles the local player entity's position with the server's authoritative state.
         * @param bs The network state received from the server.
         * @param positions Sparse array of Position components.
         */
        void reconcileLocalPlayerWithServer(const NetState &bs, Ecs::SparseArray<Ecs::Position> &positions);

        struct Vel2 {
            float vx;
            float vy;
        };

        std::unordered_map<uint32_t, Vel2> _velByNetId;
        std::unordered_map<uint32_t, std::chrono::steady_clock::time_point> _lastVelUpdate;

        std::chrono::steady_clock::time_point _lastSnapArrival{};
        bool _hasLastArrival = false;
        float _emaJitterMs = 0.f;
        float _interpDelayMs = 100.f;
    };
} // namespace World
