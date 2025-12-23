/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LifetimeSystem
*/

#include "LifetimeSystem.hpp"
#include "Lifetime.hpp"

namespace Game
{
    void LifetimeSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        std::vector<Ecs::Entity> toDestroy;
        reg.view<Ecs::Lifetime>([&](const Ecs::Entity e, Ecs::Lifetime &life) {
            life.remaining -= dt;
            if (life.remaining <= 0.f)
                toDestroy.emplace_back(e);
        });
        for (const Ecs::Entity e : toDestroy)
            reg.destroyEntity(e);
    }
} // namespace Game