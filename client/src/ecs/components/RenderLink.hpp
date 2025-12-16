/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderLink
*/

#pragma once
#include <memory>
#include "SFMLEntityCreation.hpp"

namespace Ecs
{
    /** @struct RenderLink
     * @brief Component that links an ECS entity to its graphical representation.
     */
    struct RenderLink {
        std::shared_ptr<Graphics::GraphicalEntity> gfx; ///> Pointer to the graphical entity.
    };
} // namespace Ecs
