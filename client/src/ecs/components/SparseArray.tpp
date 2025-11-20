#ifndef SPARSE_ARRAY_TPP
#define SPARSE_ARRAY_TPP

#include "SparseArray.hpp"

namespace Ecs {

template<typename Component>
typename SparseArray<Component>::reference_type SparseArray<Component>::operator[](size_type idx) {
    if (idx >= _data.size()) {
        _data.resize(idx + 1);
    }
    return _data[idx];
}

template<typename Component>
const typename SparseArray<Component>::value_type& SparseArray<Component>::operator[](size_type idx) const {
    if (idx >= _data.size()) {
        static const value_type empty;
        return empty;
    }
    return _data[idx];
}


template<typename Component>
typename SparseArray<Component>::size_type SparseArray<Component>::size() const {
    return _data.size();
}

template<typename Component>
typename SparseArray<Component>::reference_type SparseArray<Component>::insert_at(size_type pos, const Component& component) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos] = component;
    return _data[pos];
}

template<typename Component>
typename SparseArray<Component>::reference_type SparseArray<Component>::insert_at(size_type pos, Component&& component) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos] = std::move(component);
    return _data[pos];
}

template<typename Component>
template<typename... Params>
typename SparseArray<Component>::reference_type SparseArray<Component>::emplace_at(size_type pos, Params&&... params) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos].emplace(std::forward<Params>(params)...);
    return _data[pos];
}

template<typename Component>
void SparseArray<Component>::erase(size_type pos) {
    if (pos < _data.size()) {
        _data[pos].reset();
    }
}

} // namespace Ecs

#endif // SPARSE_ARRAY_TPP
