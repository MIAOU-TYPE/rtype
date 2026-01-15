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
        world.registry().view<InputComponent, Ecs::Position, Ecs::Collision>(
            [](Ecs::Entity, InputComponent &input, Ecs::Position &pos, const Ecs::Collision &col) {
                constexpr float speed = Game::Config::PLAYER_SPEED;
                constexpr float minX = 0.f;
                constexpr float minY = 0.f;
                const float maxX = Game::Config::VIEWPORT_WIDTH - col.width;
                const float maxY = Game::Config::VIEWPORT_HEIGHT - col.height;
                float dx = 0.f, dy = 0.f;
                constexpr float inv_sqrt2 = 0.7071067811865476f;

                if (input.left)
                    dx -= speed;
                if (input.right)
                    dx += speed;
                if (input.up)
                    dy -= speed;
                if (input.down)
                    dy += speed;

                if (dx != 0.f && dy != 0.f) {
                    dx *= inv_sqrt2;
                    dy *= inv_sqrt2;
                }
                pos.x += dx;
                pos.y += dy;
                input.left = false;
                input.right = false;
                input.up = false;
                input.down = false;
                if (pos.x < minX)
                    pos.x = minX;
                if (pos.x > maxX)
                    pos.x = maxX;
                if (pos.y < minY)
                    pos.y = minY;
                if (pos.y > maxY)
                    pos.y = maxY;
            });
    }
} // namespace Game
