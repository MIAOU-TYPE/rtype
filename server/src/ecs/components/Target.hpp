/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Target
*/

#pragma once

#include <cstddef>

namespace Ecs
{
    /**
     * @brief Component that defines the target of an entity (usually for AI)
     */
    struct Target {
        std::size_t targetId = 0;  ///> ID of the target entity
        float visionRange = 300.f; ///> Vision range to detect the target
    };
} // namespace Ecs
