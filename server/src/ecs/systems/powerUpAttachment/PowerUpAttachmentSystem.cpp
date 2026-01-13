/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUpAttachmentSystem
*/

#include "PowerUpAttachmentSystem.hpp"

namespace Game
{
    void PowerUpAttachmentSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        reg.view<Ecs::PlayerPowerUp, Ecs::Position>(
            [&](const Ecs::Entity, const Ecs::PlayerPowerUp &playerPowerUp, const Ecs::Position &playerPos) {
                if (!playerPowerUp.hasPowerUp)
                    return;

                reg.view<Ecs::PowerUp, Ecs::Position>(
                    [&](const Ecs::Entity, const Ecs::PowerUp &powerUp, Ecs::Position &powerUpPos) {
                        if (powerUp.collected) {
                            powerUpPos.x = playerPos.x + 30.f;
                            powerUpPos.y = playerPos.y;
                        }
                    });
            });
    }
} // namespace Game
