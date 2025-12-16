/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputSystem
*/

#include "InputSystem.hpp"
#include <cmath>
#include "Direction.hpp"
#include "InputComponent.hpp"
#include "ShootingComponent.hpp"

namespace Ecs
{
    InputSystem::InputSystem(Registry &registry) : _registry(registry)
    {
    }

    void InputSystem::update(float deltaTime)
    {
        _registry.view<InputComponent, Direction>([](Entity, const InputComponent &input, Direction &dir) {
            if (input.left)
                dir.dx = -1;
            if (input.right)
                dir.dx = 1;
            if (input.up)
                dir.dy = -1;
            if (input.down)
                dir.dy = 1;
        });

        _registry.view<InputComponent, ShootingComponent>(
            [deltaTime](Entity, const InputComponent &input, ShootingComponent &shooting) {
                (void) input;
                if (shooting.cooldown > 0.f)
                    shooting.cooldown = std::max(0.f, shooting.cooldown - deltaTime);
                if (shooting.wantsToShoot) {
                    shooting.cooldown = shooting.fireRate;
                }
            });
    }
} // namespace Ecs
