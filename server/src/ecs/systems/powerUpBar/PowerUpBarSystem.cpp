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

        std::vector<std::pair<Ecs::Entity, Ecs::PlayerPowerUp *>> playersWithPowerUp;
        reg.view<Ecs::PlayerPowerUp>([&](const Ecs::Entity playerEntity, Ecs::PlayerPowerUp &powerUp) {
            playersWithPowerUp.emplace_back(playerEntity, &powerUp);
        });

        std::sort(playersWithPowerUp.begin(), playersWithPowerUp.end(), [](const auto &a, const auto &b) {
            return static_cast<size_t>(a.first) < static_cast<size_t>(b.first);
        });

        int playerIndex = 0;
        for (auto &[playerEntity, powerUp] : playersWithPowerUp) {
            if (!powerUp->hasBar) {
                world.events().emit<PowerUpBarCreateEvent>(
                    PowerUpBarCreateEvent{static_cast<size_t>(playerEntity), playerIndex});
            }
            playerIndex++;
        }
    }
} // namespace Game
