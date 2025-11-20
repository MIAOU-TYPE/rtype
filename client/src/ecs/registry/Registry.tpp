#ifndef REGISTRY_TPP
#define REGISTRY_TPP

#include "Registry.hpp"

namespace ecs {

template<typename Component>
SparseArray<Component>& Registry::register_component() {
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
SparseArray<Component>& Registry::get_components() {
    std::type_index typeIndex(typeid(Component));
    
    auto it = _components.find(typeIndex);
    if (it == _components.end()) {
        throw std::runtime_error("Component type not registered");
    }
    
    return *std::any_cast<std::shared_ptr<SparseArray<Component>>>(it->second);
}

template<typename Component>
const SparseArray<Component>& Registry::get_components() const {
    std::type_index typeIndex(typeid(Component));
    
    auto it = _components.find(typeIndex);
    if (it == _components.end()) {
        throw std::runtime_error("Component type not registered");
    }
    
    return *std::any_cast<std::shared_ptr<SparseArray<Component>>>(it->second);
}

template<typename Component>
typename SparseArray<Component>::reference_type Registry::add_component(Entity entity, const Component& component) {
    auto& components = get_components<Component>();
    return components.insert_at(entity, component);
}

template<typename Component>
typename SparseArray<Component>::reference_type Registry::add_component(Entity entity, Component&& component) {
    auto& components = get_components<Component>();
    return components.insert_at(entity, std::move(component));
}

template<typename Component, typename... Params>
typename SparseArray<Component>::reference_type Registry::emplace_component(Entity entity, Params&&... params) {
    auto& components = get_components<Component>();
    return components.emplace_at(entity, std::forward<Params>(params)...);
}

template<typename Component>
void Registry::remove_component(Entity entity) {
    auto& components = get_components<Component>();
    components.erase(entity);
}

template<typename Component>
bool Registry::has_component(Entity entity) const {
    try {
        const auto& components = get_components<Component>();
        if (entity >= components.size()) {
            return false;
        }
        return components[entity].has_value();
    } catch (const std::runtime_error&) {
        return false;
    }
}

template<typename Component>
Component& Registry::get_component(Entity entity) {
    auto& components = get_components<Component>();
    if (entity >= components.size() || !components[entity].has_value()) {
        throw std::runtime_error("Entity doesn't have this component");
    }
    return components[entity].value();
}

template<typename Component>
const Component& Registry::get_component(Entity entity) const {
    const auto& components = get_components<Component>();
    if (entity >= components.size() || !components[entity].has_value()) {
        throw std::runtime_error("Entity doesn't have this component");
    }
    return components[entity].value();
}

} // namespace ecs

#endif // REGISTRY_TPP
