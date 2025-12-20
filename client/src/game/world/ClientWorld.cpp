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
    ClientWorld::ClientWorld(std::shared_ptr<SpriteRegistry> spriteRegistry)
        : _spriteRegistry(std::move(spriteRegistry))
    {
        _registry.registerComponent<Position>();
        _registry.registerComponent<Drawable>();
        _registry.registerComponent<AnimationState>();
        _registry.registerComponent<Render>();
    }

    void ClientWorld::update(const float dt, Graphics::IRenderer &renderer)
    {
        _registry.view<Position, Drawable, AnimationState, Render>(
            [&](Ecs::Entity, const Position &pos, const Drawable &drawable, AnimationState &animState,
                const Render &render) {
                try {
                    const SpriteDefinition &sprite = _spriteRegistry->get(drawable.spriteId);

                    if (animState.currentAnimation.empty())
                        animState.currentAnimation = sprite.defaultAnimation;

                    const auto animIt = sprite.animations.find(animState.currentAnimation);
                    if (animIt == sprite.animations.end())
                        return;

                    const Animation &anim = animIt->second;

                    AnimationSystem::update(animState, anim, dt);
                    RenderSystem::submit(pos, render, animState, anim, renderer);
                } catch (const std::exception &e) {
                    std::cerr << "{ClientWorld::update}: " << e.what() << std::endl;
                }
            });
    }

    Ecs::Registry &ClientWorld::registry()
    {
        return _registry;
    }

    void ClientWorld::applyCreate(const EntityCreate &data)
    {
        try {
            if (_entityMap.contains(data.id)) {
                std::cerr << "{ClientWorld::applyCreate} Entity " << data.id << " already exists\n";
                return;
            }

            const Ecs::Entity entity = _registry.createEntity();
            _entityMap.emplace(data.id, entity);

            _registry.emplaceComponent<Position>(entity, Position{data.x, data.y});
            _registry.emplaceComponent<Drawable>(entity, Drawable{data.sprite});

            const auto &sprite = _spriteRegistry->get(data.sprite);

            _registry.emplaceComponent<Render>(entity, Render{sprite.textureHandle});
            _registry.emplaceComponent<AnimationState>(
                entity, AnimationState{.currentAnimation = sprite.defaultAnimation, .frameIndex = 0, .elapsed = 0.f});
        } catch (const std::exception &e) {
            std::cerr << "{ClientWorld::applyCreate}" << e.what() << std::endl;
        }
    }

    void ClientWorld::applyDestroy(const EntityDestroy &data)
    {
        auto entity = static_cast<Ecs::Entity>(data.id);
        const auto it = _entityMap.find(data.id);
        if (it == _entityMap.end()) {
            std::cerr << "{ClientWorld::applyDestroy} Entity with ID " << data.id
                      << " does not exist. Cannot destroy.\n";
            return;
        }
        entity = it->second;
        _entityMap.erase(it);
        _registry.destroyEntity(entity);
    }

    void ClientWorld::applySnapshot(const SnapshotEntity &entity)
    {
        const auto it = _entityMap.find(entity.id);
        if (it == _entityMap.end()) {
            std::cout << "{ClientWorld::applySnapshot} Entity " << entity.id << " not found, creating it.\n";
            applyCreate(EntityCreate{entity.id, entity.x, entity.y, entity.sprite});
            return;
        }

        Ecs::Entity localEntity = it->second;

        auto &pos = _registry.getComponents<Position>()[static_cast<size_t>(localEntity)];
        pos->x = entity.x;
        pos->y = entity.y;

        auto &drawable = _registry.getComponents<Drawable>()[static_cast<size_t>(localEntity)];
        drawable->spriteId = entity.sprite;
    }

} // namespace Engine
