/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LifetimeSystem
*/

#include "LifetimeSystem.hpp"

namespace Game
{
    void LifetimeSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::Lifetime, Ecs::Id>([&](const Ecs::Entity, Ecs::Lifetime &life, const Ecs::Id &id) {
            life.remaining -= dt;
            if (life.remaining <= 0.f)
                world.events().emit<DestroyEvent>(DestroyEvent{id.id});
        });
    }
} // namespace Game