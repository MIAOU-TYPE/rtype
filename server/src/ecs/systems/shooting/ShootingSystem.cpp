/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootingSystem
*/

#include "ShootingSystem.hpp"

namespace Game
{
    void ShootingSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        reg.view<InputComponent, Ecs::Position, Ecs::WeaponConfig, Ecs::ShootCooldown>(
            [&](const Ecs::Entity entity, InputComponent &input, const Ecs::Position &pos,
                const Ecs::WeaponConfig &weapon, Ecs::ShootCooldown &cooldown) {
                if (!input.shoot)
                    return;

                if (cooldown.timer > 0.f) {
                    input.shoot = false;
                    return;
                }

                input.shoot = false;
                cooldown.timer = cooldown.cooldown;
                world.events().emit<ShootEvent>(ShootEvent{pos.x + 40, pos.y + 7.5f, 100.f, 0.f, 20,
                    static_cast<size_t>(entity), {8.f, 8.f}, 5.f, weapon.projectileSpriteId});
            });
    }

    void ShootingSystem::updateCooldowns(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::ShootCooldown>([&](const Ecs::Entity, Ecs::ShootCooldown &cooldown) {
            if (cooldown.timer > 0.f)
                cooldown.timer -= dt;
        });
    }
} // namespace Game