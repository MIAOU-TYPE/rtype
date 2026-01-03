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

                RenderCommand cmd;
                cmd.textureId = render.texture;
                cmd.frame = animation.frames[anim.frameIndex].rect;
                cmd.position = {pos.x, pos.y};

                // Special handling for background sprites (spriteId 100+)
                if (drawable.spriteId >= 100 && drawable.spriteId < 200) {
                    // Calculate scale to fit screen
                    const float screenWidth = 900.f;
                    const float screenHeight = 600.f;
                    const float scaleX = screenWidth / static_cast<float>(cmd.frame.w);
                    const float scaleY = screenHeight / static_cast<float>(cmd.frame.h);
                    cmd.scale = {scaleX, scaleY};
                }

                out.push_back(cmd);
            });
    }
} // namespace Engine