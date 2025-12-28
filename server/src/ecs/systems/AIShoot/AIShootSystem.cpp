/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AIShootSystem
*/

#include "AIShootSystem.hpp"

namespace Game
{
    void AIShootSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::AIBrain, Ecs::AIShoot, Ecs::Position>(
            [&](const Ecs::Entity ent, Ecs::AIBrain &, Ecs::AIShoot &shoot, const Ecs::Position &pos) {
                shoot.timer += dt;
                if (shoot.timer < shoot.cooldown)
                    return;

                const auto posX = pos.x + shoot.muzzle.first;
                const auto posY = pos.y + shoot.muzzle.second;

                shoot.timer = 0.f;

                if (shoot.type == Ecs::AIShoot::Type::Straight) {
                    const float vx = -shoot.projectileSpeed;
                    constexpr float vy = 0.f;

                    world.events().emit(
                        ShootEvent(posX, posY, vx, vy, shoot.damage, static_cast<size_t>(ent), {8.f, 8.f}, 5.f));
                    return;
                }

                for (const float angleDeg : shoot.angles) {
                    const float angleRad = angleDeg * (std::numbers::pi_v<float> / 180.f);
                    const float vx = -shoot.projectileSpeed * std::cos(angleRad);
                    const float vy = -shoot.projectileSpeed * std::sin(angleRad);

                    world.events().emit(
                        ShootEvent(posX, posY, vx, vy, shoot.damage, static_cast<size_t>(ent), {8.f, 8.f}, 5.f));
                }
            });
    }
} // namespace Game