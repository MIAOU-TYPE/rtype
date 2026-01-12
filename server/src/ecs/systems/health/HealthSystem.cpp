/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** HealthSystem
*/

#include "HealthSystem.hpp"

namespace Game
{
    void HealthSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        reg.view<Ecs::Health, Ecs::Id>([&](const Ecs::Entity, const Ecs::Health &health, const Ecs::Id &id) {
            if (health.hp <= 0)
                world.events().emit<DestroyEvent>(DestroyEvent{id.id});
        });
    }
} // namespace Game
