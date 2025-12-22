/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LifetimeSystem
*/

#include "LifetimeSystem.hpp"
#include "CommandBuffer.hpp"
#include "Lifetime.hpp"

namespace Game
{
    void LifetimeSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::Lifetime>([&](Ecs::Entity e, Ecs::Lifetime &life) {
            life.remaining -= dt;
            if (life.remaining <= 0.f)
                reg.destroyEntity(e);
        });
    }
} // namespace Game