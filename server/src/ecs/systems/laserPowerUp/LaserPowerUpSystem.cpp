/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LaserPowerUpSystem
*/

#include "LaserPowerUpSystem.hpp"

namespace Game
{
    void LaserPowerUpSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::LaserPowerUp, Ecs::Position>(
            [&](const Ecs::Entity playerEntity, Ecs::LaserPowerUp &laser, const Ecs::Position &playerPos) {
                if (!laser.isActive)
                    return;

                laser.duration += dt;
                if (laser.duration >= laser.maxDuration) {
                    laser.isActive = false;
                    laser.duration = 0.f;
                    return;
                }

                world.events().emit<ShootEvent>(ShootEvent{playerPos.x + 20.f, playerPos.y + 7.f, 0.f, 0.f, 1000,
                    static_cast<size_t>(playerEntity), {656.f, 16.f}, 0.1f, 17, 0, 0});
            });
    }
} // namespace Game
