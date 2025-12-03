/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Camera
*/

#pragma once

namespace Ecs {
    /**
     * @struct Camera
     * @brief Component for camera properties.
     * This component defines camera behavior such as offset,
     * zoom level, smoothness of movement, and world boundaries.
     */
    struct Camera {
        float offsetX = 0.f;             // Offset from center
        float offsetY = 0.f;
        float zoom = 1.0f;               // Zoom factor (1.0 = normal)
        float smoothness = 5.0f;         // Follow speed (0 = instant, higher = smoother)
        bool active = true;              // Main camera or not
        
        // Camera limits (world bounds)
        float minX = 0.f;
        float maxX = 10000.f;
        float minY = 0.f;
        float maxY = 10000.f;
    };
}