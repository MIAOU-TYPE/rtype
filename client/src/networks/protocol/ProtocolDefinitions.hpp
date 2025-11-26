/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ProtocolDefinitions
*/

#pragma once

#include <cstdint>

namespace Network
{

/**
 * @brief Header structure for all network packets
 *
 * This structure is packed to ensure consistent memory layout across platforms.
 * All multi-byte fields use network byte order (big-endian).
 */
#pragma pack(push, 1)

    struct PacketHeader {
        uint8_t type;    ///< Type of the packet (see PacketType enum)
        uint8_t version; ///< Protocol version
        uint16_t size;   ///< Total size of the packet in bytes (network byte order)
    };

#pragma pack(pop)

/**
 * @brief Connection request packet sent from client to server
 *
 * This packet is sent when a client wants to connect to the game server.
 */
#pragma pack(push, 1)

    struct PacketConnect {
        PacketHeader header; ///< Common packet header
        uint32_t clientId;   ///< Unique client identifier (network byte order)
    };

#pragma pack(pop)

/**
 * @brief Player input packet sent from client to server
 *
 * Contains player movement and action inputs for a specific entity.
 */
#pragma pack(push, 1)

    struct PacketInput {
        PacketHeader header; ///< Common packet header
        uint32_t entity;     ///< Entity ID to control (network byte order)
        float dx;            ///< Horizontal movement axis (-1.0 to 1.0)
        float dy;            ///< Vertical movement axis (-1.0 to 1.0)
        uint8_t shooting;    ///< Shooting action flag (0 = not shooting, 1 = shooting)
    };

#pragma pack(pop)

/**
 * @brief Ping packet sent from client to server
 *
 * Used to measure network latency and keep the connection alive.
 */
#pragma pack(push, 1)

    struct PacketPing {
        PacketHeader header; ///< Common packet header
    };

#pragma pack(pop)

/**
 * @brief Disconnection notification packet sent from client to server
 *
 * Sent when a client gracefully disconnects from the game server.
 */
#pragma pack(push, 1)

    struct PacketDisconnect {
        PacketHeader header; ///< Common packet header
    };

#pragma pack(pop)

} // namespace Network