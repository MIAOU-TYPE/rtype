/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SnapshotSystem
*/

#include "SnapshotSystem.hpp"

namespace Game
{
    void SnapshotSystem::update(IGameWorld &world, std::vector<SnapshotEntity> &snapshot)
    {
        auto &reg = world.registry();
        snapshot.clear();

        reg.view<Ecs::Drawable, Ecs::Position>(
            [&](const Ecs::Entity &entity, const Ecs::Drawable &draw, const Ecs::Position &pos) {
                SnapshotEntity s{};
                s.entity = static_cast<size_t>(entity);
                s.x = pos.x;
                s.y = pos.y;
                s.sprite = draw.sprite;

                snapshot.push_back(s);
            });
    }
} // namespace Game
