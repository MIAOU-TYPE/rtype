#ifndef SPARSE_ARRAY_HPP
#define SPARSE_ARRAY_HPP

#include <vector>
#include <optional>
#include <stdexcept>

namespace ecs {

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
    using const_reference_type = const value_type&;
    using container_t = std::vector<value_type>;
    using size_type = typename container_t::size_type;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

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
    const_reference_type operator[](size_type idx) const;

    /**
     * @brief Get iterator to the beginning
     */
    iterator begin();

    /**
     * @brief Get const iterator to the beginning
     */
    const_iterator begin() const;

    /**
     * @brief Get const iterator to the beginning
     */
    const_iterator cbegin() const;

    /**
     * @brief Get iterator to the end
     */
    iterator end();

    /**
     * @brief Get const iterator to the end
     */
    const_iterator end() const;

    /**
     * @brief Get const iterator to the end
     */
    const_iterator cend() const;

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

    /**
     * @brief Get the index of a value (for iteration purposes)
     * @param value Reference to the value
     * @return Index of the value in the array
     */
    size_type get_index(const_reference_type value) const;

private:
    container_t _data;
};

} // namespace ecs

#include "SparseArray.tpp"

#endif // SPARSE_ARRAY_HPP
