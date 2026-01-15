/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUpShootingSystem
*/

#include "PowerUpShootingSystem.hpp"

namespace Game
{
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
                    world.events().emit<ShootEvent>(ShootEvent{pos.x + 40.f, pos.y, 400.f, 0.f, 150,
                        static_cast<size_t>(playerEntity), {48.f, 24.f}, 5.f, 21, 1, 1});
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
