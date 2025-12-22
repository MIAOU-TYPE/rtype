/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#include "ClientWorld.hpp"
#include <iostream>

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
        _registry.view<Position, Drawable, AnimationState, Render>(
            [&](Ecs::Entity, const Position &, const Drawable &drawable, AnimationState &animState, const Render &) {
                if (!_spriteRegistry->exists(drawable.spriteId))
                    return;

                const SpriteDefinition &sprite = _spriteRegistry->get(drawable.spriteId);

                if (animState.currentAnimation.empty())
                    animState.currentAnimation = sprite.defaultAnimation;

                auto animIt = sprite.animations.find(animState.currentAnimation);
                if (animIt == sprite.animations.end()) {
                    animState.currentAnimation = sprite.defaultAnimation;
                    animState.frameIndex = 0;
                    animState.elapsed = 0.f;

                    animIt = sprite.animations.find(animState.currentAnimation);
                    if (animIt == sprite.animations.end())
                        return;
                }

                const Animation &anim = animIt->second;

                AnimationSystem::update(animState, anim, dt);
            });
    }

    Ecs::Registry &ClientWorld::registry()
    {
        return _registry;
    }

    void ClientWorld::applyCreate(const EntityCreate &data)
    {
        try {
            if (!_spriteRegistry->exists(data.spriteId))
                return;
            if (_entityMap.contains(data.id)) {
                std::cerr << "{ClientWorld::applyCreate} Entity " << data.id << " already exists\n";
                return;
            }

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
            if (_destroyTimers.contains(entity.id))
                return;
            std::cout << "{ClientWorld::applySnapshot} Entity " << entity.id << " not found, creating it.\n";
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
