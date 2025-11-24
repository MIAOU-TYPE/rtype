/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SparseArray
*/

#pragma once
#include <optional>
#include <vector>

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @class SparseArray
     * @brief A sparse array used to store components by entity index.
     *
     * Each index in the array represents an entity ID.
     * A component can be present or not using std::optional.
     *
     * @tparam Component Type of the stored component
     */
    template <typename Component>
    class SparseArray {
      public:
        /** @brief Default constructor and destructor */
        SparseArray() = default;
        //** @brief Default destructor */
        ~SparseArray() = default;

        /**
         * @brief Inserts or replaces a component at a specific index.
         * @param index Entity index
         * @param component Component to insert
         */
        void insert(size_t index, const Component &component) noexcept;

        /**
         * @brief Removes the component at a specific index.
         * @param index Entity index
         */
        void remove(size_t index) noexcept;

        /**
         * @brief Accesses the optional component at a specific index.
         *
         * If the index is out of bounds, returns an empty optional.
         *
         * @param index Entity index
         * @return Reference to the optional component
         */
        std::optional<Component> &operator[](size_t index);

        /**
         * @brief Gets the current size of the sparse array.
         * @return Number of stored slots
         */
        size_t size() const noexcept;

      private:
        /** @brief Underlying container */
        std::vector<std::optional<Component>> _components;
    };
} // namespace Ecs

#include "SparseArray.tpp"