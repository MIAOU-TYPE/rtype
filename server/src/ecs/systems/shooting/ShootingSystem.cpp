/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootingSystem
*/

#include "ShootingSystem.hpp"

struct ShootToCreate {
    float x;
    float y;
    float vx;
    float vy;
    int damage;
};

namespace
{
    void createEntityProjectile(
        Ecs::Registry &reg, const float x, const float y, const float vx, const float vy, const int damage)
    {
        const Ecs::Entity proj = reg.createEntity();
        reg.emplaceComponent<Ecs::Position>(proj, Ecs::Position{x, y});
        reg.emplaceComponent<Ecs::Velocity>(proj, Ecs::Velocity{vx, vy});
        reg.emplaceComponent<Ecs::Damage>(proj, Ecs::Damage{damage});
        reg.emplaceComponent<Ecs::Damageable>(proj);
        reg.emplaceComponent<Ecs::Collision>(proj, Ecs::Collision{8.f, 8.f});
        reg.emplaceComponent<Ecs::Drawable>(proj, Ecs::Drawable{6, true});
        reg.emplaceComponent<Ecs::Health>(proj, Ecs::Health{1, 1});
        reg.emplaceComponent<Ecs::Lifetime>(proj, Ecs::Lifetime{6.f});
    }

} // namespace

namespace Game
{
    void ShootingSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        std::vector<ShootToCreate> toCreate;

        reg.view<InputComponent, Ecs::Position>([&](Ecs::Entity, InputComponent &input, const Ecs::Position &pos) {
            if (!input.shoot)
                return;
            input.shoot = false;
            toCreate.emplace_back(ShootToCreate{pos.x + 30, pos.y, 100.f, 0.f, 20});
        });
        for (const auto &[x, y, vx, vy, damage] : toCreate)
            createEntityProjectile(reg, x, y, vx, vy, damage);
    }
} // namespace Game