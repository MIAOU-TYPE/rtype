/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#include "ClientWorld.hpp"

namespace
{
    [[nodiscard]] float lerp(const float a, const float b, const float t)
    {
        return a + (b - a) * t;
    }

    [[nodiscard]] float clamp(const float v) noexcept
    {
        if (v < 0.f)
            return 0.f;
        if (v > 1.f)
            return 1.f;
        return v;
    }
} // namespace

namespace World
{
    ClientWorld::ClientWorld(std::shared_ptr<const Engine::SpriteRegistry> spriteRegistry,
        std::shared_ptr<Engine::SoundRegistry> soundRegistry)
        : _spriteRegistry(std::move(spriteRegistry)), _soundRegistry(std::move(soundRegistry))
    {
        _registry.registerComponent<Ecs::Position>();
        _registry.registerComponent<Ecs::Drawable>();
        _registry.registerComponent<Ecs::Render>();
        _registry.registerComponent<Ecs::AnimationState>();
    }

    void ClientWorld::step(const float dt)
    {
        Engine::AnimationSystem::update(_registry, _spriteRegistry, dt);
    }

    Ecs::Registry &ClientWorld::registry()
    {
        return _registry;
    }

    void ClientWorld::applyCommand(const WorldCommand &cmd)
    {
        switch (cmd.type) {
            case WorldCommand::Type::Snapshot: applySnapshot(std::get<SnapshotBatch>(cmd.payload)); break;
            case WorldCommand::Type::Damage: applyDamage(std::get<World::DamageInfo>(cmd.payload)); break;
            case WorldCommand::Type::Destroy: applyDestroy(std::get<World::DestroyInfo>(cmd.payload)); break;
            case WorldCommand::Type::Score: _score = std::get<uint32_t>(cmd.payload); break;
            case WorldCommand::Type::Accept: applyAccept(std::get<uint32_t>(cmd.payload)); break;
            default: break;
        }
    }

    uint32_t ClientWorld::getScore() const noexcept
    {
        return _score;
    }

    int ClientWorld::getEntityPlayerId() const noexcept
    {
        return _entityPlayerId;
    }

    void ClientWorld::applySnapshot(const SnapshotBatch &batch)
    {
        TickSnapshot snap;
        snap.tick = batch.serverTick;
        snap.arrivalTime = std::chrono::steady_clock::now();

        snap.entities.reserve(batch.entities.size());
        for (const auto &[id, x, y, z, spriteId] : batch.entities) {
            _destroyed.erase(static_cast<uint32_t>(id));
            snap.entities[id] = NetState{x, y, z, spriteId};
            _entityLastSeen[id] = std::chrono::steady_clock::now();
        }

        if (!_snapshots.empty() && snap.tick <= _snapshots.back().tick) {
            auto it = _snapshots.end();
            while (it != _snapshots.begin()) {
                const auto prev = std::prev(it);
                if (prev->tick <= snap.tick)
                    break;
                it = prev;
            }
            _snapshots.insert(it, std::move(snap));
        } else
            _snapshots.push_back(std::move(snap));

        while (_snapshots.size() > _maxSnapshots)
            _snapshots.pop_front();
        purgeStaleEntities(std::chrono::milliseconds(500));
    }

    void ClientWorld::applyDestroy(const DestroyInfo &destroyInfo)
    {
        _destroyed.insert(static_cast<uint32_t>(destroyInfo.entityId));

        for (auto &[tick, arrivalTime, entities] : _snapshots)
            entities.erase(destroyInfo.entityId);

        const auto it = _entityMap.find(destroyInfo.entityId);
        if (it == _entityMap.end())
            return;

        if (destroyInfo.wasKilled && _soundRegistry) {
            const auto entityIndex = static_cast<size_t>(it->second);
            if (const auto &drawable = _registry.getComponents<Ecs::Drawable>().at(entityIndex)) {
                if (_spriteRegistry->exists(drawable->spriteId)) {
                    if (const auto &sprite = _spriteRegistry->get(drawable->spriteId);
                        sprite.destroySoundHandle != Graphics::InvalidAudio)
                        _soundRegistry->playSound(sprite.destroySoundHandle);
                }
            }
        }

        _registry.destroyEntity(it->second);
        _entityMap.erase(it);
        _entityLastSeen.erase(destroyInfo.entityId);
    }

    void ClientWorld::applyDamage(const DamageInfo &damageInfo)
    {
        const auto it = _entityMap.find(damageInfo.targetId);
        if (it == _entityMap.end())
            return;

        if (_soundRegistry) {
            const auto entityIndex = static_cast<size_t>(it->second);
            if (auto &drawable = _registry.getComponents<Ecs::Drawable>().at(entityIndex)) {
                if (_spriteRegistry->exists(drawable->spriteId)) {
                    const auto &sprite = _spriteRegistry->get(drawable->spriteId);
                    if (sprite.hitSoundHandle != Graphics::InvalidAudio)
                        _soundRegistry->playSound(sprite.hitSoundHandle);
                }
            }
        }
    }

    void ClientWorld::applyAccept(const uint32_t &data)
    {
        _entityPlayerId = static_cast<int>(data);
    }

    void ClientWorld::applyCreate(const EntityCreate &data)
    {
        try {
            if (!_spriteRegistry->exists(data.spriteId)) {
                std::cerr << "{ClientWorld::applyCreate} Sprite ID " << data.spriteId << " not found in registry!"
                          << std::endl;
                return;
            }

            const Ecs::Entity entity = _registry.createEntity();
            _entityMap.emplace(data.id, entity);

            _registry.emplaceComponent<Ecs::Position>(entity, Ecs::Position{data.x, data.y, data.z});
            _registry.emplaceComponent<Ecs::Drawable>(entity, Ecs::Drawable{data.spriteId});

            const auto &sprite = _spriteRegistry->get(data.spriteId);

            if (sprite.textureHandle == Graphics::InvalidTexture) {
                std::cerr << "{ClientWorld::applyCreate} WARNING: Sprite " << data.spriteId
                          << " has invalid texture handle! Path: " << sprite.texturePath << std::endl;
            }

            _registry.emplaceComponent<Ecs::Render>(entity, Ecs::Render{sprite.textureHandle});
            _registry.emplaceComponent<Ecs::AnimationState>(entity,
                Ecs::AnimationState{.currentAnimation = sprite.defaultAnimation, .frameIndex = 0, .elapsed = 0.f});

            if (data.spriteId == 6 && _soundRegistry && sprite.shootSoundHandle != Graphics::InvalidAudio)
                _soundRegistry->playSound(sprite.shootSoundHandle);
        } catch (const std::exception &e) {
            std::cerr << "{ClientWorld::applyCreate} " << e.what() << std::endl;
        }
    }

    void ClientWorld::refreshSpriteIfChanged(const Ecs::Entity e, const uint32_t spriteId,
        Ecs::SparseArray<Ecs::Drawable> &drawables, Ecs::SparseArray<Ecs::AnimationState> &anims,
        Ecs::SparseArray<Ecs::Render> &renders) const
    {
        const auto idx = static_cast<size_t>(e);

        if (auto &drawable = drawables.at(idx)) {
            if (drawable->spriteId == spriteId)
                return;
            if (!_spriteRegistry->exists(spriteId))
                return;

            drawable->spriteId = spriteId;
            const auto &sprite = _spriteRegistry->get(spriteId);

            if (auto &render = renders.at(idx))
                render->texture = sprite.textureHandle;

            if (auto &anim = anims.at(idx)) {
                anim->currentAnimation = sprite.defaultAnimation;
                anim->frameIndex = 0;
                anim->elapsed = 0.f;
            }
        }
    }

    void ClientWorld::reconcileLocalPlayerWithServer(const NetState &bs, Ecs::SparseArray<Ecs::Position> &positions)
    {
        if (_entityPlayerId < 0)
            return;

        const auto itEnt = _entityMap.find(static_cast<uint32_t>(_entityPlayerId));
        if (itEnt == _entityMap.end())
            return;

        const auto ent = static_cast<size_t>(itEnt->second);

        if (ent >= positions.size())
            return;

        auto &posOpt = positions.at(ent);
        if (!posOpt)
            return;

        const float serverX = bs.x;
        const float serverY = bs.y;

        const float dx = serverX - posOpt->x;
        const float dy = serverY - posOpt->y;
        const float dist2 = dx * dx + dy * dy;

        constexpr float SnapDist = 250.f;

        if (constexpr float SnapDist2 = SnapDist * SnapDist; dist2 > SnapDist2) {
            const float dist = std::sqrt(dist2);

            const float factor = dist > 300.f ? 0.5f : dist > 150.f ? 0.35f : 0.2f;

            posOpt->x += dx * factor;
            posOpt->y += dy * factor;
            return;
        }

        constexpr float SmoothFactor = 0.15f;
        posOpt->x += dx * SmoothFactor;
        posOpt->y += dy * SmoothFactor;
    }

    void ClientWorld::updateInterpolatedPositions()
    {
        if (_snapshots.size() < 2)
            return;

        auto &positions = _registry.getComponents<Ecs::Position>();
        auto &drawables = _registry.getComponents<Ecs::Drawable>();
        auto &renders = _registry.getComponents<Ecs::Render>();
        auto &anims = _registry.getComponents<Ecs::AnimationState>();

        const auto now = std::chrono::steady_clock::now();
        constexpr auto InterpDelay = std::chrono::milliseconds(100);

        std::optional<size_t> idxA;
        std::optional<size_t> idxB;

        for (size_t i = 1; i < _snapshots.size(); ++i) {
            if (_snapshots.at(i).arrivalTime > now - InterpDelay) {
                idxA = i - 1;
                idxB = i;
                break;
            }
        }

        if (!idxA || !idxB)
            return;

        const TickSnapshot &A = _snapshots.at(*idxA);
        const TickSnapshot &B = _snapshots.at(*idxB);

        const float denom = std::chrono::duration<float>(B.arrivalTime - A.arrivalTime).count();
        if (denom <= 0.f)
            return;

        const float alpha = clamp(std::chrono::duration<float>(now - InterpDelay - A.arrivalTime).count() / denom);

        for (const auto &[netId, bs] : B.entities) {
            if (_destroyed.contains(static_cast<uint32_t>(netId)))
                continue;

            if (std::cmp_equal(netId, _entityPlayerId)) {
                if (!_entityMap.contains(netId))
                    applyCreate(EntityCreate{netId, bs.x, bs.y, bs.z, bs.spriteId});

                reconcileLocalPlayerWithServer(bs, positions);
                if (auto it = _entityMap.find(netId); it != _entityMap.end())
                    refreshSpriteIfChanged(it->second, bs.spriteId, drawables, anims, renders);
                continue;
            }

            if (!_entityMap.contains(netId))
                applyCreate(EntityCreate{netId, bs.x, bs.y, bs.z, bs.spriteId});

            const Ecs::Entity e = _entityMap[netId];
            const auto entIdx = static_cast<size_t>(e);
            auto &posOpt = positions.at(entIdx);
            if (!posOpt)
                continue;

            const auto itA = A.entities.find(netId);
            const NetState as = (itA != A.entities.end()) ? itA->second : bs;

            constexpr float MaxVisualStep = 20.f;

            const float targetX = lerp(as.x, bs.x, alpha);
            const float targetY = lerp(as.y, bs.y, alpha);

            const float dx = targetX - posOpt->x;
            const float dy = targetY - posOpt->y;

            posOpt->x += std::clamp(dx, -MaxVisualStep, MaxVisualStep);
            posOpt->y += std::clamp(dy, -MaxVisualStep, MaxVisualStep);
            posOpt->z = bs.z;

            refreshSpriteIfChanged(e, bs.spriteId, drawables, anims, renders);
        }

        while (_snapshots.size() > 2 && _snapshots.front().arrivalTime < A.arrivalTime) {
            _snapshots.pop_front();
        }
    }

    void ClientWorld::purgeStaleEntities(const std::chrono::milliseconds maxAge)
    {
        const auto now = std::chrono::steady_clock::now();

        std::vector<size_t> toDestroy;
        toDestroy.reserve(_entityLastSeen.size());

        for (const auto &[id, lastSeen] : _entityLastSeen) {
            if ((now - lastSeen) > maxAge)
                toDestroy.push_back(id);
        }

        for (const auto id : toDestroy)
            applyDestroy(DestroyInfo{id, false});
    }

    void ClientWorld::applyLocalMovementFromNetId(const uint8_t input) noexcept
    {
        float dx = 0.f;
        float dy = 0.f;

        if (input & 0x01)
            dx -= 1.f;
        if (input & 0x02)
            dx += 1.f;
        if (input & 0x04)
            dy += 1.f;
        if (input & 0x08)
            dy -= 1.f;

        const auto it = _entityMap.find(static_cast<size_t>(_entityPlayerId));
        if (it == _entityMap.end())
            return;

        const auto ent = it->second;
        auto &pos = _registry.getComponents<Ecs::Position>().at(static_cast<size_t>(ent));
        if (!pos)
            return;
        pos->x += dx * 5.f;
        pos->y += dy * 5.f;
    }
} // namespace World
