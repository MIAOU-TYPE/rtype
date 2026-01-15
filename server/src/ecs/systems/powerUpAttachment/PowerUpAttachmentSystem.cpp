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
                if (!playerPowerUp.hasPowerUp || !playerPowerUp.powerUpEntity.has_value())
                    return;

                const size_t powerUpIdx = static_cast<size_t>(playerPowerUp.powerUpEntity.value());
                auto &powerUpPos = reg.getComponents<Ecs::Position>().at(powerUpIdx);

                if (powerUpPos) {
                    powerUpPos->x = playerPos.x + 50.f;
                    powerUpPos->y = playerPos.y;
                }
            });
    }
} // namespace Game
