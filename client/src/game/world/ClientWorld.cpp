/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#include "ClientWorld.hpp"

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
            case WorldCommand::Type::Snapshot: applySnapshot(std::get<std::vector<SnapshotEntity>>(cmd.payload)); break;
            case WorldCommand::Type::Damage: applyDamage(std::get<DamageInfo>(cmd.payload)); break;
            case WorldCommand::Type::Destroy: {
                const auto &destroyInfo = std::get<DestroyInfo>(cmd.payload);
                applyDestroy(destroyInfo.entityId, destroyInfo.wasKilled);
                break;
            }
            default: break;
        }
    }

    void ClientWorld::applySnapshot(const std::vector<SnapshotEntity> &entities)
    {
        static constexpr auto maxTime = std::chrono::milliseconds(500);
        std::vector<size_t> toDestroy;
        std::unordered_set<size_t> receivedIds;

        const auto time = std::chrono::steady_clock::now();
        for (const auto &entity : entities) {
            _entityLastSeen[entity.id] = time;
            receivedIds.insert(entity.id);
            applySingleSnapshot(entity);
        }

        for (auto it = _entityMap.begin(); it != _entityMap.end();) {
            if (!receivedIds.contains(it->first)) {
                _registry.destroyEntity(it->second);
                it = _entityMap.erase(it);
            } else {
                ++it;
            }
        }
        for (const auto &[id, lastSeen] : _entityLastSeen) {
            if ((time - lastSeen) > maxTime)
                toDestroy.push_back(id);
        }

        for (auto id : toDestroy) {
            applyDestroy(id, false);
            _entityLastSeen.erase(id);
        }
    }

    void ClientWorld::applyDestroy(const size_t entityId, const bool wasKilled)
    {
        const auto it = _entityMap.find(entityId);
        if (it == _entityMap.end())
            return;

        if (wasKilled && _soundRegistry) {
            const auto entityIndex = static_cast<size_t>(it->second);
            if (auto &drawable = _registry.getComponents<Ecs::Drawable>().at(entityIndex)) {
                if (_spriteRegistry->exists(drawable->spriteId)) {
                    const auto &sprite = _spriteRegistry->get(drawable->spriteId);
                    if (sprite.destroySoundHandle != Graphics::InvalidAudio)
                        _soundRegistry->playSound(sprite.destroySoundHandle);
                }
            }
        }

        _registry.destroyEntity(it->second);
        _entityMap.erase(it);
        _entityLastSeen.erase(entityId);
    }

    void ClientWorld::applyDamage(const DamageInfo &damageInfo)
    {
        const auto it = _entityMap.find(damageInfo.targetId);
        if (it == _entityMap.end())
            return;

        if (!damageInfo.wasKilled && _soundRegistry) {
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

    void ClientWorld::applyCreate(const EntityCreate &data)
    {
        try {
            if (!_spriteRegistry->exists(data.spriteId))
                return;

            const Ecs::Entity entity = _registry.createEntity();
            _entityMap.emplace(data.id, entity);

            _registry.emplaceComponent<Ecs::Position>(entity, Ecs::Position{data.x, data.y});
            _registry.emplaceComponent<Ecs::Drawable>(entity, Ecs::Drawable{data.spriteId});

            const auto &sprite = _spriteRegistry->get(data.spriteId);

            _registry.emplaceComponent<Ecs::Render>(entity, Ecs::Render{sprite.textureHandle});
            _registry.emplaceComponent<Ecs::AnimationState>(entity,
                Ecs::AnimationState{.currentAnimation = sprite.defaultAnimation, .frameIndex = 0, .elapsed = 0.f});

            if (data.spriteId == 6 && _soundRegistry && sprite.shootSoundHandle != Graphics::InvalidAudio)
                _soundRegistry->playSound(sprite.shootSoundHandle);
        } catch (const std::exception &e) {
            std::cerr << "{ClientWorld::applyCreate} " << e.what() << std::endl;
        }
    }

    void ClientWorld::applySingleSnapshot(const SnapshotEntity &entity)
    {
        const auto it = _entityMap.find(entity.id);
        if (it == _entityMap.end()) {
            applyCreate(EntityCreate{entity.id, entity.x, entity.y, entity.spriteId});
            return;
        }

        const Ecs::Entity localEntity = it->second;
        const auto entityIndex = static_cast<size_t>(localEntity);

        if (auto &pos = _registry.getComponents<Ecs::Position>().at(entityIndex)) {
            pos->x = entity.x;
            pos->y = entity.y;
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
} // namespace World
