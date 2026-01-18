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
                const auto scalePosX = static_cast<float>(viewportSize.width) / RenderSystem::BASE_WIDTH;
                const auto scalePosY = static_cast<float>(viewportSize.height) / RenderSystem::BASE_HEIGHT;
                cmd.position = {pos.x * scalePosX, pos.y * scalePosY, pos.z};
                cmd.scale = {SCALE_FACTOR, SCALE_FACTOR};

                if (drawable.spriteId >= 100 && drawable.spriteId <= 103) {
                    const auto viewportWidth = static_cast<float>(viewportSize.width);
                    const auto viewportHeight = static_cast<float>(viewportSize.height);
                    const auto frameWidth = static_cast<float>(cmd.frame.w);
                    const auto frameHeight = static_cast<float>(cmd.frame.h);

                    const float scaleX = viewportWidth / frameWidth;
                    const float scaleY = viewportHeight / frameHeight;

                    const float scale = std::max(scaleX, scaleY);
                    cmd.scale = {scale, scale};
                }

                float finalX = pos.x;
                float finalY = pos.y;
                if (drawable.spriteId == 16) {
                    const float viewportHeight = static_cast<float>(viewportSize.height);
                    finalX = 10.f;
                    finalY = viewportHeight - 50.f - pos.y;
                    cmd.position = {finalX, finalY};
                    cmd.scale = {2.f, 2.f};
                }

                if (drawable.spriteId == 20) {
                    cmd.scale = {3.f, 3.f};
                    cmd.position = {finalX - 20.f, finalY - 30.f};
                }

                out.push_back(cmd);
            });
        std::ranges::sort(out, [](const RenderCommand &a, const RenderCommand &b) {
            return a.position.z < b.position.z;
        });
    }
} // namespace Engine