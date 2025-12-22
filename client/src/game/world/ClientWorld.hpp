#pragma once

#include <iostream>
#include "AnimationState.hpp"
#include "AnimationSystem.hpp"
#include "Drawable.hpp"
#include "IRenderer.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "Render.hpp"
#include "RenderSystem.hpp"
#include "SpriteRegistry.hpp"
#include "WorldCommandBuffer.hpp"
#include <unordered_set>

namespace Engine
{
    /**
     * @class ClientWorld
     * @brief Represents the client-side world, managing entities and their components.
     * Responsible for updating the world state and rendering entities.
     */
    class ClientWorld {
      public:
        /**
         * @brief Constructs a ClientWorld with the given SpriteRegistry.
         * @param spriteRegistry Shared pointer to the SpriteRegistry used for rendering sprites.
         */
        explicit ClientWorld(std::shared_ptr<const SpriteRegistry> spriteRegistry);

        /**
         * @brief Updates the world state and renders entities.
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
         * @brief Applies a snapshot of entities to the client world.
         * @param entities Vector of snapshot entities to apply.
         */
        void applySnapshot(const std::vector<SnapshotEntity> &entities);

      private:
        Ecs::Registry _registry; ///> Entity registry managing entities and their components
        std::shared_ptr<const SpriteRegistry>
            _spriteRegistry; ///> Shared pointer to the SpriteRegistry for sprite management

        std::unordered_map<size_t, Ecs::Entity> _entityMap; ///> Maps network entity IDs to local entity IDs

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
    };
} // namespace Engine
