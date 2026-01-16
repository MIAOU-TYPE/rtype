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
                if (!powerUp.hasBar) {
                    world.events().emit<PowerUpBarCreateEvent>(
                        PowerUpBarCreateEvent{static_cast<size_t>(playerEntity), playerIndex});
                }
                playerIndex++;
            });
    }
} // namespace Game
