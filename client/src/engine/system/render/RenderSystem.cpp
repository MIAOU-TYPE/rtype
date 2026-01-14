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
        const Graphics::Extent2u &viewportSize, std::vector<RenderCommand> &out)
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

                RenderCommand cmd;
                cmd.textureId = render.texture;
                cmd.frame = animation.frames[anim.frameIndex].rect;
                cmd.position = {pos.x, pos.y};
                float finalX = pos.x;
                float finalY = pos.y;
                if (drawable.spriteId == 16) {
                    const float viewportHeight = static_cast<float>(viewportSize.height);
                    finalX = 10.f;
                    finalY = viewportHeight - 50.f - pos.y;
                    cmd.position = {finalX, finalY};
                    cmd.scale = {2.f, 2.f};
                }

                out.push_back(cmd);
            });
    }
} // namespace Engine