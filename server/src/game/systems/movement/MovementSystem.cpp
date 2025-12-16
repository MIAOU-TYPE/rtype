/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MovementSystem
*/

#include "MovementSystem.hpp"

namespace Game
{
    void MovementSystem::update(IGameWorld &world, float deltaTime)
    {
        world.registry().view<Ecs::Position, Ecs::Velocity>([&](Ecs::Entity, Ecs::Position &pos, Ecs::Velocity &vel) {
            pos.x += vel.vx * deltaTime;
            pos.y += vel.vy * deltaTime;

            if (pos.x < 0)
                pos.x = -100;
            if (pos.y < 0)
                pos.y = -100;
        });
    }
} // namespace Game
