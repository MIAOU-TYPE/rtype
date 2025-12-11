/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CommandBuffer
*/

#pragma once
#include <mutex>
#include <queue>

/**
 * @namespace Command
 * @brief Namespace for command buffer related classes and functions
 */
namespace Command
{
    /**
     * @class CommandBuffer
     * @brief A thread-safe command buffer for storing and retrieving commands of type T
     *
     * @tparam T The type of commands to be stored in the buffer
     */
    template <typename T>
    class CommandBuffer {
      public:
        /**
         * @brief Default constructor
         */
        CommandBuffer() = default;

        /**
         * @brief Default destructor
         */
        ~CommandBuffer() = default;

        /**
         * @brief Push a command into the buffer
         *
         * @param cmd The command to be pushed
         * @return true if the command was successfully pushed, false otherwise
         */
        bool push(const T &cmd) noexcept;

        /**
         * @brief Pop a command from the buffer
         *
         * @param out Reference to store the popped command
         * @return true if a command was successfully popped, false if the buffer was empty
         */
        bool pop(T &out) noexcept;

        /**
         * @brief Clear all commands from the buffer
         */
        void clear() noexcept;

        /**
         * @brief Check if the buffer is empty
         *
         * @return true if the buffer is empty, false otherwise
         */
        bool empty() const noexcept;

      private:
        mutable std::mutex _mutex; ///< Mutex for thread-safe access
        std::queue<T> _queue;      ///< Queue to store commands
    };
} // namespace Command

#include "CommandBuffer.tpp"
