/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BubblePowerUpSystem
*/

#include "BubblePowerUpSystem.hpp"

namespace Game
{
    void BubblePowerUpSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        reg.view<Ecs::BubblePowerUp, Ecs::Position>(
            [&](const Ecs::Entity playerEntity, Ecs::BubblePowerUp &bubble, const Ecs::Position &playerPos) {
                if (!bubble.bubbleEntity.has_value()) {
                    world.events().emit<BubblePowerUpCreateEvent>(
                        BubblePowerUpCreateEvent{static_cast<size_t>(playerEntity), playerPos.x, playerPos.y});
                    return;
                }

                if (bubble.hitsRemaining <= 0 && bubble.bubbleEntity.has_value()) {
                    world.events().emit<BubblePowerUpDestroyEvent>(
                        BubblePowerUpDestroyEvent{static_cast<size_t>(playerEntity)});
                    return;
                }

                world.events().emit<BubblePowerUpUpdatePosEvent>(
                    BubblePowerUpUpdatePosEvent{static_cast<size_t>(playerEntity), playerPos.x, playerPos.y});
            });
    }
} // namespace Game
