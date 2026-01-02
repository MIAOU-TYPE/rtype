/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BackgroundSystem
*/

#pragma once

#include <vector>
#include "Position.hpp"
#include "Registry.hpp"
#include "Render.hpp"
#include "RenderCommand.hpp"
#include "ScrollingBackground.hpp"

namespace Engine
{
    /**
     * @class BackgroundSystem
     * @brief System responsible for updating scrolling backgrounds.
     */
    class BackgroundSystem {
      public:
        /**
         * @brief Updates the scrolling backgrounds based on their scroll speed and the elapsed time.
         * @param registry The ECS registry containing entity components.
         * @param deltaTime The time elapsed since the last update (in seconds).
         */
        static void update(Ecs::Registry &registry, float deltaTime);

        /**
         * @brief Generates render commands for scrolling backgrounds.
         * @param registry The ECS registry containing entity components.
         * @param out Vector to store the generated render commands.
         * @param viewportWidth Width of the viewport for scaling.
         * @param viewportHeight Height of the viewport for scaling.
         */
        static void render(Ecs::Registry &registry, std::vector<RenderCommand> &out, unsigned int viewportWidth,
            unsigned int viewportHeight);
    };
} // namespace Engine