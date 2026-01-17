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
            auto &reg = w->registry();

            const size_t targetIdx = event.target;
            auto &bubbleComp = reg.getComponents<Ecs::BubblePowerUp>().at(targetIdx);

            if (bubbleComp && bubbleComp->hitsRemaining > 0) {
                const auto &proj = reg.getComponents<Ecs::Projectile>().at(event.source);
                if (proj) {
                    bubbleComp->hitsRemaining--;

                    w->events().emit<DestroyEvent>(DestroyEvent{event.source, false});
                    return;
                }
            }

            auto &health = reg.getComponents<Ecs::Health>().at(event.target);
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
            if (proj) {
                if (const auto &ks = w->registry().getComponents<Ecs::KillScore>().at(event.target);
                    ks && ks->score > 0)
                    w->events().emit<UpdateScoreEvent>(UpdateScoreEvent{proj->shooter, ks->score});
            }

            if (const auto &playerPowerUp = w->registry().getComponents<Ecs::PlayerPowerUp>().at(event.target)) {
                if (playerPowerUp->powerUpEntity.has_value())
                    w->events().emit<DestroyEvent>(
                        DestroyEvent{static_cast<size_t>(playerPowerUp->powerUpEntity.value()), false});
                if (playerPowerUp->hasBar && playerPowerUp->barEntity.has_value())
                    w->events().emit<DestroyEvent>(
                        DestroyEvent{static_cast<size_t>(playerPowerUp->barEntity.value()), false});
            }
            if (const auto &bubblePowerUp = w->registry().getComponents<Ecs::BubblePowerUp>().at(event.target)) {
                if (bubblePowerUp->bubbleEntity.has_value())
                    w->events().emit<DestroyEvent>(
                        DestroyEvent{static_cast<size_t>(bubblePowerUp->bubbleEntity.value()), false});
            }
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
            w->registry().emplaceComponent<Ecs::Collision>(
                proj, Ecs::Collision{event.bounds.first, event.bounds.second});
            w->registry().emplaceComponent<Ecs::Drawable>(proj, Ecs::Drawable{event.spriteId, true});
            if (event.health != 0 && event.maxHealth != 0)
                w->registry().emplaceComponent<Ecs::Health>(proj, Ecs::Health{event.health, event.maxHealth});
            w->registry().emplaceComponent<Ecs::Lifetime>(proj, Ecs::Lifetime{event.lifetime});
            w->registry().emplaceComponent<Ecs::Projectile>(proj, Ecs::Projectile{event.shooter});

            if (event.spriteId == 23) {
                std::vector<size_t> possibleTargets;
                auto &reg = w->registry();
                reg.view<Game::InputComponent, Ecs::Health, Ecs::Id>(
                    [&](const Ecs::Entity, const Game::InputComponent &, const Ecs::Health &hp, const Ecs::Id &id) {
                        if (hp.hp > 0)
                            possibleTargets.push_back(id.id);
                    });
                size_t targetId = 0;
                if (!possibleTargets.empty()) {
                    const size_t randomIndex = Rand::rng() % possibleTargets.size();
                    targetId = possibleTargets.at(randomIndex);
                }
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

    void registerPowerUpBarEvents(Game::IGameWorld &world)
    {
        auto *w = &world;

        world.events().subscribe<PowerUpBarCreateEvent>([w](const PowerUpBarCreateEvent &event) {
            auto &reg = w->registry();
            auto &playerPowerUp = reg.getComponents<Ecs::PlayerPowerUp>().at(event.playerId);

            if (!playerPowerUp || playerPowerUp->hasBar)
                return;

            const float yOffset = (static_cast<float>(event.playerIndex) * 40.f);
            const Ecs::Entity barEntity = w->createEntity();
            reg.emplaceComponent<Ecs::Position>(barEntity, Ecs::Position{10.f, yOffset});
            reg.emplaceComponent<Ecs::Velocity>(barEntity, Ecs::Velocity{0.f, 0.f});
            reg.emplaceComponent<Ecs::Drawable>(barEntity, Ecs::Drawable{16, true});
            reg.emplaceComponent<Ecs::Id>(barEntity, Ecs::Id{static_cast<uint32_t>(static_cast<size_t>(barEntity))});
            playerPowerUp->hasBar = true;
            playerPowerUp->barEntity = barEntity;
        });

        world.events().subscribe<PowerUpBarDestroyEvent>([w](const PowerUpBarDestroyEvent &event) {
            auto &reg = w->registry();
            auto &playerPowerUp = reg.getComponents<Ecs::PlayerPowerUp>().at(event.playerId);

            if (!playerPowerUp || !playerPowerUp->hasBar || !playerPowerUp->barEntity.has_value())
                return;

            w->events().emit<DestroyEvent>(DestroyEvent{static_cast<size_t>(playerPowerUp->barEntity.value()), false});
            playerPowerUp->hasBar = false;
            playerPowerUp->barEntity = std::nullopt;
        });
    }

    void registerBubblePowerUpEvents(Game::IGameWorld &world)
    {
        auto *w = &world;

        world.events().subscribe<BubblePowerUpCreateEvent>([w](const BubblePowerUpCreateEvent &event) {
            auto &reg = w->registry();
            auto &bubblePowerUp = reg.getComponents<Ecs::BubblePowerUp>().at(event.playerId);

            if (!bubblePowerUp || bubblePowerUp->bubbleEntity.has_value())
                return;

            const Ecs::Entity bubbleEnt = w->createEntity();
            reg.emplaceComponent<Ecs::Position>(bubbleEnt, Ecs::Position{event.playerX, event.playerY});
            reg.emplaceComponent<Ecs::Drawable>(bubbleEnt, Ecs::Drawable{20u, true});
            bubblePowerUp->bubbleEntity = bubbleEnt;
            bubblePowerUp->hitsRemaining = Ecs::BubblePowerUp::maxHits;
        });

        world.events().subscribe<BubblePowerUpUpdatePosEvent>([w](const BubblePowerUpUpdatePosEvent &event) {
            auto &reg = w->registry();
            const auto &bubblePowerUp = reg.getComponents<Ecs::BubblePowerUp>().at(event.playerId);

            if (!bubblePowerUp || !bubblePowerUp->bubbleEntity.has_value())
                return;

            const auto bubbleIdx = static_cast<size_t>(bubblePowerUp->bubbleEntity.value());
            if (auto &bubblePos = reg.getComponents<Ecs::Position>().at(bubbleIdx)) {
                bubblePos->x = event.playerX;
                bubblePos->y = event.playerY;
            }
        });

        world.events().subscribe<BubblePowerUpDestroyEvent>([w](const BubblePowerUpDestroyEvent &event) {
            auto &reg = w->registry();
            auto &bubblePowerUp = reg.getComponents<Ecs::BubblePowerUp>().at(event.playerId);

            if (!bubblePowerUp || !bubblePowerUp->bubbleEntity.has_value())
                return;

            w->events().emit<DestroyEvent>(
                DestroyEvent{static_cast<size_t>(bubblePowerUp->bubbleEntity.value()), false});
            bubblePowerUp->bubbleEntity = std::nullopt;
            bubblePowerUp->hitsRemaining = 0;
            reg.getComponents<Ecs::BubblePowerUp>().remove(event.playerId);
        });
    }

    void activateLaserPowerUp(Game::IGameWorld *w, const size_t powerUpIdx, const size_t playerIdx)
    {
        auto &reg = w->registry();
        w->events().emit<DestroyEvent>(DestroyEvent{powerUpIdx, false});

        auto &laserPowerUp = reg.getComponents<Ecs::LaserPowerUp>().at(playerIdx);
        if (laserPowerUp) {
            laserPowerUp->duration = 0.f;
        } else {
            reg.emplaceComponent<Ecs::LaserPowerUp>(static_cast<Ecs::Entity>(playerIdx));
        }
    }

    void activateShieldPowerUp(Game::IGameWorld *w, const size_t powerUpIdx, const size_t playerIdx)
    {
        auto &reg = w->registry();
        w->events().emit<DestroyEvent>(DestroyEvent{powerUpIdx, false});

        auto &bubblePowerUp = reg.getComponents<Ecs::BubblePowerUp>().at(playerIdx);
        if (bubblePowerUp) {
            bubblePowerUp->hitsRemaining = Ecs::BubblePowerUp::maxHits;
        } else {
            reg.emplaceComponent<Ecs::BubblePowerUp>(static_cast<Ecs::Entity>(playerIdx));
        }
    }

    void attachStandardPowerUp(Game::IGameWorld *w, const size_t powerUpIdx, const size_t playerIdx)
    {
        auto &reg = w->registry();
        auto &playerPowerUp = reg.getComponents<Ecs::PlayerPowerUp>().at(playerIdx);

        if (playerPowerUp)
            return;

        reg.getComponents<Ecs::PowerUp>().remove(powerUpIdx);

        if (auto &drawable = reg.getComponents<Ecs::Drawable>().at(powerUpIdx))
            drawable->spriteId = 14;

        if (auto &vel = reg.getComponents<Ecs::Velocity>().at(powerUpIdx)) {
            vel->vx = 0.f;
            vel->vy = 0.f;
        }

        if (auto &pos = reg.getComponents<Ecs::Position>().at(powerUpIdx)) {
            const auto &playerPos = reg.getComponents<Ecs::Position>().at(playerIdx);
            if (playerPos) {
                pos->x = playerPos->x + 20.f;
                pos->y = playerPos->y - 10.f;
            }
        }

        reg.emplaceComponent<Ecs::PlayerPowerUp>(static_cast<Ecs::Entity>(playerIdx),
            Ecs::PlayerPowerUp{0.f, 5.f, false, false, std::nullopt, static_cast<Ecs::Entity>(powerUpIdx), false});
    }

    void registerPowerUpCollection(Game::IGameWorld &world)
    {
        auto *w = &world;

        world.events().subscribe<CollisionEvent>([w](const CollisionEvent &event) {
            auto &reg = w->registry();

            const auto &powerUpA = reg.getComponents<Ecs::PowerUp>().at(event.a);
            const auto &playerB = reg.getComponents<Game::InputComponent>().at(event.b);

            const auto &powerUpB = reg.getComponents<Ecs::PowerUp>().at(event.b);
            const auto &playerA = reg.getComponents<Game::InputComponent>().at(event.a);

            size_t powerUpIdx = 0;
            size_t playerIdx = 0;

            if (powerUpA && playerB) {
                powerUpIdx = event.a;
                playerIdx = event.b;
            } else if (powerUpB && playerA) {
                powerUpIdx = event.b;
                playerIdx = event.a;
            } else {
                return;
            }

            const auto &powerUpType = reg.getComponents<Ecs::PowerUpType>().at(powerUpIdx);
            if (powerUpType && powerUpType->type == Ecs::PowerUpTypeEnum::Standard) {
                if (const auto &playerPowerUp = reg.getComponents<Ecs::PlayerPowerUp>().at(playerIdx))
                    return;
            }

            if (powerUpType && powerUpType->type == Ecs::PowerUpTypeEnum::Laser) {
                activateLaserPowerUp(w, powerUpIdx, playerIdx);
            } else if (powerUpType && powerUpType->type == Ecs::PowerUpTypeEnum::Shield) {
                activateShieldPowerUp(w, powerUpIdx, playerIdx);
            } else {
                attachStandardPowerUp(w, powerUpIdx, playerIdx);
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
        registerPowerUpBarEvents(*this);
        registerBubblePowerUpEvents(*this);
        registerPowerUpCollection(*this);
    }

    Ecs::Registry &World::registry()
    {
        return _registry;
    }

    Ecs::EventsRegistry &World::events()
    {
        return _events;
    }

    Ecs::Entity World::createPlayer(const int sessionId, const size_t index)
    {
        static std::vector<unsigned int> playerSpriteId = {7, 8, 48, 10};

        const Ecs::Entity ent = World::createEntity();

        _registry.emplaceComponent<Ecs::Position>(ent, Ecs::Position{100.f, Rand::enemyY(Rand::rng), 2});
        _registry.emplaceComponent<Ecs::Velocity>(ent, Ecs::Velocity{0.f, 0.f});
        _registry.emplaceComponent<Ecs::Health>(ent, Ecs::Health{500, 500});
        _registry.emplaceComponent<InputComponent>(ent);
        _registry.emplaceComponent<Ecs::Drawable>(ent, Ecs::Drawable(playerSpriteId.at(index % 4), true));
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
