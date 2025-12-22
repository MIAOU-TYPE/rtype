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

        reg.view<Ecs::Health>([&](const Ecs::Entity e, const Ecs::Health &health) {
            if (health.hp <= 0)
                reg.destroyEntity(e);
        });
    }
} // namespace Game
