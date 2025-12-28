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
        registry.view<Ecs::Position, Ecs::Drawable, Ecs::AnimationState, Ecs::Render>(
            [&](Ecs::Entity, const Ecs::Position &pos, const Ecs::Drawable &drawable, const Ecs::AnimationState &anim,
                const Ecs::Render &render) {
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
                    .frame = animation.frames[anim.frameIndex].rect,
                    .position = {pos.x, pos.y}});
            });
    }
} // namespace Engine