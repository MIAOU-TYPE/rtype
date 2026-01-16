/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AIShootSystem
*/

#include "AIShootSystem.hpp"

namespace Game
{
    void AIShootSystem::spreadProjectile(IGameWorld &world, const Ecs::Entity ent, const Ecs::AIShoot &shoot,
        const float posX, const float posY, const Ecs::WeaponConfig &weapon)
    {
        const float startY = posY - (static_cast<float>(shoot.bulletsNbr) - 1.f) * 10.f;
        for (int i = 0; i < shoot.bulletsNbr; ++i) {
            const float bulletY = startY + static_cast<float>(i) * 50.f;
            const float vx = -shoot.projectileSpeed;
            constexpr float vy = 0.f;
            world.events().emit(ShootEvent(posX, bulletY, vx, vy, shoot.damage, static_cast<size_t>(ent), {8.f, 8.f},
                5.f, weapon.projectileSpriteId));
        }
        return;
    }

    void AIShootSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::AIBrain, Ecs::AIShoot, Ecs::Position, Ecs::WeaponConfig>(
            [&](const Ecs::Entity ent, Ecs::AIBrain &, Ecs::AIShoot &shoot, const Ecs::Position &pos,
                const Ecs::WeaponConfig &weapon) {
                shoot.timer += dt;
                if (shoot.timer < shoot.cooldown)
                    return;

                const auto posX = pos.x + shoot.muzzle.first;
                const auto posY = pos.y + shoot.muzzle.second;

                shoot.timer = 0.f;

                if (shoot.type == Ecs::AIShoot::Type::Straight || shoot.type == Ecs::AIShoot::Type::Homing) {
                    const float vx = -shoot.projectileSpeed;
                    constexpr float vy = 0.f;

                    world.events().emit(ShootEvent(posX, posY, vx, vy, shoot.damage, static_cast<size_t>(ent),
                        {8.f, 8.f}, 5.f, weapon.projectileSpriteId));
                    return;
                }

                if (shoot.type == Ecs::AIShoot::Type::Spread) {
                    spreadProjectile(world, ent, shoot, posX, posY, weapon);
                    return;
                }

                for (const float angleDeg : shoot.angles) {
                    const float angleRad = angleDeg * (std::numbers::pi_v<float> / 180.f);
                    const float vx = -shoot.projectileSpeed * std::cos(angleRad);
                    const float vy = -shoot.projectileSpeed * std::sin(angleRad);

                    world.events().emit(ShootEvent(posX, posY, vx, vy, shoot.damage, static_cast<size_t>(ent),
                        {8.f, 8.f}, 5.f, weapon.projectileSpriteId));
                }
            });
    }
} // namespace Game