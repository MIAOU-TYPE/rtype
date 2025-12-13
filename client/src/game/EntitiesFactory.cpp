/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntitiesFactory
*/

#include "EntitiesFactory.hpp"

namespace Ecs
{
    EntitiesFactory::EntitiesFactory(Registry &registry, std::shared_ptr<Graphics::ITextureManager> textureManager)
        : _registry(registry), _textureManager(textureManager)
    {
    }

    void EntitiesFactory::createEntityFromServer(u_int64_t id, float x, float y, uint16_t sprite)
    {
        Entity entity = _registry.createEntity();
        _entityMap[id] = entity;

        switch (sprite) {
            case 1: createPlayer(entity, x, y, sprite); break;
            case 2: createEnemy(entity, x, y, sprite); break;
            case 3: createProjectile(entity, x, y, sprite); break;
            default: break;
        }
    }

    void EntitiesFactory::destroyEntityFromServer(u_int64_t id)
    {
        auto it = _entityMap.find(id);
        if (it != _entityMap.end()) {
            _registry.destroyEntity(it->second);
            _entityMap.erase(it);
        }
    }

    void EntitiesFactory::createPlayer(Entity entity, float x, float y, uint16_t sprite)
    {
        _registry.emplaceComponent<Health>(entity, Health{100, 100});
        _registry.emplaceComponent<Controllable>(entity);
        _registry.emplaceComponent<Damageable>(entity);
        _registry.emplaceComponent<Position>(entity, Position{x, y});
        _registry.emplaceComponent<Renderable>(entity, Renderable{true, 1.0f, 1.0f, 1.0f});
        _registry.emplaceComponent<Direction>(entity, Direction{0, 0});
        _registry.emplaceComponent<Velocity>(entity, Velocity{0.0f, 0.0f});
    }

    void EntitiesFactory::createEnemy(Entity entity, float x, float y, uint16_t sprite)
    {
        _registry.emplaceComponent<Health>(entity, Health{50, 50});
        _registry.emplaceComponent<Damageable>(entity);
        _registry.emplaceComponent<Position>(entity, Position{x, y});
        _registry.emplaceComponent<Renderable>(entity, Renderable{true, 1.0f, 1.0f, 1.0f});
        _registry.emplaceComponent<Direction>(entity, Direction{0, 0});
        _registry.emplaceComponent<Velocity>(entity, Velocity{0.0f, 0.0f});
    }

    void EntitiesFactory::createProjectile(Entity entity, float x, float y, uint16_t sprite)
    {
        _registry.emplaceComponent<Damage>(entity, Damage{10});
        _registry.emplaceComponent<Position>(entity, Position{x, y});
        _registry.emplaceComponent<Renderable>(entity, Renderable{true, 1.0f, 1.0f, 1.0f});
        _registry.emplaceComponent<Direction>(entity, Direction{0, -1});
        _registry.emplaceComponent<Velocity>(entity, Velocity{0.0f, -300.0f});
    }
} // namespace Ecs
