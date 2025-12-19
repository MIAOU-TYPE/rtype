/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TargetingSystem
*/

#include "TargetingSystem.hpp"

namespace Game
{
    void TargetingSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();
        auto &posArr = reg.getComponents<Ecs::Position>();
        auto &ctrlArr = reg.getComponents<Ecs::Controllable>();

        std::vector<size_t> players;
        for (size_t i = 0; i < ctrlArr.size(); ++i)
            if (ctrlArr[i].has_value())
                players.push_back(i);
        if (players.empty())
            return;

        const size_t playerId = players[Rand::rng() % players.size()];
        const Ecs::Position &playerPos = *posArr[playerId];
        reg.view<Ecs::Target, Ecs::Position>([&](Ecs::Entity, Ecs::Target &tgt, const Ecs::Position &pos) {
            const float dx = playerPos.x - pos.x;
            const float dy = playerPos.y - pos.y;
            const float distSq = dx * dx + dy * dy;

            if (distSq <= tgt.visionRange * tgt.visionRange)
                tgt.targetId = playerId;
            else
                tgt.targetId = SIZE_MAX;
        });
    }
} // namespace Game
