/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationSystem
*/

#pragma once
#include "Animation.hpp"
#include "EntityClient.hpp"

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
         * @param e The client entity whose animation state is to be updated.
         * @param anim The animation associated with the entity
         * @param dt The delta time since the last update (in seconds).
         */
        static void update(ClientEntity &e, const Animation &anim, float dt);
    };
} // namespace Engine