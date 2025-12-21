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
        world.registry().view<Ecs::Position, Ecs::Velocity>(
            [&](const Ecs::Entity entity, Ecs::Position &pos, const Ecs::Velocity &vel) {
                pos.x += vel.vx * dt;
                pos.y += vel.vy * dt;

                if (pos.x < 0)
                    world.registry().destroyEntity(entity);
                if (pos.y < 0)
                    world.registry().destroyEntity(entity);
            });
    }
} // namespace Game
