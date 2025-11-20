#ifndef SPARSE_ARRAY_HPP
#define SPARSE_ARRAY_HPP

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
    using value_type = std::optional<Component>;
    using reference_type = value_type&;
    using size_type = std::size_t;

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
    reference_type operator[](size_type idx);

    /**
     * @brief Access element at given index (const version)
     * @param idx Index of the element
     * @return Const reference to the optional component
     */
    const value_type& operator[](size_type idx) const;

    /**
     * @brief Get the size of the array
     */
    size_type size() const;

    /**
     * @brief Insert a component at the given position
     * @param pos Position to insert at
     * @param component Component to insert
     * @return Reference to the inserted component
     */
    reference_type insert_at(size_type pos, const Component& component);

    /**
     * @brief Insert a component at the given position (move version)
     * @param pos Position to insert at
     * @param component Component to insert
     * @return Reference to the inserted component
     */
    reference_type insert_at(size_type pos, Component&& component);

    /**
     * @brief Emplace a component at the given position
     * @tparam Params Parameter pack for component constructor
     * @param pos Position to emplace at
     * @param params Parameters to forward to component constructor
     * @return Reference to the emplaced component
     */
    template<typename... Params>
    reference_type emplace_at(size_type pos, Params&&... params);

    /**
     * @brief Erase the component at the given position
     * @param pos Position to erase
     */
    void erase(size_type pos);

private:
    std::vector<value_type> _data;
};

} // namespace Ecs

#include "SparseArray.tpp"

#endif // SPARSE_ARRAY_HPP
