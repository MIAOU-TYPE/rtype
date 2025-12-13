/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** World
*/

#include "World.hpp"

namespace Game
{
    World::World()
    {
        _registry.registerComponent<Ecs::Position>();
        _registry.registerComponent<Ecs::Velocity>();
        _registry.registerComponent<Ecs::Health>();
        _registry.registerComponent<InputComponent>();
        _registry.registerComponent<Ecs::Drawable>();
    }

    Ecs::Registry &World::registry()
    {
        return _registry;
    }

    Ecs::Entity World::createPlayer()
    {
        const Ecs::Entity ent = _registry.createEntity();

        _registry.emplaceComponent<Ecs::Position>(ent, 100.f, 100.f);
        _registry.emplaceComponent<Ecs::Velocity>(ent, 0.f, 0.f);
        _registry.emplaceComponent<Ecs::Health>(ent, 100, 100);
        _registry.emplaceComponent<InputComponent>(ent);
        _registry.emplaceComponent<Ecs::Drawable>(ent, "player_sprite");
        return ent;
    }

    void World::destroyEntity(const Ecs::Entity ent)
    {
        _registry.destroyEntity(ent);
    }

    void World::copyFrom(IGameWorld &other)
    {
        auto &src = other.registry();
        auto &dst = this->registry();

        dst.clear();
        src.view<Ecs::Position, Ecs::Velocity, Ecs::Drawable>(
            [&](Ecs::Entity e, const Ecs::Position &p, const Ecs::Velocity &v, const Ecs::Drawable &d) {
                dst.emplaceComponent<Ecs::Position>(e, p);
                dst.emplaceComponent<Ecs::Velocity>(e, v);
                dst.emplaceComponent<Ecs::Drawable>(e, d);
            });
    }
} // namespace Game
