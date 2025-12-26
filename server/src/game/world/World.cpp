/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** World
*/

#include "World.hpp"

namespace
{
    void registerCollisionDamage(Game::IGameWorld &world)
    {
        world.events().subscribe<CollisionEvent>([&](const CollisionEvent &event) {
            auto &reg = world.registry();

            auto &hpArr = reg.getComponents<Ecs::Health>();

            const auto &dmgA = reg.getComponents<Ecs::Damage>().at(event.a);
            if (const auto &hpB = hpArr.at(event.b); dmgA && hpB) {
                world.events().emit(DamageEvent{event.a, event.b, dmgA->amount});
            }

            const auto &dmgB = reg.getComponents<Ecs::Damage>().at(event.b);
            if (const auto &hpA = hpArr.at(event.a); dmgB && hpA) {
                world.events().emit(DamageEvent{event.b, event.a, dmgB->amount});
            }
        });
    }

    void registerDamageToScoreEvent(Game::IGameWorld &world)
    {
        world.events().subscribe<DamageEvent>([&](const DamageEvent &event) {
            auto &health = world.registry().getComponents<Ecs::Health>().at(event.target);
            if (!health || health->hp <= 0)
                return;
            if (health->hp <= event.amount)
                health->hp = 0;
            else
                health->hp -= event.amount;

            if (health->hp > 0)
                return;
            const auto &proj = world.registry().getComponents<Ecs::Projectile>().at(event.source);
            if (!proj)
                return;
            if (const auto &ks = world.registry().getComponents<Ecs::KillScore>().at(event.target); ks && ks->score > 0)
                world.events().emit<UpdateScoreEvent>(UpdateScoreEvent{proj->shooter, ks->score});
        });
    }

    void registerProjectileSpawning(Game::IGameWorld &world)
    {
        world.events().subscribe<ShootEvent>([&](const ShootEvent &event) {
            const Ecs::Entity proj = world.registry().createEntity();
            world.registry().emplaceComponent<Ecs::Position>(proj, Ecs::Position{event.x, event.y});
            world.registry().emplaceComponent<Ecs::Velocity>(proj, Ecs::Velocity{event.vx, event.vy});
            world.registry().emplaceComponent<Ecs::Damage>(proj, Ecs::Damage{event.damage});
            world.registry().emplaceComponent<Ecs::Damageable>(proj);
            world.registry().emplaceComponent<Ecs::Collision>(proj, Ecs::Collision{8.f, 8.f});
            world.registry().emplaceComponent<Ecs::Drawable>(proj, Ecs::Drawable{6, true});
            world.registry().emplaceComponent<Ecs::Health>(proj, Ecs::Health{1, 1});
            world.registry().emplaceComponent<Ecs::Lifetime>(proj, Ecs::Lifetime{event.lifetime});
            world.registry().emplaceComponent<Ecs::Projectile>(proj, Ecs::Projectile{event.shooter});
        });
    }

    void registerDestroyEvent(Game::IGameWorld &world)
    {
        world.events().subscribe<DestroyEvent>([&](const DestroyEvent &event) {
            world.destroyEntity(Ecs::Entity(event.entityId));
        });
    }

    void registerDamageToScore(Game::IGameWorld &world)
    {
        world.events().subscribe<UpdateScoreEvent>([&](const UpdateScoreEvent &event) {
            auto &reg = world.registry();
            auto &scoreArr = reg.getComponents<Ecs::Score>();
            if (auto &scoreComp = scoreArr.at(event.playerId))
                scoreComp->score += event.scoreDelta;
        });
    }
} // namespace

namespace Game
{
    World::World()
    {
        registerCollisionDamage(*this);
        registerDamageToScoreEvent(*this);
        registerProjectileSpawning(*this);
        registerDestroyEvent(*this);
        registerDamageToScore(*this);
    }

    Ecs::Registry &World::registry()
    {
        return _registry;
    }

    Ecs::EventsRegistry &World::events()
    {
        return _events;
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
        _registry.emplaceComponent<Ecs::Score>(ent, Ecs::Score{0, 0});
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
            [&](const Ecs::Entity e, const Ecs::Position &, const Ecs::Velocity &, const Ecs::Drawable &) {
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
