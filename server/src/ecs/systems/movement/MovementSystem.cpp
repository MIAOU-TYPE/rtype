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

        reg.view<Ecs::Position, Ecs::Velocity, Ecs::Id>(
            [&](const Ecs::Entity, Ecs::Position &pos, const Ecs::Velocity &vel, const Ecs::Id &id) {
                pos.x += vel.vx * dt;
                pos.y += vel.vy * dt;

                if (pos.x < 0 || pos.y < 0)
                    world.events().emit<DestroyEvent>(DestroyEvent{id.id, false});
            });
    }
} // namespace Game
