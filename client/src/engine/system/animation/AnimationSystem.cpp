/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationSystem
*/

#include "AnimationSystem.hpp"

namespace Engine
{
    void AnimationSystem::update(AnimationState &state, const Animation &anim, float dt)
    {
        state.elapsed += dt;

        if (state.elapsed >= anim.frames[state.frameIndex].duration) {
            state.elapsed = 0.f;
            state.frameIndex++;

            if (state.frameIndex >= anim.frames.size())
                state.frameIndex = anim.loop ? 0 : anim.frames.size() - 1;
        }
    }
} // namespace Engine
