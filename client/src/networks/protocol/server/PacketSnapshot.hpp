/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketSnapshot
*/

#pragma once

#include "PacketHeader.hpp"

namespace Network
{
/**
 * @brief Base packet structure for connect, ping & disconnect packets
 *
 * This packet serves as a base for simple packets that only require a header
 * without additional data.
 */
#pragma pack(push, 1)

    struct SnapshotEntity {
        uint32_t id;     ///< Entity ID
        float x;         ///< X position
        float y;         ///< Y position
        float vx;        ///< X velocity
        float vy;        ///< Y velocity
        uint16_t sprite; ///< Sprite ID (network byte order)
    };

#pragma pack(pop)

/**
 * @brief Snapshot packet structure
 *
 * This packet is used by the server to send the current state of all entities to clients.
 */
#pragma pack(push, 1)

    struct PacketSnapshot {
        PacketHeader header;       ///< Common packet header
        uint32_t tick;             ///< Server frame number (network byte order)
        uint16_t entityCount;      ///< Number of entities (network byte order)
        SnapshotEntity entities[]; ///< Array of entity snapshots
    };

#pragma pack(pop)
} // namespace Network