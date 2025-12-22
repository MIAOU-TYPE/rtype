/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#include "ClientWorld.hpp"

namespace Engine
{
    ClientWorld::ClientWorld(std::shared_ptr<const SpriteRegistry> spriteRegistry)
        : _spriteRegistry(std::move(spriteRegistry))
    {
        _registry.registerComponent<Position>();
        _registry.registerComponent<Drawable>();
        _registry.registerComponent<Render>();
        _registry.registerComponent<AnimationState>();
    }

    void ClientWorld::step(const float dt)
    {
        AnimationSystem::update(_registry, _spriteRegistry, dt);
    }

    Ecs::Registry &ClientWorld::registry()
    {
        return _registry;
    }

    void ClientWorld::applyCommand(const WorldCommand &cmd)

    {
        switch (cmd.type) {
            case WorldCommand::Type::Snapshot: applySnapshot(std::get<std::vector<SnapshotEntity>>(cmd.payload)); break;
            default: break;
        }
    }

    void ClientWorld::applySnapshot(const std::vector<SnapshotEntity> &entities)
    {
        std::unordered_set<size_t> receivedIds;
        receivedIds.reserve(entities.size());

        for (const auto &entity : entities) {
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
    }

    void ClientWorld::applyCreate(const EntityCreate &data)
    {
        try {
            if (!_spriteRegistry->exists(data.spriteId))
                return;

            const Ecs::Entity entity = _registry.createEntity();
            _entityMap.emplace(data.id, entity);

            _registry.emplaceComponent<Position>(entity, Position{data.x, data.y});
            _registry.emplaceComponent<Drawable>(entity, Drawable{data.spriteId});

            const auto &sprite = _spriteRegistry->get(data.spriteId);

            _registry.emplaceComponent<Render>(entity, Render{sprite.textureHandle});
            _registry.emplaceComponent<AnimationState>(
                entity, AnimationState{.currentAnimation = sprite.defaultAnimation, .frameIndex = 0, .elapsed = 0.f});
        } catch (const std::exception &e) {
            std::cerr << "{ClientWorld::applyCreate}" << e.what() << std::endl;
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

        if (auto &pos = _registry.getComponents<Position>().at(entityIndex)) {
            pos->x = entity.x;
            pos->y = entity.y;
        }

        if (auto &drawable = _registry.getComponents<Drawable>().at(entityIndex)) {
            if (const bool spriteChanged = (drawable->spriteId != entity.spriteId);
                spriteChanged && _spriteRegistry->exists(entity.spriteId)) {
                drawable->spriteId = entity.spriteId;
                const auto &sprite = _spriteRegistry->get(drawable->spriteId);

                if (auto &render = _registry.getComponents<Render>().at(entityIndex)) {
                    render->texture = sprite.textureHandle;

                    if (auto &animState = _registry.getComponents<AnimationState>().at(entityIndex)) {
                        animState->currentAnimation = sprite.defaultAnimation;
                        animState->frameIndex = 0;
                        animState->elapsed = 0.f;
                    }
                }
            }
        }
    }
} // namespace Engine
