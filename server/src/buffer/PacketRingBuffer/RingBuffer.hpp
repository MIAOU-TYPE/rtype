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
     * @tparam data The type of data to be stored in the buffer.
     */
    template <typename data>
    class RingBuffer : public IBuffer<data> {
      public:
        /**
         * @brief Constructor to initialize the ring buffer with a given capacity.
         * @param capacity The maximum number of elements the buffer can hold.
         */
        RingBuffer(size_t capacity);

        /**
         * @brief Destructor to clean up resources.
         */
        ~RingBuffer() override = default;

        /**
         * @brief Push data into the buffer.
         * @param data The data to be pushed into the buffer.
         * @return true if the data was successfully pushed, false otherwise.
         */
        bool push(const data &data) noexcept override;

        /**
         * @brief Pop data from the buffer.
         * @param data Reference to store the popped data.
         * @return true if data was successfully popped, false otherwise.
         */
        bool pop(data &data) noexcept override;

        /**
         * @brief Get the top data from the buffer without removing it.
         * @return The top data in the buffer.
         * @throws BufferError if the buffer is empty.
         */
        const data &top() noexcept override;

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

      private:
        size_t _capacity = 0;
        size_t _writeIndex = 0;
        size_t _readIndex = 0;
        size_t _count = 0;
        std::unique_ptr<data[]> _buffer;
    };
} // namespace Buffer
