/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** IBuffer
*/

#pragma once
#include <stdexcept>
#include <string>
#include <utility>

/**
 * @namespace Buffer
 * @brief Namespace for buffer-related classes and interfaces.
 */
namespace Buffer
{
    /**
     * @class BufferError
     * @brief Exception class for buffer-related errors.
     */
    class BufferError : public std::exception {
      public:
        /**
         * @brief Constructor with error message.
         * @param message The error message.
         */
        explicit BufferError(std::string message) : _message(std::move(message))
        {
        }

        /**
         * @brief Get the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; //> The error message
    };

    /**
     * @interface IBuffer
     * @brief Interface for a generic buffer.
     *
     * This interface defines the basic operations for a buffer that can store and retrieve data of type `data`.
     *
     * @tparam data The type of data to be stored in the buffer.
     */
    template <typename Tdata>
    class IBuffer {
      public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IBuffer() = default;

        /**
         * @brief Push data into the buffer.
         * @param data The data to be pushed into the buffer.
         * @return true if the data was successfully pushed, false otherwise.
         */
        virtual bool push(const Tdata &data) noexcept = 0;

        /**
         * @brief Pop data from the buffer.
         * @param data Reference to store the popped data.
         * @return true if data was successfully popped, false otherwise.
         */
        virtual bool pop(Tdata &data) noexcept = 0;

        /**
         * @brief Get the top data from the buffer without removing it.
         * @return The top data in the buffer.
         * @throws BufferError if the buffer is empty.
         */
        virtual const Tdata &top() = 0;

        /**
         * @brief Clear the buffer.
         */
        virtual void clear() noexcept = 0;

        /**
         * @brief Check if the buffer is empty.
         * @return true if the buffer is empty, false otherwise.
         */
        virtual bool isEmpty() const noexcept = 0;

        /**
         * @brief Check if the buffer is full.
         * @return true if the buffer is full, false otherwise.
         */
        virtual bool isFull() const noexcept = 0;
    };
} // namespace Buffer
