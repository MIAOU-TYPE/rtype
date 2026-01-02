/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#include "ClientWorld.hpp"

namespace World
{
    ClientWorld::ClientWorld(
        std::shared_ptr<const Engine::SpriteRegistry> spriteRegistry, std::shared_ptr<Graphics::IRenderer> renderer)
        : _spriteRegistry(std::move(spriteRegistry)), _renderer(std::move(renderer))
    {
        _registry.registerComponent<Ecs::Position>();
        _registry.registerComponent<Ecs::Drawable>();
        _registry.registerComponent<Ecs::Render>();
        _registry.registerComponent<Ecs::AnimationState>();
        _registry.registerComponent<Ecs::ScrollingBackground>();

        initializeScrollingBackground();
    }

    void ClientWorld::step(const float dt)
    {
        Engine::AnimationSystem::update(_registry, _spriteRegistry, dt);
        Engine::BackgroundSystem::update(_registry, dt);
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

            _registry.emplaceComponent<Ecs::Position>(entity, Ecs::Position{data.x, data.y});
            _registry.emplaceComponent<Ecs::Drawable>(entity, Ecs::Drawable{data.spriteId});

            const auto &sprite = _spriteRegistry->get(data.spriteId);

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

    void ClientWorld::initializeScrollingBackground()
    {
        auto bgTexture = _renderer->textures()->load("sprites/background_space.png");
        if (bgTexture == Graphics::InvalidTexture)
            return;

        auto texSize = _renderer->textures()->getSize(bgTexture);
        const float spriteWidth = static_cast<float>(texSize.width);
        const float spriteHeight = static_cast<float>(texSize.height);

        const auto viewportSize = _renderer->getViewportSize();
        const float scale = static_cast<float>(viewportSize.height) / spriteHeight;
        const float scaledWidth = spriteWidth * scale;

        const Ecs::Entity bg1 = _registry.createEntity();
        _registry.emplaceComponent<Ecs::Position>(bg1, Ecs::Position{0.f, 0.f});
        _registry.emplaceComponent<Ecs::Render>(bg1, Ecs::Render{bgTexture});
        _registry.emplaceComponent<Ecs::ScrollingBackground>(bg1,
            Ecs::ScrollingBackground{.scrollSpeed = -50.f, .spriteWidth = scaledWidth, .spriteHeight = spriteHeight});

        const Ecs::Entity bg2 = _registry.createEntity();
        _registry.emplaceComponent<Ecs::Position>(bg2, Ecs::Position{scaledWidth, 0.f});
        _registry.emplaceComponent<Ecs::Render>(bg2, Ecs::Render{bgTexture});
        _registry.emplaceComponent<Ecs::ScrollingBackground>(bg2,
            Ecs::ScrollingBackground{.scrollSpeed = -50.f, .spriteWidth = scaledWidth, .spriteHeight = spriteHeight});
    }
} // namespace World
