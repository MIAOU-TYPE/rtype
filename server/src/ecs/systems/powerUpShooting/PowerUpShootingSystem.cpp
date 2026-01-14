/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUpShootingSystem
*/

#include "PowerUpShootingSystem.hpp"

namespace Game
{
    void PowerUpShootingSystem::createPowerUpProjectile(
        IGameWorld &world, const float x, const float y, const float vx, const float vy, const size_t shooterId)
    {
        auto &reg = world.registry();
        const Ecs::Entity proj = world.createEntity();

        reg.emplaceComponent<Ecs::Position>(proj, Ecs::Position{x, y});
        reg.emplaceComponent<Ecs::Velocity>(proj, Ecs::Velocity{vx, vy});
        reg.emplaceComponent<Ecs::Drawable>(proj, Ecs::Drawable{15, true});
        reg.emplaceComponent<Ecs::Collision>(proj, Ecs::Collision{48.f, 24.f});
        reg.emplaceComponent<Ecs::Projectile>(proj, Ecs::Projectile{shooterId});
        reg.emplaceComponent<Ecs::Damage>(proj, Ecs::Damage{150});
        reg.emplaceComponent<Ecs::Lifetime>(proj, Ecs::Lifetime{5.f});
    }

    void PowerUpShootingSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::PlayerPowerUp, InputComponent, Ecs::Position>(
            [&](const Ecs::Entity playerEntity, Ecs::PlayerPowerUp &powerUp, const InputComponent &input,
                const Ecs::Position &pos) {
                const bool powerShootPressed = input.powerShoot && !powerUp.previousPowerShoot;
                powerUp.previousPowerShoot = input.powerShoot;

                if (!powerUp.hasPowerUp)
                    return;
                if (!powerUp.isReady) {
                    powerUp.cooldown += dt;
                    if (powerUp.cooldown >= powerUp.maxCooldown) {
                        powerUp.isReady = true;
                        powerUp.cooldown = powerUp.maxCooldown;
                    }
                }

                if (powerUp.isReady && powerShootPressed) {
                    createPowerUpProjectile(world, pos.x + 40.f, pos.y, 400.f, 0.f, static_cast<size_t>(playerEntity));
                    powerUp.isReady = false;
                    powerUp.cooldown = 0.f;
                    if (powerUp.hasBar && powerUp.barEntity.has_value()) {
                        world.destroyEntity(powerUp.barEntity.value());
                        powerUp.hasBar = false;
                        powerUp.barEntity = std::nullopt;
                    }
                }
            });
    }
} // namespace Game
