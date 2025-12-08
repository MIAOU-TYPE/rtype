/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MovementSystem
*/

#include "MovementSystem.hpp"

namespace Game
{
    void MovementSystem::update(IGameWorld &world, float dt)
    {
        world.registry().view<Ecs::Position, Ecs::Velocity>(
            [dt](Ecs::Entity, Ecs::Position &pos, const Ecs::Velocity &vel) {
                pos.x += vel.vx * dt;
                pos.y += vel.vy * dt;
            });
    }
} // namespace Game
