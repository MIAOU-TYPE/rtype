/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SnapshotSystem
*/

#include "SnapshotSystem.hpp"

namespace Game
{
    void SnapshotSystem::update(IGameWorld &world, std::vector<SnapshotEntity> &out)
    {
        auto &reg = world.registry();
        out.clear();

        reg.view<Ecs::Drawable, Ecs::Position, Ecs::Id>(
            [&](const Ecs::Entity &, const Ecs::Drawable &draw, const Ecs::Position &pos, const Ecs::Id id) {
                SnapshotEntity s{};
                s.id = id.id;
                s.x = pos.x;
                s.y = pos.y;
                s.spriteId = draw.spriteId;

                out.push_back(s);
            });
    }
} // namespace Game
