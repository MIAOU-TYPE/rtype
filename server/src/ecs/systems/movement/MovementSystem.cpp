/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MovementSystem
*/

#include "MovementSystem.hpp"

namespace Game
{
    void MovementSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        std::vector<Ecs::Entity> toDestroy;
        reg.view<Ecs::Position, Ecs::Velocity>(
            [&](const Ecs::Entity entity, Ecs::Position &pos, const Ecs::Velocity &vel) {
                pos.x += vel.vx * dt;
                pos.y += vel.vy * dt;

                if (pos.x < 0 || pos.y < 0)
                    toDestroy.emplace_back(entity);
        });
        for (const Ecs::Entity entity : toDestroy)
            reg.destroyEntity(entity);
    }
} // namespace Game
