/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderSystem
*/

#pragma once
#include <algorithm>
#include <memory>
#include "AnimationState.hpp"
#include "Drawable.hpp"
#include "GraphicsTypes.hpp"
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
         * @brief Design-time reference resolution used for position and size scaling.
         * BASE_WIDTH and BASE_HEIGHT define the logical resolution the game was authored for.
         * They are used as scaling factors to map entity coordinates and dimensions to the
         * actual viewport size, keeping rendering consistent across different resolutions.
         */
        static constexpr float BASE_WIDTH = 1280.0f;
        static constexpr float BASE_HEIGHT = 720.0f;
        /**
         * @brief Submits a render command for the given entity and animation to the renderer.
         * @param registry The ECS registry containing entity components.
         * @param spriteRegistry Shared pointer to the sprite registry for retrieving sprite definitions.
         * @param viewportSize Current viewport size for dynamic scaling calculations.
         * @param out Vector to store the generated render commands.
         */
        static void update(Ecs::Registry &registry, const std::shared_ptr<const SpriteRegistry> &spriteRegistry,
            const Graphics::Extent2u &viewportSize, std::vector<RenderCommand> &out);
    };
} // namespace Engine