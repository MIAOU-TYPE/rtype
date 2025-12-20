/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteDefinition
*/

#pragma once
#include <string>
#include "Animation/Animation.hpp"
#include <unordered_map>

namespace Engine
{
    /**
     * @struct SpriteDefinition
     * @brief Defines a sprite with its texture path, default animation, and available animations.
     */
    struct SpriteDefinition {
        std::string texturePath;                               ///> Path to the texture file
        std::string defaultAnimation;                          ///> Name of the default animation
        std::unordered_map<std::string, Animation> animations; ///> Map of animation names to Animation objects
    };
} // namespace Engine