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
    struct RenderLink {
        std::shared_ptr<Graphics::GraphicalEntity> gfx;
    };
} // namespace Ecs
