/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderSystem
*/

#pragma once
#include <memory>
#include "AnimationState.hpp"
#include "Drawable.hpp"
#include "Registry.hpp"
#include "Render.hpp"
#include "RenderCommand.hpp"
#include "SpriteRegistry.hpp"

namespace Engine
{
    /**
     * @class RenderSystem
     * @brief System responsible for submitting render commands for entities to the renderer.
     * Uses the entity's animation state to determine which frame to render.
     */
    class RenderSystem {
      public:
        /**
         * @brief Submits a render command for the given entity and animation to the renderer.
         * @param registry The ECS registry containing entity components.
         * @param spriteRegistry Shared pointer to the sprite registry for retrieving sprite definitions.
         * @param out Vector to store the generated render commands.
         */
        static void update(Ecs::Registry &registry, const std::shared_ptr<const SpriteRegistry> &spriteRegistry,
            std::vector<RenderCommand> &out);
    };
} // namespace Engine