/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUpBarSystem
*/

#include "PowerUpBarSystem.hpp"

namespace Game
{
    void PowerUpBarSystem::update(IGameWorld &world, const float)
    {
        auto &reg = world.registry();

        int playerIndex = 0;
        reg.view<Ecs::PlayerPowerUp, Ecs::Position>(
            [&](const Ecs::Entity playerEntity, Ecs::PlayerPowerUp &powerUp, const Ecs::Position &) {
                if (powerUp.hasPowerUp && !powerUp.hasBar) {
                    world.events().emit<PowerUpBarEvent>(
                        PowerUpBarEvent{static_cast<size_t>(playerEntity), true, playerIndex});
                } else if (!powerUp.hasPowerUp && powerUp.hasBar && powerUp.barEntity.has_value()) {
                    world.events().emit<PowerUpBarEvent>(
                        PowerUpBarEvent{static_cast<size_t>(playerEntity), false, playerIndex});
                }
                playerIndex++;
            });
    }
} // namespace Game
