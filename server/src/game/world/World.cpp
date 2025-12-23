/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** World
*/

#include "World.hpp"

#include "Collision.hpp"
#include "Damage.hpp"
#include "Damageable.hpp"

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

        _registry.emplaceComponent<Ecs::Position>(ent, Ecs::Position{100.f, 100.f});
        _registry.emplaceComponent<Ecs::Velocity>(ent, Ecs::Velocity{0.f, 0.f});
        _registry.emplaceComponent<Ecs::Health>(ent, Ecs::Health{100, 100});
        _registry.emplaceComponent<InputComponent>(ent);
        _registry.emplaceComponent<Ecs::Drawable>(ent, Ecs::Drawable(7, true));
        _registry.emplaceComponent<Ecs::Collision>(ent, Ecs::Collision{30, 15});
        _registry.emplaceComponent<Ecs::Damageable>(ent);
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

        std::unordered_map<size_t, Ecs::Entity> remap;
        src.view<Ecs::Position, Ecs::Velocity, Ecs::Drawable>(
            [&](Ecs::Entity e, const Ecs::Position &, const Ecs::Velocity &, const Ecs::Drawable &) {
                const Ecs::Entity newEnt = dst.createEntity();
                remap[static_cast<size_t>(e)] = newEnt;
            });

        src.view<Ecs::Position, Ecs::Velocity, Ecs::Drawable>(
            [&](const Ecs::Entity e, const Ecs::Position &p, const Ecs::Velocity &v, const Ecs::Drawable &d) {
                const Ecs::Entity newEnt = remap[static_cast<size_t>(e)];
                dst.emplaceComponent<Ecs::Position>(newEnt, p);
                dst.emplaceComponent<Ecs::Velocity>(newEnt, v);
                dst.emplaceComponent<Ecs::Drawable>(newEnt, d);
            });
    }
} // namespace Game
