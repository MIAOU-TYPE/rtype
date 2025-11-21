/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Registry
*/

#pragma once

#include "Registry.hpp"

namespace Ecs {

template<typename Component>
SparseArray<Component>& Registry::registerComponent() {
    std::type_index typeIndex(typeid(Component));
    
    if (_components.find(typeIndex) == _components.end()) {
        auto sparseArray = std::make_shared<SparseArray<Component>>();
        _components[typeIndex] = sparseArray;
        
        _componentErasers[typeIndex] = [sparseArray](Entity entity) {
            sparseArray->erase(entity);
        };
    }
    
    return *std::any_cast<std::shared_ptr<SparseArray<Component>>>(_components[typeIndex]);
}

template<typename Component>
SparseArray<Component>& Registry::getComponents() {
    std::type_index typeIndex(typeid(Component));
    
    auto it = _components.find(typeIndex);
    if (it == _components.end()) {
        throw std::runtime_error("Component type not registered");
    }
    
    return *std::any_cast<std::shared_ptr<SparseArray<Component>>>(it->second);
}

template<typename Component>
const SparseArray<Component>& Registry::getComponents() const {
    std::type_index typeIndex(typeid(Component));
    
    auto it = _components.find(typeIndex);
    if (it == _components.end()) {
        throw std::runtime_error("Component type not registered");
    }
    
    return *std::any_cast<std::shared_ptr<SparseArray<Component>>>(it->second);
}

template<typename Component>
typename SparseArray<Component>::referenceType Registry::addComponent(Entity entity, const Component& component) {
    auto& components = getComponents<Component>();
    return components.insertAt(entity, component);
}

template<typename Component>
typename SparseArray<Component>::referenceType Registry::addComponent(Entity entity, Component&& component) {
    auto& components = getComponents<Component>();
    return components.insertAt(entity, std::move(component));
}

template<typename Component, typename... Params>
typename SparseArray<Component>::referenceType Registry::emplaceComponent(Entity entity, Params&&... params) {
    auto& components = getComponents<Component>();
    return components.emplaceAt(entity, std::forward<Params>(params)...);
}

template<typename Component>
void Registry::removeComponent(Entity entity) {
    auto& components = getComponents<Component>();
    components.erase(entity);
}

template<typename Component>
bool Registry::hasComponent(Entity entity) const {
    try {
        const auto& components = getComponents<Component>();
        if (entity >= components.size()) {
            return false;
        }
        return components[entity].has_value();
    } catch (const std::runtime_error&) {
        return false;
    }
}

template<typename Component>
Component& Registry::getComponent(Entity entity) {
    auto& components = getComponents<Component>();
    if (entity >= components.size() || !components[entity].has_value()) {
        throw std::runtime_error("Entity doesn't have this component");
    }
    return components[entity].value();
}

template<typename Component>
const Component& Registry::getComponent(Entity entity) const {
    const auto& components = getComponents<Component>();
    if (entity >= components.size() || !components[entity].has_value()) {
        throw std::runtime_error("Entity doesn't have this component");
    }
    return components[entity].value();
}

} // namespace Ecs