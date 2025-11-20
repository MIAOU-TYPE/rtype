#pragma once
#include <cstddef>

namespace ecs {

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

/**
 * @brief Special constant representing a null/invalid entity
 */
const Entity NULL_ENTITY(static_cast<std::size_t>(-1));

} // namespace ecs
