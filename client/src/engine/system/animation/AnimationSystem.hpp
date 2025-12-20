/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationSystem
*/

#pragma once
#include "Animation.hpp"
#include "AnimationState.hpp"

namespace Engine
{
    /**
     * @class AnimationSystem
     * @brief System responsible for updating the animation state of client entities.
     * Updates the current frame of the animation based on elapsed time.
     */
    class AnimationSystem {
      public:
        /**
         * @brief Updates the animation state of the given client entity based on the provided animation and delta time.
         * @param state
         * @param anim The animation associated with the entity
         * @param dt The delta time since the last update (in seconds).
         */
        static void update(AnimationState &state, const Animation &anim, float dt);
    };
} // namespace Engine