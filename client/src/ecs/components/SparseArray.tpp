/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SparseArray
*/

#pragma once

#include "SparseArray.hpp"

namespace Ecs {

template<typename Component>
typename SparseArray<Component>::referenceType SparseArray<Component>::operator[](sizeType idx) {
    if (idx >= _data.size()) {
        _data.resize(idx + 1);
    }
    return _data[idx];
}

template<typename Component>
const typename SparseArray<Component>::valueType& SparseArray<Component>::operator[](sizeType idx) const {
    if (idx >= _data.size()) {
        static const valueType empty;
        return empty;
    }
    return _data[idx];
}


template<typename Component>
typename SparseArray<Component>::sizeType SparseArray<Component>::size() const {
    return _data.size();
}

template<typename Component>
typename SparseArray<Component>::referenceType SparseArray<Component>::insertAt(sizeType pos, const Component& component) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos] = component;
    return _data[pos];
}

template<typename Component>
typename SparseArray<Component>::referenceType SparseArray<Component>::insertAt(sizeType pos, Component&& component) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos] = std::move(component);
    return _data[pos];
}

template<typename Component>
template<typename... Params>
typename SparseArray<Component>::referenceType SparseArray<Component>::emplaceAt(sizeType pos, Params&&... params) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos].emplace(std::forward<Params>(params)...);
    return _data[pos];
}

template<typename Component>
void SparseArray<Component>::erase(sizeType pos) {
    if (pos < _data.size()) {
        _data[pos].reset();
    }
}

} // namespace Ecs