/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationSystem
*/

#include "AnimationSystem.hpp"

namespace
{
    void resetAnimationState(Engine::AnimationState &animState, const std::string &defaultAnimation)
    {
        animState.currentAnimation = defaultAnimation;
        animState.frameIndex = 0;
        animState.elapsed = 0.f;
    }

    void updateFrameIndex(Engine::AnimationState &animState, const Engine::Animation &anim)
    {
        if (anim.frames.empty())
            return;

        if (animState.frameIndex >= anim.frames.size())
            animState.frameIndex = 0;

        while (anim.frames[animState.frameIndex].duration <= animState.elapsed) {
            animState.elapsed -= anim.frames[animState.frameIndex].duration;
            animState.frameIndex++;

            if (animState.frameIndex >= anim.frames.size()) {
                if (anim.loop) {
                    animState.frameIndex = 0;
                } else {
                    animState.frameIndex = anim.frames.size() - 1;
                    animState.elapsed = 0.f;
                    break;
                }
            }
        }
    }
} // namespace

namespace Engine
{

    void AnimationSystem::update(
        Ecs::Registry &registry, const std::shared_ptr<const SpriteRegistry> &spriteRegistry, const float dt)
    {
        registry.view<Drawable, AnimationState>([&](Ecs::Entity, const Drawable &drawable, AnimationState &animState) {
            if (!spriteRegistry->exists(drawable.spriteId))
                return;

            const SpriteDefinition &sprite = spriteRegistry->get(drawable.spriteId);

            if (animState.currentAnimation.empty())
                animState.currentAnimation = sprite.defaultAnimation;

            auto animIt = sprite.animations.find(animState.currentAnimation);
            if (animIt == sprite.animations.end()) {
                resetAnimationState(animState, sprite.defaultAnimation);
                animIt = sprite.animations.find(animState.currentAnimation);
                if (animIt == sprite.animations.end())
                    return;
            }

            const Animation &anim = animIt->second;
            animState.elapsed += dt;
            updateFrameIndex(animState, anim);
        });
    }
} // namespace Engine
