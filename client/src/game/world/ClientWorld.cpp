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

    [[nodiscard]] uint32_t computeTargetTick(const uint32_t latestTick) noexcept
    {
        return (latestTick > World::ClientWorld::InterpDelayTicks) ? (latestTick - World::ClientWorld::InterpDelayTicks)
                                                                   : 0u;
    }
} // namespace

namespace World
{
    ClientWorld::ClientWorld(std::shared_ptr<const Engine::SpriteRegistry> spriteRegistry)
        : _spriteRegistry(std::move(spriteRegistry))
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
            case WorldCommand::Type::Snapshot: applySnapshot(std::get<World::SnapshotBatch>(cmd.payload)); break;
            case WorldCommand::Type::Destroy: applyDestroy(std::get<size_t>(cmd.payload)); break;
            case WorldCommand::Type::Score: _score = std::get<uint32_t>(cmd.payload); break;
            default: break;
        }
    }

    uint32_t ClientWorld::getScore() const
    {
        return _score;
    }

    void ClientWorld::applySnapshot(const SnapshotBatch &batch)
    {
        TickSnapshot snap;
        snap.tick = batch.serverTick;

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

    void ClientWorld::applyDestroy(const size_t entityId)
    {
        _destroyed.insert(static_cast<uint32_t>(entityId));

        for (auto &[tick, entities] : _snapshots)
            entities.erase(entityId);

        const auto it = _entityMap.find(entityId);
        if (it == _entityMap.end())
            return;

        _registry.destroyEntity(it->second);
        _entityMap.erase(it);
        _entityLastSeen.erase(entityId);
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
        } catch (const std::exception &e) {
            std::cerr << "{ClientWorld::applyCreate} " << e.what() << std::endl;
        }
    }

    void ClientWorld::applySingleSnapshot(const SnapshotEntity &entity)
    {
        const auto it = _entityMap.find(entity.id);
        if (it == _entityMap.end()) {
            applyCreate(EntityCreate{entity.id, entity.x, entity.y, entity.z, entity.spriteId});
            return;
        }

        const Ecs::Entity localEntity = it->second;
        const auto entityIndex = static_cast<size_t>(localEntity);

        if (auto &pos = _registry.getComponents<Ecs::Position>().at(entityIndex)) {
            pos->x = entity.x;
            pos->y = entity.y;
            pos->z = entity.z;
        }

        if (auto &drawable = _registry.getComponents<Ecs::Drawable>().at(entityIndex)) {
            if (const bool spriteChanged = (drawable->spriteId != entity.spriteId);
                spriteChanged && _spriteRegistry->exists(entity.spriteId)) {
                drawable->spriteId = entity.spriteId;
                const auto &sprite = _spriteRegistry->get(drawable->spriteId);

                if (auto &render = _registry.getComponents<Ecs::Render>().at(entityIndex)) {
                    render->texture = sprite.textureHandle;

                    if (auto &animState = _registry.getComponents<Ecs::AnimationState>().at(entityIndex)) {
                        animState->currentAnimation = sprite.defaultAnimation;
                        animState->frameIndex = 0;
                        animState->elapsed = 0.f;
                    }
                }
            }
        }
    }

    void ClientWorld::refreshSpriteIfChanged(const Ecs::Entity e, const uint32_t spriteId,
        Ecs::SparseArray<Ecs::Drawable> &drawables, Ecs::SparseArray<Ecs::AnimationState> &anims,
        Ecs::SparseArray<Ecs::Render> &renders)
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

    void ClientWorld::updateInterpolatedPositions()
    {
        if (_snapshots.empty())
            return;

        auto &positions = _registry.getComponents<Ecs::Position>();
        auto &drawables = _registry.getComponents<Ecs::Drawable>();
        auto &renders = _registry.getComponents<Ecs::Render>();
        auto &anims = _registry.getComponents<Ecs::AnimationState>();

        const auto isDestroyed = [&](const size_t netId) {
            return _destroyed.contains(static_cast<uint32_t>(netId));
        };

        const auto ensureEntity = [&](const size_t netId, const NetState &st) -> Ecs::Entity {
            if (!_entityMap.contains(netId))
                applyCreate(EntityCreate{netId, st.x, st.y, st.z, st.spriteId});
            return _entityMap[netId];
        };

        const auto setPosition = [&](const Ecs::Entity e, const float x, const float y, const uint8_t z) {
            const auto idx = static_cast<size_t>(e);
            if (auto &pos = positions.at(idx)) {
                pos->x = x;
                pos->y = y;
                pos->z = z;
            }
        };

        const uint32_t latestTick = _snapshots.back().tick;
        const uint32_t targetTick = computeTargetTick(latestTick);

        const auto applyNoInterp = [&] {
            for (const auto &[tick, entities] = _snapshots.front(); const auto &[netId, st] : entities) {
                if (isDestroyed(netId))
                    continue;

                const Ecs::Entity e = ensureEntity(netId, st);
                setPosition(e, st.x, st.y, st.z);
                refreshSpriteIfChanged(e, st.spriteId, drawables, anims, renders);
            }
        };

        const auto trimOldSnapshots = [&] {
            while (_snapshots.size() >= 2 && _snapshots.at(1).tick <= targetTick)
                _snapshots.pop_front();
        };

        if (_snapshots.size() == 1 || targetTick < _snapshots.front().tick)
            return applyNoInterp();

        trimOldSnapshots();
        if (_snapshots.size() < 2)
            return;

        const auto &A = _snapshots.at(0);
        const auto &B = _snapshots.at(1);

        const uint32_t dt = B.tick - A.tick;
        if (dt == 0)
            return;

        const float alpha = clamp(static_cast<float>(targetTick - A.tick) / static_cast<float>(dt));

        for (const auto &[netId, bs] : B.entities) {
            if (isDestroyed(netId))
                continue;

            const auto itA = A.entities.find(netId);
            const NetState as = (itA != A.entities.end()) ? itA->second : bs;

            const Ecs::Entity e = ensureEntity(netId, bs);

            const float x = lerp(as.x, bs.x, alpha);
            const float y = lerp(as.y, bs.y, alpha);

            setPosition(e, x, y, bs.z);
            refreshSpriteIfChanged(e, bs.spriteId, drawables, anims, renders);
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
            applyDestroy(id);
    }

} // namespace World
