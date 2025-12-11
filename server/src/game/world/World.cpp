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

    Ecs::Registry &World::registry()
    {
        return _registry;
    }
} // namespace Game
