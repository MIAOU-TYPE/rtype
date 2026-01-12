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
     * @struct DamageInfo
     * @brief Information about damage dealt to an entity.
     */
    struct DamageInfo {
        uint32_t targetId; ///> ID of the entity that received damage
        uint16_t amount;   ///> Amount of damage dealt
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
            Damage,   ///> Damage event
        };

        Type type;                                                                         ///> Type of the command
        std::variant<std::monostate, std::vector<SnapshotEntity>, DamageInfo> payload; ///> Command payload
    };
} // namespace World