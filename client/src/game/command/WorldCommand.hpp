/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** WorldCommand
*/

#pragma once
#include <variant>
#include <vector>
#include "SnapEntityData.hpp"

namespace World
{
    /**
     * @struct WorldCommand
     * @brief Represents a command sent to or from the game world.
     */
    struct WorldCommand {
        enum class Type {
            Accept,   ///> Accept connection
            Reject,   ///> Reject connection
            Pong,     ///> Pong response
            GameOver, ///> Game over notification
            Snapshot, ///> Snapshot of the world state
        };

        Type type;                                                         ///> Type of the command
        std::variant<std::monostate, std::vector<SnapshotEntity>> payload; ///> Command payload
    };
} // namespace World