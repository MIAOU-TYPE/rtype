/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"

namespace Engine
{
    void RenderSystem::update(Ecs::Registry &registry, const std::shared_ptr<const SpriteRegistry> &spriteRegistry,
        std::vector<RenderCommand> &out)
    {
        registry.view<Position, Drawable, AnimationState, Render>(
            [&](Ecs::Entity, const Position &pos, const Drawable &drawable, const AnimationState &anim,
                const Render &render) {
                if (!spriteRegistry->exists(drawable.spriteId) || anim.currentAnimation.empty())
                    return;

                const SpriteDefinition &sprite = spriteRegistry->get(drawable.spriteId);

                const auto it = sprite.animations.find(anim.currentAnimation);
                if (it == sprite.animations.end())
                    return;

                const Animation &animation = it->second;
                if (anim.frameIndex >= animation.frames.size())
                    return;

                out.push_back({.textureId = render.texture,
                    .frame = animation.frames.at(anim.frameIndex).rect,
                    .position = {pos.x, pos.y}});
            });
    }
} // namespace Engine