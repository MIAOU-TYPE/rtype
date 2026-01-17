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
     * @struct DestroyInfo
     * @brief Information about entity destruction.
     */
    struct DestroyInfo {
        size_t entityId; ///> ID of the entity to be destroyed
        bool wasKilled;  ///> True if entity was killed by player (play sound), false otherwise
    };

    /**
     * @struct DamageInfo
     * @brief Information about entity damage.
     */
    struct DamageInfo {
        size_t targetId; ///> ID of the entity that received damage
        bool wasKilled;  ///> True if entity was killed by this damage (play sound), false otherwise
    };

    /**
     * @struct ScoreUpdate
     * @brief Information about a score update.
     */
    struct ScoreUpdate {
        uint32_t playerId; ///> ID of the player whose score was updated
        uint32_t score;    ///> The updated score
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
            Damage,   ///> Damage event (for hit sound)
            Destroy,  ///> Destroy an entity
            Score,    ///> Score update
        };

        Type type; ///> Type of the command
        std::variant<std::monostate, SnapshotBatch, DestroyInfo, DamageInfo, uint32_t, ScoreUpdate>
            payload; ///> Command payload (uint32_t used for Score)
    };
} // namespace World
