/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Registry
*/

#pragma once

namespace Ecs
{
    template <typename T>
    SparseArray<T> &Registry::registerComponent()
    {
        const std::type_index typeIdx(typeid(T));

        if (_entityToIndex.contains(typeIdx) == false) {
            _entityToIndex[typeIdx] = SparseArray<T>();
            _destroyers.push_back([](Registry &reg, const Entity ent) {
                reg.getComponents<T>().remove(static_cast<size_t>(ent));
            });
        }
        return std::any_cast<SparseArray<T> &>(_entityToIndex[typeIdx]);
    }

    template <typename T>
    SparseArray<T> &Registry::getComponents()
    {
        return registerComponent<T>();
    }

    template <typename T, typename... Args>
    void Registry::emplaceComponent(const Entity entity, Args &&...args)
    {
        auto data = registerComponent<T>();
        getComponents<T>().insert(static_cast<size_t>(entity), T(std::forward<Args>(args)...));
    }

    template <typename T>
    bool Registry::hasComponent(const Entity entity)
    {
        const auto typeIdx = std::type_index(typeid(T));
        if (!_entityToIndex.contains(typeIdx))
            return false;
        auto &arr = std::any_cast<SparseArray<T> &>(_entityToIndex[typeIdx]);
        auto idx = static_cast<size_t>(entity);
        if (idx >= arr.size())
            return false;
        return arr[idx].has_value();
    }

    template <typename... Components, typename Function>
    void Registry::view(Function fn)
    {
        if ((!_entityToIndex.contains(std::type_index(typeid(Components))) || ...))
            return;
        auto arrays = std::forward_as_tuple(getComponents<Components>()...);
        auto &first = std::get<0>(arrays);
        const size_t maxSize = first.size();

        for (size_t i = 0; i < maxSize; ++i) {
            const bool ok = ((i < std::get<SparseArray<Components> &>(arrays).size()
                                 && std::get<SparseArray<Components> &>(arrays)[i].has_value())
                && ...);
            if (!ok)
                continue;

            fn(Entity(i), *std::get<SparseArray<Components> &>(arrays)[i]...);
        }
    }

} // namespace Ecs
