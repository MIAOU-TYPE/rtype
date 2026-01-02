/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#pragma once
#include <iostream>
#include <memory>
#include "AnimationSystem.hpp"
#include "BackgroundSystem.hpp"
#include "IRenderer.hpp"
#include "Registry.hpp"
#include "RenderSystem.hpp"
#include "SpriteRegistry.hpp"
#include "WorldCommand.hpp"
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
         * @brief Constructs a ClientWorld with the given SpriteRegistry.
         * @param spriteRegistry Shared pointer to the SpriteRegistry used for rendering sprites.
         * @param renderer Shared pointer to the renderer for loading textures.
         */
        explicit ClientWorld(std::shared_ptr<const Engine::SpriteRegistry> spriteRegistry,
            std::shared_ptr<Graphics::IRenderer> renderer);

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
         * @brief Applies a snapshot of entities to the client world.
         * @param entities Vector of snapshot entities to apply.
         */
        void applySnapshot(const std::vector<SnapshotEntity> &entities);

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
            _spriteRegistry;                            ///> Shared pointer to the SpriteRegistry for sprite management
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the renderer for loading textures

        std::unordered_map<size_t, Ecs::Entity> _entityMap; ///> Maps network entity IDs to local entity IDs

        /**
         * @brief Initializes the scrolling background with two tiled sprites.
         */
        void initializeScrollingBackground();

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
} // namespace World
