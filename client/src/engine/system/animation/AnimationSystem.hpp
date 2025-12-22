/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationSystem
*/

#pragma once
#include "ClientWorld.hpp"
#include "SpriteRegistry.hpp"
#include "Registry.hpp"

namespace Engine
{
    /**
     * @class AnimationSystem
     * @brief System responsible for updating animation states of entities.
     */
    class AnimationSystem {
      public:
        /**
         * @brief Update the animation state for a given entity.
         * @param registry The client registry containing entities and their components.
         * @param spriteRegistry Shared pointer to the sprite registry for retrieving sprite definitions.
         * @param dt The delta time since the last update.
         */
        static void update(Ecs::Registry &registry, std::shared_ptr<const SpriteRegistry> &spriteRegistry, float dt);
    };
} // namespace Engine