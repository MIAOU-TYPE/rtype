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

        std::vector<Ecs::Entity> toDestroy;

        reg.view<Ecs::Health>([&](const Ecs::Entity e, const Ecs::Health &health) {
            if (health.hp <= 0)
                toDestroy.emplace_back(e);
        });
        for (const auto &entity : toDestroy)
            reg.destroyEntity(entity);
    }
} // namespace Game
