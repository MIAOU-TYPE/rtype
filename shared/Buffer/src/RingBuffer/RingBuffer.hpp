/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** RingBuffer
*/

#pragma once
#include <cstddef>
#include <memory>
#include "IBuffer.hpp"

namespace Buffer
{
    /**
     * @class RingBuffer
     * @brief A ring buffer implementation for storing data.
     *
     * This class implements a ring buffer that can store and retrieve data of type `data`.
     *
     * @tparam Tdata The type of data to be stored in the buffer.
     */
    template <typename Tdata>
    class RingBuffer final : public IBuffer<Tdata> {
      public:
        /**
         * @brief Constructor to initialize the ring buffer with a given capacity.
         * @param capacity The maximum number of elements the buffer can hold.
         */
        explicit RingBuffer(size_t capacity);

        /**
         * @brief Destructor to clean up resources.
         */
        ~RingBuffer() override = default;

        /**
         * @brief Push data into the buffer.
         * @param data The data to be pushed into the buffer.
         * @return true if the data was successfully pushed, false otherwise.
         */
        bool push(const Tdata &data) noexcept override;

        /**
         * @brief Pop data from the buffer.
         * @param data Reference to store the popped data.
         * @return true if data was successfully popped, false otherwise.
         */
        bool pop(Tdata &data) noexcept override;

        /**
         * @brief Get the top data from the buffer without removing it.
         * @return The top data in the buffer.
         * @throws BufferError if the buffer is empty.
         */
        const Tdata &top() override;

        /**
         * @brief Clear the buffer.
         */
        void clear() noexcept override;

        /**
         * @brief Check if the buffer is empty.
         * @return true if the buffer is empty, false otherwise.
         */
        bool isEmpty() const noexcept override;

        /**
         * @brief Check if the buffer is full.
         * @return true if the buffer is full, false otherwise.
         */
        bool isFull() const noexcept override;

        /**
         * @brief Get the current size of the buffer.
         * @return The number of elements currently in the buffer.
         */
        size_t readable() const noexcept override;

        /**
         * @brief Get the remaining capacity of the buffer.
         * @return The number of elements that can still be added to the buffer.
         */
        size_t writable() const noexcept override;

        /**
         * @brief Write multiple elements to the buffer.
         * @param data Pointer to the data to be written.
         * @param count Number of elements to write.
         * @return true if the data was successfully written, false otherwise.
         */
        bool write(const Tdata *data, size_t count) noexcept override;

        /**
         * @brief Read multiple elements from the buffer.
         * @param data Pointer to store the read data.
         * @param count Number of elements to read.
         * @return true if the data was successfully read, false otherwise.
         */
        bool read(Tdata *data, size_t count) noexcept override;

        /**
         * @brief Peek at multiple elements from the buffer without removing them.
         * @param data Pointer to store the peeked data.
         * @param count Number of elements to peek.
         * @return true if the data was successfully peeked, false otherwise.
         */
        bool peek(Tdata *data, size_t count) const noexcept override;

      private:
        size_t _capacity = 0;                  ///> Maximum number of elements in the buffer
        size_t _writeIndex = 0;                ///> Index to write the next element
        size_t _readIndex = 0;                 ///> Index to read the next element
        size_t _count = 0;                     ///> Current number of elements in the buffer
        std::unique_ptr<Tdata[]> _buffer = {}; ///> The buffer to store elements
    };
} // namespace Buffer

#include "RingBuffer.tpp"
