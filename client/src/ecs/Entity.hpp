#pragma once
#include <cstddef>

namespace Ecs {

/**
 * @brief Entity class representing a unique identifier for an entity in the ECS system
 * 
 * An entity is simply an ID that serves as an index to access components.
 * It doesn't contain any data itself, just acts as a handle.
 */
class Entity {
public:
    explicit Entity(std::size_t id) : _id(id) {}
    operator std::size_t() const { return _id; }

    bool operator==(const Entity& other) const { return _id == other._id; }
    bool operator!=(const Entity& other) const { return _id != other._id; }
    bool operator<(const Entity& other) const { return _id < other._id; }

private:
    std::size_t _id;
};

} // namespace Ecs
