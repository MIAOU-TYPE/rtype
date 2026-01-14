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
                if (!bubble.isActive)
                    return;

                if (!bubble.bubbleEntity.has_value()) {
                    const Ecs::Entity bubbleEnt = world.createEntity();

                    reg.emplaceComponent<Ecs::Position>(bubbleEnt, Ecs::Position{playerPos.x, playerPos.y});
                    reg.emplaceComponent<Ecs::Drawable>(bubbleEnt, Ecs::Drawable{20u, true});

                    bubble.bubbleEntity = bubbleEnt;
                    bubble.hitsRemaining = Ecs::BubblePowerUp::maxHits;
                }

                if (bubble.bubbleEntity.has_value()) {
                    const size_t bubbleIdx = static_cast<size_t>(bubble.bubbleEntity.value());
                    if (auto &bubblePos = reg.getComponents<Ecs::Position>().at(bubbleIdx)) {
                        bubblePos->x = playerPos.x;
                        bubblePos->y = playerPos.y;
                    }
                }

                if (bubble.hitsRemaining <= 0 && bubble.bubbleEntity.has_value()) {
                    world.destroyEntity(bubble.bubbleEntity.value());
                    bubble.bubbleEntity = std::nullopt;
                    bubble.isActive = false;
                    bubble.hitsRemaining = 0;
                }
            });
    }
} // namespace Game
