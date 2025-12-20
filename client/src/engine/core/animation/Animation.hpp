/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Animation
*/

#pragma once
#include <string>
#include <vector>
#include "AnimationFrame.hpp"

namespace Engine
{
    /**
     * @struct Animation
     * @brief Represents an animation consisting of multiple frames.
     */
    struct Animation {
        std::string name;                   ///> Name of the animation
        std::vector<AnimationFrame> frames; ///> Frames of the animation
        bool loop;                          ///> Indicates if the animation should loop
    };
} // namespace Engine
