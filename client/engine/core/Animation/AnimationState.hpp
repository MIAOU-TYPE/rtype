/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationState
*/

#pragma once
#include <string>

namespace Engine
{
    /**
     * @struct AnimationState
     * @brief Represents the state of an animation for an entity.
     */
    struct AnimationState {
        std::string currentAnimation; ///> Name of the current animation
        size_t frameIndex = 0;        ///> Current frame index in the animation
        float elapsed = 0.f;          ///> Elapsed time since the last frame change
    };
} // namespace Engine