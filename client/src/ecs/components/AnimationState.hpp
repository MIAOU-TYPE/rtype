/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationState
*/

#pragma once
#include <cstddef>
#include <string>

namespace Ecs
{
    /**
    ** @brief Component that holds the state of an animation for an entity.
    */
    struct AnimationState {
        std::string currentAnimation; ///> Name of the current animation.
        std::size_t frameIndex = 0;   ///> Current frame index in the animation.
        float elapsed = 0.f;          ///> Time elapsed since the last frame change.
    };
} // namespace Engine
