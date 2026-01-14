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

        reg.view<Ecs::LaserPowerUp, Ecs::Position>([&](const Ecs::Entity playerEntity, Ecs::LaserPowerUp &laser,
                                                       const Ecs::Position &playerPos) {
            if (!laser.isActive)
                return;

            if (!laser.laserEntity.has_value()) {
                const Ecs::Entity laserBeam = world.createEntity();
                reg.emplaceComponent<Ecs::Position>(laserBeam, Ecs::Position{playerPos.x + 40.f, playerPos.y + 7.f});
                reg.emplaceComponent<Ecs::Drawable>(laserBeam, Ecs::Drawable{17, true});
                reg.emplaceComponent<Ecs::Collision>(laserBeam, Ecs::Collision{656.f, 16.f});
                reg.emplaceComponent<Ecs::Damage>(laserBeam, Ecs::Damage{1000});
                laser.laserEntity = laserBeam;
            }

            if (laser.laserEntity.has_value()) {
                const size_t laserIdx = static_cast<size_t>(laser.laserEntity.value());
                if (auto &laserPosOpt = reg.getComponents<Ecs::Position>().at(laserIdx)) {
                    const float newX = playerPos.x + 40.f;
                    const float newY = playerPos.y + 7.f;

                    laserPosOpt->x = newX;
                    laserPosOpt->y = newY;
                }
            }

            laser.duration += dt;
            if (laser.duration >= laser.maxDuration) {
                if (laser.laserEntity.has_value()) {
                    world.destroyEntity(laser.laserEntity.value());
                    laser.laserEntity = std::nullopt;
                }
                laser.isActive = false;
                laser.duration = 0.f;
            }
        });
    }
} // namespace Game
