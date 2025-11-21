#include "Registry.hpp"

namespace Ecs
{

    Registry::Registry() : _nextEntity(Entity(0))
    {
    }

    Entity Registry::spawnEntity()
    {
        Entity entity(0);

        if (!_availableEntities.empty()) {
            entity = _availableEntities.back();
            _availableEntities.pop_back();
        } else {
            entity = Entity(static_cast<std::size_t>(_nextEntity));
            _nextEntity = Entity(static_cast<std::size_t>(_nextEntity) + 1);
        }

        _entities.push_back(entity);
        return entity;
    }

    Entity Registry::entityFromIndex(std::size_t idx) const
    {
        if (idx >= _entities.size()) {
            throw std::out_of_range("Entity index out of range");
        }
        return _entities[idx];
    }

    void Registry::killEntity(Entity entity)
    {
        auto it = std::find(_entities.begin(), _entities.end(), entity);
        if (it != _entities.end()) {
            _entities.erase(it);
            _availableEntities.push_back(entity);

            for (auto &[type, eraser] : _componentErasers) {
                eraser(entity);
            }
        }
    }

    std::size_t Registry::size() const
    {
        return _entities.size();
    }

    const std::vector<Entity> &Registry::getEntities() const
    {
        return _entities;
    }

} // namespace Ecs
