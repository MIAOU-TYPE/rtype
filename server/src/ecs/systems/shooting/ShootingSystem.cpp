/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootingSystem
*/

#include "ShootingSystem.hpp"

namespace Game
{
    void ShootingSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<InputComponent, Ecs::Position, Ecs::WeaponConfig>(
            [&](const Ecs::Entity entity, InputComponent &input, const Ecs::Position &pos, Ecs::WeaponConfig &weapon) {
                if (!input.shoot)
                    return;

                if (weapon.timer > 0.f) {
                    weapon.timer -= dt;
                    input.shoot = false;
                    return;
                }

                input.shoot = false;
                weapon.timer = weapon.cooldown;
                world.events().emit<ShootEvent>(ShootEvent{pos.x + 40, pos.y + 7.5f, 100.f, 0.f, 20,
                    static_cast<size_t>(entity), {8.f, 8.f}, 5.f, weapon.projectileSpriteId, 1, 1});
            });
    }
} // namespace Game