/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootingSystem
*/

#include "ShootingSystem.hpp"
#include "WeaponConfig.hpp"

namespace Game
{
    void ShootingSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        reg.view<InputComponent, Ecs::Position, Ecs::WeaponConfig>(
            [&](const Ecs::Entity entity, InputComponent &input, const Ecs::Position &pos, const Ecs::WeaponConfig &weapon) {
                if (!input.shoot)
                    return;
                input.shoot = false;
                world.events().emit<ShootEvent>(ShootEvent{
                    pos.x + 30, pos.y, 100.f, 0.f, 20, static_cast<size_t>(entity), {8.f, 8.f}, 5.f, weapon.projectileSpriteId});
            });
    }
} // namespace Game