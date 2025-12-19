/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationLibrary
*/

#pragma once
#include <string>
#include "Animation.hpp"
#include <unordered_map>

namespace Engine
{
    /**
     * @class AnimationLibrary
     * @brief Manages a collection of animations for different sprites.
     * Allows retrieval of animations by sprite ID and animation name.
     */
    class AnimationLibrary {
      public:
        /**
         * @brief Adds an animation for a specific sprite.
         * @param spriteId String identifier for the sprite.
         * @param animationName Name of the animation.
         * @return Reference to the newly added Animation.
         */
        const Animation &get(const std::string &spriteId, const std::string &animationName) const;

      private:
        std::unordered_map<std::string, std::unordered_map<std::string, Animation>>
            _animations; ///> Map of sprite IDs to their animations
    };
} // namespace Engine