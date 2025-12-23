/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AIShootSystem
*/

#include "AIShootSystem.hpp"

namespace
{
    void createProjectile(Ecs::Registry &reg, PendingProjectile pending)
    {
        const Ecs::Entity proj = reg.createEntity();
        reg.emplaceComponent<Ecs::Position>(proj, pending.pos);
        reg.emplaceComponent<Ecs::Velocity>(proj, pending.velocity);
        reg.emplaceComponent<Ecs::Drawable>(proj, Ecs::Drawable{6, true});
        reg.emplaceComponent<Ecs::Lifetime>(proj, Ecs::Lifetime{5.f});
        reg.emplaceComponent<Ecs::Projectile>(proj, pending.projectile);
        reg.emplaceComponent<Ecs::Health>(proj, Ecs::Health{1, 1});
        reg.emplaceComponent<Ecs::Damage>(proj, pending.damage);
        reg.emplaceComponent<Ecs::Collision>(proj, pending.collision);
    }
} // namespace

namespace Game
{
    void AIShootSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        std::vector<PendingProjectile> pendingProjectiles;

        reg.view<Ecs::AIBrain, Ecs::AIShoot, Ecs::Position, Ecs::Collision>(
            [&](const Ecs::Entity ent, Ecs::AIBrain &, Ecs::AIShoot &shoot, const Ecs::Position &pos,
                const Ecs::Collision &col) {
                shoot.timer += dt;
                if (shoot.timer < shoot.cooldown)
                    return;

                const auto posX = pos.x + shoot.muzzle.first;
                const auto posY = pos.y + shoot.muzzle.second;

                shoot.timer = 0.f;

                if (shoot.type == Ecs::AIShoot::Type::Straight) {
                    const float vx = -shoot.projectileSpeed;
                    constexpr float vy = 0.f;

                    pendingProjectiles.emplace_back(
                        posX, posY, vx, vy, shoot.damage, static_cast<size_t>(ent), col.width, col.height);
                    return;
                }

                for (const float angleDeg : shoot.angles) {
                    const float angleRad = angleDeg * (std::numbers::pi_v<float> / 180.f);
                    const float vx = -shoot.projectileSpeed * std::cos(angleRad);
                    const float vy = -shoot.projectileSpeed * std::sin(angleRad);

                    pendingProjectiles.emplace_back(
                        posX, posY, vx, vy, shoot.damage, static_cast<size_t>(ent), col.width, col.height);
                }
            });
        for (const auto &pending : pendingProjectiles)
            createProjectile(reg, pending);
    }
} // namespace Game