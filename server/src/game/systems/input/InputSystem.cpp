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
        constexpr float speed = 300.f;
        world.registry().view<InputComponent, Ecs::Velocity>(
            [&](Ecs::Entity, InputComponent &input, Ecs::Velocity &vel) {
                float dx = 0.f;
                float dy = 0.f;

                if (input.left)
                    dx -= 1.f;
                if (input.right)
                    dx += 1.f;
                if (input.up)
                    dy -= 1.f;
                if (input.down)
                    dy += 1.f;

                const float len = std::sqrt(dx * dx + dy * dy);
                if (len > 0.f) {
                    dx /= len;
                    dy /= len;
                }
                vel.vx = dx * speed;
                vel.vy = dy * speed;
            });
    }
} // namespace Game
