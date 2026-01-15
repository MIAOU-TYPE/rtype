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
                    world.events().emit<BubblePowerUpEvent>(BubblePowerUpEvent{
                        static_cast<size_t>(playerEntity), playerPos.x, playerPos.y, true, false, false});
                    return;
                }

                if (bubble.hitsRemaining <= 0 && bubble.bubbleEntity.has_value()) {
                    world.events().emit<BubblePowerUpEvent>(
                        BubblePowerUpEvent{static_cast<size_t>(playerEntity), 0.f, 0.f, false, false, true});
                    return;
                }

                world.events().emit<BubblePowerUpEvent>(BubblePowerUpEvent{
                    static_cast<size_t>(playerEntity), playerPos.x, playerPos.y, false, true, false});
            });
    }
} // namespace Game
