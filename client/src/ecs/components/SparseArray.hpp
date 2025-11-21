/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SparseArray
*/

#pragma once

#include <vector>
#include <optional>
#include <stdexcept>

namespace Ecs {

/**
 * @brief A sparse array container that stores optional components
 * 
 * This container is optimized for sparse data where many entities may not
 * have a particular component. It uses std::optional to represent the presence
 * or absence of a component.
 * 
 * @tparam Component The type of component to store
 */
template<typename Component>
class SparseArray {
public:
    using valueType = std::optional<Component>;
    using referenceType = valueType&;
    using sizeType = std::size_t;

    /**
     * @brief Default constructor
     */
    SparseArray() = default;

    /**
     * @brief Copy constructor
     */
    SparseArray(const SparseArray&) = default;

    /**
     * @brief Move constructor
     */
    SparseArray(SparseArray&&) noexcept = default;

    /**
     * @brief Destructor
     */
    ~SparseArray() = default;

    /**
     * @brief Copy assignment operator
     */
    SparseArray& operator=(const SparseArray&) = default;

    /**
     * @brief Move assignment operator
     */
    SparseArray& operator=(SparseArray&&) noexcept = default;

    /**
     * @brief Access element at given index
     * @param idx Index of the element
     * @return Reference to the optional component
     */
    referenceType operator[](sizeType idx);

    /**
     * @brief Access element at given index (const version)
     * @param idx Index of the element
     * @return Const reference to the optional component
     */
    const valueType& operator[](sizeType idx) const;

    /**
     * @brief Get the size of the array
     */
    sizeType size() const;

    /**
     * @brief Insert a component at the given position
     * @param pos Position to insert at
     * @param component Component to insert
     * @return Reference to the inserted component
     */
    referenceType insertAt(sizeType pos, const Component& component);

    /**
     * @brief Insert a component at the given position (move version)
     * @param pos Position to insert at
     * @param component Component to insert
     * @return Reference to the inserted component
     */
    referenceType insertAt(sizeType pos, Component&& component);

    /**
     * @brief Emplace a component at the given position
     * @tparam Params Parameter pack for component constructor
     * @param pos Position to emplace at
     * @param params Parameters to forward to component constructor
     * @return Reference to the emplaced component
     */
    template<typename... Params>
    referenceType emplaceAt(sizeType pos, Params&&... params);

    /**
     * @brief Erase the component at the given position
     * @param pos Position to erase
     */
    void erase(sizeType pos);

private:
    std::vector<valueType> _data;
};

} // namespace Ecs

#include "SparseArray.tpp"