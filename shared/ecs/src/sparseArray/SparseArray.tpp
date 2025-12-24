/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** sparseArray
*/

#pragma once

namespace Ecs
{
    template <typename Component>
    void SparseArray<Component>::insert(size_t index, const Component &component) noexcept
    {
        if (index >= _components.size())
            _components.resize(index + 1);
        _components[index] = component;
    }

    template <typename Component>
    void SparseArray<Component>::remove(size_t index) noexcept
    {
        if (index < _components.size())
            _components[index].reset();
    }

    template <typename Component>
    std::optional<Component> &SparseArray<Component>::at(size_t index) noexcept
    {
        static std::optional<Component> empty = std::nullopt;
        if (index >= _components.size())
            return empty;
        return _components[index];
    }

    template <typename Component>
    std::optional<Component> SparseArray<Component>::at(size_t index) const noexcept
    {
        static std::optional<Component> empty = std::nullopt;
        if (index >= _components.size())
            return empty;
        return _components[index];
    }

    template <typename Component>
    size_t SparseArray<Component>::size() const noexcept
    {
        return _components.size();
    }
} // namespace Ecs
