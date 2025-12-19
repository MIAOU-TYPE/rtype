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
        world.registry().view<InputComponent, Ecs::Position>(
            [](Ecs::Entity, InputComponent &input, Ecs::Position &pos) {
                constexpr float speed = 7.f;
                if (input.left)
                    pos.x -= speed;
                if (input.right)
                    pos.x += speed;
                if (input.up)
                    pos.y -= speed;
                if (input.down)
                    pos.y += speed;
                input.left = false;
                input.right = false;
                input.up = false;
                input.down = false;
                if (pos.x < 0)
                    pos.x = 0;
                if (pos.y < 0)
                    pos.y = 0;
            });
    }
} // namespace Game
