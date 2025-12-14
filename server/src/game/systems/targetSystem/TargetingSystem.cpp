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

        std::optional<size_t> playerId = std::nullopt;
        for (size_t i = 0; i < ctrlArr.size(); ++i)
            if (ctrlArr[i].has_value())
                playerId = i;
        if (!playerId.has_value())
            return;

        const Ecs::Position playerPos = *posArr[*playerId];
        reg.view<Ecs::Target, Ecs::Position>([&](Ecs::Entity, Ecs::Target &tgt, const Ecs::Position &pos) {
            const float dx = playerPos.x - pos.x;
            const float dy = playerPos.y - pos.y;
            const float dist = std::sqrt(dx * dx + dy * dy);

            if (dist <= tgt.visionRange)
                tgt.targetId = *playerId;
            else
                tgt.targetId = SIZE_MAX;
        });
    }
} // namespace Game
