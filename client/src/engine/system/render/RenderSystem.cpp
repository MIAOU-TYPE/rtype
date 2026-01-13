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

                constexpr float SCALE_FACTOR = 1.7f;

                RenderCommand cmd;
                cmd.textureId = render.texture;
                cmd.frame = animation.frames[anim.frameIndex].rect;
                cmd.position = {pos.x, pos.y};
                cmd.scale = {1.f * SCALE_FACTOR, 1.f * SCALE_FACTOR};

                if (drawable.spriteId >= 100 && drawable.spriteId < 102) {
                    const float viewportWidth = static_cast<float>(viewportSize.width);
                    const float viewportHeight = static_cast<float>(viewportSize.height);
                    const float frameWidth = static_cast<float>(cmd.frame.w);
                    const float frameHeight = static_cast<float>(cmd.frame.h);

                    const float scaleX = viewportWidth / frameWidth;
                    const float scaleY = viewportHeight / frameHeight;

                    const float scale = std::max(scaleX, scaleY);
                    cmd.scale = {scale, scale};
                }

                out.push_back(cmd);
            });
    }
} // namespace Engine