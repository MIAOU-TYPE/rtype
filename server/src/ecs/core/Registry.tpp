/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Registry
*/

namespace Ecs
{
    template <typename T>
    SparseArray<T> &Registry::registerComponent()
    {
        std::type_index typeIdx(typeid(T));

        if (_entityToIndex.find(typeIdx) == _entityToIndex.end()) {
            _entityToIndex[typeIdx] = SparseArray<T>();
            _destroyers.push_back([](Registry &reg, Entity ent) {
                reg.getComponents<T>().remove(static_cast<size_t>(ent));
            });
        }
        return std::any_cast<SparseArray<T> &>(_entityToIndex[typeIdx]);
    }

    template <typename T>
    SparseArray<T> &Registry::getComponents()
    {
        return std::any_cast<SparseArray<T> &>(
            _entityToIndex.at(std::type_index(typeid(T))));
    }

    template <typename T, typename... Args>
    void Registry::emplaceComponent(Entity entity, Args &&...args)
    {
        registerComponent<T>();
        getComponents<T>().insert(
            static_cast<size_t>(entity),
            T(std::forward<Args>(args)...));
    }

    template <typename T>
    bool Registry::hasComponent(Entity entity)
    {
        auto typeIdx = std::type_index(typeid(T));
        if (_entityToIndex.find(typeIdx) == _entityToIndex.end())
            return false;
        auto &arr = std::any_cast<SparseArray<T>&>(_entityToIndex[typeIdx]);
        if (static_cast<size_t>(entity) >= arr.size())
            return false;
        return arr[entity].has_value();
    }

    template <typename... Components, typename Function>
    void Registry::view(Function func)
    {
        if ((!_entityToIndex.contains(std::type_index(typeid(Components))) || ...))
            return;
        auto arrays = std::forward_as_tuple(getComponents<Components>()...);
        auto &first = std::get<0>(arrays);
        for (size_t i = 0; i < first.size(); ++i) {
            if ((std::get<SparseArray<Components>&>(arrays)[Entity(i)].has_value() && ...))
                func(Entity(i), *std::get<SparseArray<Components>&>(arrays)[i]...);
        }
    }
}
