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
     * @struct SnapshotBatch
     * @brief Represents a batch of snapshot entities at a specific server tick.
     */
    struct SnapshotBatch {
        uint32_t serverTick;                  ///> Server tick of the snapshot
        std::vector<SnapshotEntity> entities; ///> List of snapshot entities
    };

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
            Destroy,  ///> Destroy an entity
            Score,    ///> Score update
        };

        Type type;                                                             ///> Type of the command
        std::variant<std::monostate, SnapshotBatch, size_t, uint32_t> payload; ///> Command payload
    };
} // namespace World
