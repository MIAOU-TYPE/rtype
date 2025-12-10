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
#include "Direction.hpp"

namespace Ecs
{
    InputSystem::InputSystem(Registry &registry) : _registry(registry)
    {
    }

    void InputSystem::update(float deltaTime)
    {
        _registry.view<InputComponent, Direction>([](Entity, const InputComponent &input, Direction &dir) {
            if (input.left)
                dir.dx = -1.f;
            if (input.right)
                dir.dx = 1.f;
            if (input.up)
                dir.dy = -1.f;
            if (input.down)
                dir.dy = 1.f;
        });

        _registry.view<InputComponent, ShootingComponent>(
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
