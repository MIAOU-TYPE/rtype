/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderSystem
*/

#pragma once
#include "Animation.hpp"
#include "EntityClient.hpp"
#include "IRenderer.hpp"
#include "RenderCommand.hpp"

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
         * @param e The client entity to be rendered.
         * @param anim The animation associated with the entity.
         * @param renderer The renderer to which the render command will be submitted.
         */
        void submit(const ClientEntity &e, const Animation &anim, Graphics::IRenderer &renderer);
    };

} // namespace Engine