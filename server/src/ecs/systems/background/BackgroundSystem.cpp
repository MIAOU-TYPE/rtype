/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BackgroundSystem
*/

#include "BackgroundSystem.hpp"

namespace Game
{
    void BackgroundSystem::update(IGameWorld &world, float deltaTime)
    {
        auto &registry = world.registry();

        registry.view<Ecs::Background, Ecs::Position>([&](Ecs::Entity, Ecs::Background &bg, Ecs::Position &pos) {
            pos.x += bg.scrollSpeed * deltaTime;
        });
    }

    void BackgroundSystem::resetScroll(IGameWorld &world)
    {
        auto &registry = world.registry();

        registry.view<Ecs::Background, Ecs::Position>([&](Ecs::Entity, Ecs::Background &bg, Ecs::Position &pos) {
            if (pos.x + bg.tileWidth <= 0) {
                pos.x += bg.tileWidth * 2.0f;
            }
        });
    }
} // namespace Game