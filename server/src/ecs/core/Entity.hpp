/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Entity
*/

#pragma once
#include <cstddef>
#include <cstdint>

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @class Entity
     * @brief Represents a unique entity identifier in the ECS.
     *
     * An Entity is simply a wrapper around a unique ID (index).
     * It does not store any data besides this identifier.
     */
    class Entity {
      public:
        /**
         * @brief Construct an Entity with a given ID.
         * @param id Unique identifier for the entity (default: 0)
         */
        explicit Entity(size_t id = 0);

        /**
         * @brief Implicit conversion to size_t.
         * @return The entity ID.
         */
        explicit operator size_t() const noexcept;

      private:
        /** @brief Unique identifier of the entity */
        size_t _id = 0;
    };
} // namespace Ecs
