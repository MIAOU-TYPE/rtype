/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationFrame
*/

#pragma once

namespace Engine
{
    /**
     * @struct IntRect
     * @brief Represents a rectangle defined by its position (x, y) and size (w, h).
     * Used for defining frames in animations.
     */
    struct IntRect {
        int x; ///> X position of the rectangle
        int y; ///> Y position of the rectangle
        int w; ///> Width of the rectangle
        int h; ///> Height of the rectangle
    };

    /**
     * @struct AnimationFrame
     * @brief Represents a single frame in an animation.
     * Contains the rectangle defining the frame and its duration.
     */
    struct AnimationFrame {
        IntRect rect;   ///> Rectangle defining the frame
        float duration; ///> Duration of the frame in seconds
    };
} // namespace Engine