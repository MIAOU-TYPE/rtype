/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BackgroundSystem
*/

#include "BackgroundSystem.hpp"

namespace Engine
{
    void BackgroundSystem::update(Ecs::Registry &registry, float deltaTime)
    {
        registry.view<Ecs::Position, Ecs::ScrollingBackground>(
            [deltaTime](Ecs::Entity, Ecs::Position &pos, Ecs::ScrollingBackground &bg) {
                pos.x += bg.scrollSpeed * deltaTime;
                if (pos.x <= -bg.spriteWidth) {
                    pos.x += bg.spriteWidth * 2.0f;
                }
            });
    }

    void BackgroundSystem::render(Ecs::Registry &registry, std::vector<RenderCommand> &out, unsigned int viewportWidth,
        unsigned int viewportHeight)
    {
        registry.view<Ecs::Position, Ecs::Render, Ecs::ScrollingBackground>(
            [&](Ecs::Entity, const Ecs::Position &pos, const Ecs::Render &render, const Ecs::ScrollingBackground &bg) {
                const float scaleY = static_cast<float>(viewportHeight) / bg.spriteHeight;
                const float scaleX = scaleY;

                out.push_back({.textureId = render.texture,
                    .frame = {0, 0, static_cast<int>(bg.spriteWidth), static_cast<int>(bg.spriteHeight)},
                    .position = {pos.x, pos.y},
                    .scale = {scaleX, scaleY}});
            });
    }
} // namespace Engine