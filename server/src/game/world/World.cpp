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
        auto *w = &world;

        world.events().subscribe<CollisionEvent>([w](const CollisionEvent &event) {
            auto &reg = w->registry();

            auto &hpArr = reg.getComponents<Ecs::Health>();

            const auto &dmgA = reg.getComponents<Ecs::Damage>().at(event.a);
            if (const auto &hpB = hpArr.at(event.b); dmgA && hpB)
                w->events().emit(DamageEvent{event.a, event.b, dmgA->amount});

            const auto &dmgB = reg.getComponents<Ecs::Damage>().at(event.b);
            if (const auto &hpA = hpArr.at(event.a); dmgB && hpA)
                w->events().emit(DamageEvent{event.b, event.a, dmgB->amount});
        });
    }

    void registerDamageToScoreEvent(Game::IGameWorld &world)
    {
        auto *w = &world;

        world.events().subscribe<DamageEvent>([w](const DamageEvent &event) {
            auto &health = w->registry().getComponents<Ecs::Health>().at(event.target);
            if (!health || health->hp <= 0)
                return;
            if (health->hp <= event.amount)
                health->hp = 0;
            else
                health->hp -= event.amount;
            w->events().emit<DamageApplyEvent>(DamageApplyEvent{
                event.target, static_cast<uint32_t>(health->hp), static_cast<uint32_t>(health->maxHp)});

            const auto &proj = w->registry().getComponents<Ecs::Projectile>().at(event.source);
            if (proj) {
                if (auto &sourceHealth = w->registry().getComponents<Ecs::Health>().at(event.source)) {
                    sourceHealth->hp -= 1;
                    if (sourceHealth->hp <= 0)
                        w->events().emit<DestroyEvent>(DestroyEvent{event.source, false});
                }
            }

            if (health->hp > 0)
                return;
            if (const auto &ks = w->registry().getComponents<Ecs::KillScore>().at(event.target); ks && ks->score > 0)
                w->events().emit<UpdateScoreEvent>(UpdateScoreEvent{proj->shooter, ks->score});
        });
    }

    void registerProjectileSpawning(Game::IGameWorld &world)
    {
        auto *w = &world;

        world.events().subscribe<ShootEvent>([w](const ShootEvent &event) {
            const Ecs::Entity proj = w->createEntity();
            w->registry().emplaceComponent<Ecs::Position>(proj, Ecs::Position{event.x, event.y, 1});
            w->registry().emplaceComponent<Ecs::Velocity>(proj, Ecs::Velocity{event.vx, event.vy});
            w->registry().emplaceComponent<Ecs::Damage>(proj, Ecs::Damage{event.damage});
            w->registry().emplaceComponent<Ecs::Damageable>(proj);
            w->registry().emplaceComponent<Ecs::Collision>(proj, Ecs::Collision{8.f, 8.f});
            w->registry().emplaceComponent<Ecs::Drawable>(proj, Ecs::Drawable{event.spriteId, true});
            w->registry().emplaceComponent<Ecs::Health>(proj, Ecs::Health{1, 1});
            w->registry().emplaceComponent<Ecs::Lifetime>(proj, Ecs::Lifetime{event.lifetime});
            w->registry().emplaceComponent<Ecs::Projectile>(proj, Ecs::Projectile{event.shooter});

            if (event.spriteId == 29) {
                size_t targetId;
                auto &reg = w->registry();
                reg.view<Game::InputComponent, Ecs::Health, Ecs::Id>(
                    [&](const Ecs::Entity, const Game::InputComponent &, const Ecs::Health &hp, const Ecs::Id &id) {
                        if (hp.hp > 0)
                            targetId = id.id;
                    });
                if (targetId > 0)
                    w->registry().emplaceComponent<Ecs::HomingProjectile>(
                        proj, Ecs::HomingProjectile{targetId, 2.0f, 200.0f});
            }
        });
    }

    void registerDestroyEvent(Game::IGameWorld &world)
    {
        auto *w = &world;

        world.events().subscribe<DestroyEvent>([w](const DestroyEvent &event) {
            w->destroyEntity(Ecs::Entity(event.entityId));
        });
    }

    void registerDamageToScore(Game::IGameWorld &world)
    {
        auto *w = &world;

        world.events().subscribe<UpdateScoreEvent>([w](const UpdateScoreEvent &event) {
            auto &reg = w->registry();
            auto &scoreArr = reg.getComponents<Ecs::Score>();
            if (auto &scoreComp = scoreArr.at(event.playerId)) {
                scoreComp->score += event.scoreDelta;
                w->events().emit(ScoreUpdatedEvent{event.playerId, scoreComp->score});
            }
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

    Ecs::Entity World::createPlayer(const int sessionId)
    {
        const Ecs::Entity ent = World::createEntity();

        _registry.emplaceComponent<Ecs::Position>(ent, Ecs::Position{100.f, Rand::enemyY(Rand::rng), 2});
        _registry.emplaceComponent<Ecs::Velocity>(ent, Ecs::Velocity{0.f, 0.f});
        _registry.emplaceComponent<Ecs::Health>(ent, Ecs::Health{500, 500});
        _registry.emplaceComponent<InputComponent>(ent);
        _registry.emplaceComponent<Ecs::Drawable>(ent, Ecs::Drawable(7, true));
        _registry.emplaceComponent<Ecs::Collision>(ent, Ecs::Collision{51, 25.5f});
        _registry.emplaceComponent<Ecs::Damageable>(ent);
        _registry.emplaceComponent<Ecs::Score>(ent, Ecs::Score{0, 0});
        _registry.emplaceComponent<Ecs::WeaponConfig>(ent, Ecs::WeaponConfig{6});
        _registry.emplaceComponent<Ecs::GravityAffected>(ent, Ecs::GravityAffected{});
        if (const auto netId = _registry.getComponents<Ecs::Id>().at(static_cast<size_t>(ent)); netId)
            _events.emit<PlayerConnectedEvent>(PlayerConnectedEvent{sessionId, static_cast<size_t>(ent)});
        return ent;
    }

    void World::destroyEntity(const Ecs::Entity ent)
    {
        const auto it = _netToEntity.find(static_cast<size_t>(ent));
        if (it == _netToEntity.end())
            return;
        _registry.destroyEntity(it->second);
        _netToEntity.erase(it);
    }

    Ecs::Entity World::createEntity()
    {
        const Ecs::Entity ent = _registry.createEntity();
        const size_t id = _nextId++;
        _registry.emplaceComponent<Ecs::Id>(ent, Ecs::Id{id});
        _netToEntity[id] = ent;
        return ent;
    }

    void World::copyFrom(IGameWorld &other)
    {
        auto &src = other.registry();
        auto &dst = this->registry();
        dst.clear();
        _netToEntity.clear();
        _nextId = 1;
        std::unordered_map<size_t, Ecs::Entity> remap;
        src.view<Ecs::Id, Ecs::Position, Ecs::Drawable>(
            [&](Ecs::Entity, const Ecs::Id &id, const Ecs::Position &, const Ecs::Drawable &) {
                const Ecs::Entity newEnt = dst.createEntity();
                remap.emplace(id.id, newEnt);
            });
        src.view<Ecs::Id, Ecs::Position, Ecs::Drawable>(
            [&](Ecs::Entity, const Ecs::Id &nid, const Ecs::Position &p, const Ecs::Drawable &d) {
                const Ecs::Entity newEnt = remap.at(nid.id);
                dst.emplaceComponent<Ecs::Id>(newEnt, nid);
                dst.emplaceComponent<Ecs::Position>(newEnt, p);
                dst.emplaceComponent<Ecs::Drawable>(newEnt, d);
                _netToEntity[nid.id] = newEnt;
                if (nid.id >= _nextId)
                    _nextId = nid.id + 1;
            });
    }
} // namespace Game
