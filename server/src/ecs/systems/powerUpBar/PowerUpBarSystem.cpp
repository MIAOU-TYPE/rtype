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
            [&](const Ecs::Entity, Ecs::PlayerPowerUp &powerUp, const Ecs::Position &) {
                if (powerUp.hasPowerUp && !powerUp.hasBar) {
                    const float yOffset = (static_cast<float>(playerIndex) * 40.f);
                    const Ecs::Entity barEntity = world.createEntity();
                    reg.emplaceComponent<Ecs::Position>(barEntity, Ecs::Position{10.f, yOffset});
                    reg.emplaceComponent<Ecs::Velocity>(barEntity, Ecs::Velocity{0.f, 0.f});
                    reg.emplaceComponent<Ecs::Drawable>(barEntity, Ecs::Drawable{16, true});
                    reg.emplaceComponent<Ecs::Id>(
                        barEntity, Ecs::Id{static_cast<uint32_t>(static_cast<size_t>(barEntity))});
                    powerUp.hasBar = true;
                    powerUp.barEntity = barEntity;
                } else if (!powerUp.hasPowerUp && powerUp.hasBar && powerUp.barEntity.has_value()) {
                    world.destroyEntity(powerUp.barEntity.value());
                    powerUp.hasBar = false;
                    powerUp.barEntity = std::nullopt;
                }
                playerIndex++;
            });
    }
} // namespace Game
