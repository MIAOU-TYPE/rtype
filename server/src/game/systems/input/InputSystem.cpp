/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputSystem
*/

#include "InputSystem.hpp"

namespace Game
{
    void InputSystem::update(IGameWorld &world)
    {
        world.registry().view<InputComponent, Ecs::Velocity>(
            [](Ecs::Entity, const InputComponent &input, Ecs::Velocity &vel) {
                constexpr float speed = 200.f;
                vel.vx = 0.f;
                vel.vy = 0.f;
                if (input.left)
                    vel.vx -= speed;
                if (input.right)
                    vel.vx += speed;
                if (input.up)
                    vel.vy -= speed;
                if (input.down)
                    vel.vy += speed;
            });
    }
} // namespace Game
