/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationSystem
*/

#include "AnimationSystem.hpp"

namespace Engine
{
    void AnimationSystem::update(ClientEntity &e, const Animation &anim, const float dt)
    {
        e.animation.elapsed += dt;

        const auto &frames = anim.frames;

        if (auto &index = e.animation.frameIndex; e.animation.elapsed >= frames[index].duration) {
            e.animation.elapsed = 0.f;
            index++;

            if (index >= frames.size())
                index = anim.loop ? 0 : frames.size() - 1;
        }
    }
} // namespace Engine
