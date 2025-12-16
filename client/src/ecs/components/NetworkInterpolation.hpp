/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkInterpolation
*/

#pragma once

namespace Ecs
{
    /**
     * @struct NetworkInterpolation
     * @brief Component that holds network interpolation data for an entity.
     */
    struct NetworkInterpolation {
        float prevX = 0.f;      ///> Previous X position
        float prevY = 0.f;      ///> Previous Y position
        float targetX = 0.f;    ///> Target X position
        float targetY = 0.f;    ///> Target Y position

        float alpha = 1.f;              ///> Interpolation factor
        float snapshotInterval = 0.05f; ///> Time between snapshots
    };
} // namespace Ecs
