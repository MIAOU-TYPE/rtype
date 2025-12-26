/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** WorldCommandBuffer
*/

#pragma once
#include <mutex>
#include <queue>
#include <variant>
#include <vector>
#include "SnapEntityData.hpp"

namespace Engine
{
    /**
     * @struct WorldCommand
     * @brief Represents a command to be processed by the game world.
     */
    struct WorldCommand {
        enum class Type {
            Accept,   ///> Accept connection
            Reject,   ///> Reject connection
            Pong,     ///> Pong response
            GameOver, ///> Game over notification
            Snapshot, ///> Snapshot of the world state
            Score
        };

        Type type;                                                                   ///> Type of the command
        std::variant<std::monostate, std::vector<SnapshotEntity>, uint32_t> payload; ///> Command payload
    };

    /**
     * @class WorldCommandBuffer
     * @brief Thread-safe buffer for storing and retrieving world commands.
     */
    class WorldCommandBuffer {
      public:
        /**
         * @brief Pushes a command into the buffer.
         * @param cmd The command to be pushed.
         */
        void push(const WorldCommand &cmd);

        /**
         * @brief Tries to pop a command from the buffer.
         * @param out Reference to store the popped command.
         * @return True if a command was popped, false if the buffer was empty.
         */
        [[nodiscard]] bool tryPop(WorldCommand &out);

      private:
        std::mutex _mutex;               ///> Mutex for thread safety
        std::queue<WorldCommand> _queue; ///> Queue of world commands
    };
} // namespace Engine