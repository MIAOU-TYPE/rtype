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

        reg.view<Ecs::AIBrain, Ecs::AIShoot, Ecs::Position, Ecs::WeaponConfig>(
            [&](const Ecs::Entity ent, Ecs::AIBrain &, Ecs::AIShoot &shoot, const Ecs::Position &pos,
                const Ecs::WeaponConfig &weapon) {
                shoot.timer += dt;
                if (shoot.timer < shoot.cooldown)
                    return;

                const auto posX = pos.x + shoot.muzzle.first;
                const auto posY = pos.y + shoot.muzzle.second;

                shoot.timer = 0.f;

                if (shoot.type == Ecs::AIShoot::Type::Straight) {
                    const float vx = -shoot.projectileSpeed;
                    constexpr float vy = 0.f;

                    world.events().emit(ShootEvent(posX, posY, vx, vy, shoot.damage, static_cast<size_t>(ent),
                        {8.f, 8.f}, 5.f, weapon.projectileSpriteId));
                    return;
                }

                if (shoot.type == Ecs::AIShoot::Type::Spread) {
                    const float startY = posY - (static_cast<float>(shoot.bulletsNbr) - 1.f) * 10.f;
                    for (int i = 0; i < shoot.bulletsNbr; ++i) {
                        const float bulletY = startY + static_cast<float>(i) * 50.f;
                        const float vx = -shoot.projectileSpeed;
                        constexpr float vy = 0.f;
                        world.events().emit(ShootEvent(posX, bulletY, vx, vy, shoot.damage, static_cast<size_t>(ent),
                            {8.f, 8.f}, 5.f, weapon.projectileSpriteId));
                    }
                    return;
                }

                if (shoot.type == Ecs::AIShoot::Type::Homing) {
                    size_t targetId = SIZE_MAX;

                    reg.view<Game::InputComponent, Ecs::Position, Ecs::Health, Ecs::Id>(
                        [&](const Ecs::Entity, const Game::InputComponent &, const Ecs::Position &,
                            const Ecs::Health &hp, const Ecs::Id &id) {
                            if (hp.hp > 0 && targetId == SIZE_MAX) {
                                targetId = id.id;
                            }
                        });

                    if (targetId != SIZE_MAX) {
                        auto &playerPos = reg.getComponents<Ecs::Position>().at(targetId);
                        if (playerPos) {
                            float dx = playerPos->x - posX;
                            float dy = playerPos->y - posY;
                            float distance = std::sqrt(dx * dx + dy * dy);
                            if (distance > 0) {
                                float vx = (dx / distance) * shoot.projectileSpeed;
                                float vy = (dy / distance) * shoot.projectileSpeed;
                                world.events().emit(ShootEvent(posX, posY, vx, vy, shoot.damage,
                                    static_cast<size_t>(ent), {8.f, 8.f}, 5.f, weapon.projectileSpriteId));
                            }
                        }
                    }
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