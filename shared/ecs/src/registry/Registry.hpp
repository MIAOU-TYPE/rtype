/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Registry
*/

#pragma once
#include <any>
#include <cstddef>
#include <functional>
#include <typeindex>
#include <vector>
#include "Entity.hpp"
#include "SparseArray.hpp"
#include <unordered_map>

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @class Registry
     * @brief Central class of the ECS that manages entities and components.
     *
     * The Registry is responsible for:
     * - Creating and destroying entities
     * - Registering component types
     * - Attaching and removing components to entities
     * - Iterating over entities owning specific components
     */
    class Registry {
      public:
        /**
         * @brief Creates a new entity.
         * @return A newly created Entity with a unique ID.
         */
        Entity createEntity() noexcept;

        /**
         * @brief Destroys an entity and removes all of its components.
         * @param entity The entity to destroy.
         */
        void destroyEntity(Entity entity) noexcept;

        /**
         * @brief Registers a new component type in the registry.
         *
         * If the component type is already registered, nothing happens.
         *
         * @tparam T Component type
         * @return A reference to the component SparseArray
         */
        template <typename T>
        SparseArray<T> &registerComponent();

        /**
         * @brief Gets the SparseArray associated with a component type.
         *
         * The component must have been registered previously.
         *
         * @tparam T Component type
         * @return A reference to the component SparseArray
         */
        template <typename T>
        SparseArray<T> &getComponents();

        /**
         * @brief Constructs and assigns a component to an entity.
         *
         * @tparam T Component type
         * @tparam Args Constructor parameter pack
         * @param entity Target entity
         * @param args Arguments forwarded to the component constructor
         */
        template <typename T, typename... Args>
        void emplaceComponent(Entity entity, Args &&...args);

        /**
         * @brief Checks if an entity owns a specific component.
         *
         * @tparam T Component type
         * @param entity Target entity
         * @return true if the entity has the component, false otherwise
         */
        template <typename T>
        bool hasComponent(Entity entity);

        /**
         * @brief Iterates over entities owning a set of components.
         *
         * Only entities with all specified components will be passed to the function.
         *
         * @tparam Components List of required component types
         * @tparam Function Callable type
         * @param fn Function called for each valid entity
         *
         * Function signature must be:
         * `void(Entity, Components&...)`
         */
        template <typename... Components, typename Function>
        void view(Function fn);

        /**
         * @brief Clears the registry, removing all entities and components.
         */
        void clear() noexcept;

      private:
        /** @brief Counter used to assign unique IDs to entities */
        size_t _entityCounter = 0;

        /** @brief Stores all registered component arrays indexed by type */
        std::unordered_map<std::type_index, std::any> _entityToIndex = {};

        /** @brief List of cleanup functions called during entity destruction */
        std::vector<std::function<void(Registry &, Entity)>> _destroyers = {};
    };
} // namespace Ecs

#include "Registry.tpp"
