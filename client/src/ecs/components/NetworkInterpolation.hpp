/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkInterpolation
*/

#pragma once

namespace Ecs
{
    struct NetworkInterpolation {
        float prevX = 0.f;
        float prevY = 0.f;
        float targetX = 0.f;
        float targetY = 0.f;

        float alpha = 1.f;
        float snapshotInterval = 0.05f;
    };
}
