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

        reg.view<InputComponent, Ecs::Position>([&reg](Ecs::Entity, InputComponent &input, const Ecs::Position &pos) {
            if (!input.shoot)
                return;
            input.shoot = false;
            const Ecs::Entity proj = reg.createEntity();
            reg.emplaceComponent<Ecs::Position>(proj, pos.x + 20, pos.y);
            reg.emplaceComponent<Ecs::Velocity>(proj, 100.f, 0.f);
            reg.emplaceComponent<Ecs::Damage>(proj, 20);
            reg.emplaceComponent<Ecs::Damageable>(proj);
            reg.emplaceComponent<Ecs::Collision>(proj, 8.f, 8.f);
            reg.emplaceComponent<Ecs::Drawable>(proj, static_cast<unsigned int>(6));
        });
    }
} // namespace Game