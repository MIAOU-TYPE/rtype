/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputSystem
*/

#include "InputSystem.hpp"
#include <cmath>
#include "InputComponent.hpp"
#include "ShootingComponent.hpp"
#include "Velocity.hpp"

namespace Ecs
{
    void InputSystem::update(Registry &registry, float deltaTime)
    {
        constexpr float MOVE_SPEED = 200.f;

        registry.view<InputComponent, Velocity>([](Entity, const InputComponent &input, Velocity &vel) {
            vel.vx = 0.f;
            vel.vy = 0.f;

            if (input.left)
                vel.vx -= MOVE_SPEED;
            if (input.right)
                vel.vx += MOVE_SPEED;
            if (input.up)
                vel.vy -= MOVE_SPEED;
            if (input.down)
                vel.vy += MOVE_SPEED;

            if (vel.vx != 0.f && vel.vy != 0.f) {
                float magnitude = std::sqrt(vel.vx * vel.vx + vel.vy * vel.vy);
                vel.vx = (vel.vx / magnitude) * MOVE_SPEED;
                vel.vy = (vel.vy / magnitude) * MOVE_SPEED;
            }
        });

        registry.view<InputComponent, ShootingComponent>(
            [deltaTime](Entity, const InputComponent &input, ShootingComponent &shooting) {
                if (shooting.cooldown > 0.f)
                    shooting.cooldown = std::max(0.f, shooting.cooldown - deltaTime);
                shooting.wantsToShoot = input.shoot && (shooting.cooldown <= 0.f);
                if (shooting.wantsToShoot) {
                    shooting.cooldown = shooting.fireRate;
                }
            });
    }
} // namespace Ecs
