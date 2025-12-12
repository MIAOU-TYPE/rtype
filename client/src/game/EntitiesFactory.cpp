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
        case 1:
            createPlayer(entity, x, y, sprite);
            break;
        case 2:
            createEnemy(entity, x, y, sprite);
            break;
        case 3:
            createProjectile(entity, x, y, sprite);
            break;
        default:
            break;
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
} // namespace Ecs
