#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <algorithm>
#include <any>
#include <memory>
#include <stdexcept>
#include "../Entity.hpp"
#include "../components/SparseArray.hpp"

namespace Ecs {

/**
 * @brief Central registry for the ECS system
 * 
 * The Registry manages:
 * - Entity creation and destruction
 * - Component storage and retrieval
 * - Component addition and removal from entities
 * 
 * Components are stored in separate SparseArrays, one per component type.
 * This allows for efficient iteration over entities with specific components.
 */
class Registry {
public:
    /**
     * @brief Constructor
     */
    Registry();

    /**
     * @brief Destructor
     */
    ~Registry() = default;

    Registry(const Registry&) = delete;
    Registry& operator=(const Registry&) = delete;

    Registry(Registry&&) noexcept = default;
    Registry& operator=(Registry&&) noexcept = default;

    /**
     * @brief Create a new entity
     * @return The newly created entity ID
     */
    Entity spawn_entity();

    /**
     * @brief Get an entity from an index
     * @param idx Index of the entity
     * @return The entity at the given index
     */
    Entity entity_from_index(std::size_t idx) const;

    /**
     * @brief Kill an entity and remove all its components
     * @param entity The entity to destroy
     */
    void kill_entity(Entity entity);

    /**
     * @brief Register a component type with the registry
     * @tparam Component The component type to register
     * @return Reference to the SparseArray for this component type
     */
    template<typename Component>
    SparseArray<Component>& register_component();

    /**
     * @brief Get the SparseArray for a component type
     * @tparam Component The component type
     * @return Reference to the SparseArray for this component type
     */
    template<typename Component>
    SparseArray<Component>& get_components();

    /**
     * @brief Get the SparseArray for a component type (const version)
     * @tparam Component The component type
     * @return Const reference to the SparseArray for this component type
     */
    template<typename Component>
    const SparseArray<Component>& get_components() const;

    /**
     * @brief Add a component to an entity
     * @tparam Component The component type
     * @param entity The entity to add the component to
     * @param component The component to add
     * @return Reference to the added component
     */
    template<typename Component>
    typename SparseArray<Component>::reference_type add_component(Entity entity, const Component& component);

    /**
     * @brief Add a component to an entity (move version)
     * @tparam Component The component type
     * @param entity The entity to add the component to
     * @param component The component to add
     * @return Reference to the added component
     */
    template<typename Component>
    typename SparseArray<Component>::reference_type add_component(Entity entity, Component&& component);

    /**
     * @brief Emplace a component for an entity
     * @tparam Component The component type
     * @tparam Params Parameter pack for component constructor
     * @param entity The entity to add the component to
     * @param params Parameters to forward to component constructor
     * @return Reference to the emplaced component
     */
    template<typename Component, typename... Params>
    typename SparseArray<Component>::reference_type emplace_component(Entity entity, Params&&... params);

    /**
     * @brief Remove a component from an entity
     * @tparam Component The component type
     * @param entity The entity to remove the component from
     */
    template<typename Component>
    void remove_component(Entity entity);

    /**
     * @brief Check if an entity has a component
     * @tparam Component The component type
     * @param entity The entity to check
     * @return true if the entity has the component, false otherwise
     */
    template<typename Component>
    bool has_component(Entity entity) const;

    /**
     * @brief Get a component from an entity
     * @tparam Component The component type
     * @param entity The entity to get the component from
     * @return Reference to the component
     * @throws std::runtime_error if the entity doesn't have the component
     */
    template<typename Component>
    Component& get_component(Entity entity);

    /**
     * @brief Get a component from an entity (const version)
     * @tparam Component The component type
     * @param entity The entity to get the component from
     * @return Const reference to the component
     * @throws std::runtime_error if the entity doesn't have the component
     */
    template<typename Component>
    const Component& get_component(Entity entity) const;

    /**
     * @brief Get the number of active entities
     * @return The number of entities
     */
    std::size_t size() const;

    /**
     * @brief Get all active entities
     * @return Vector of all active entities
     */
    const std::vector<Entity>& get_entities() const;

private:
    Entity _nextEntity;
    std::vector<Entity> _entities;
    std::vector<Entity> _availableEntities;
    std::unordered_map<std::type_index, std::any> _components;
    std::unordered_map<std::type_index, std::function<void(Entity)>> _componentErasers;
};

} // namespace Ecs

#include "Registry.tpp"

#endif // REGISTRY_HPP
