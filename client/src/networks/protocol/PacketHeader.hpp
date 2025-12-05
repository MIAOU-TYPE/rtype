/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHeader
*/

#pragma once

#include <cstdint>
#include <vector>

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
        uint8_t type;    ///> Type of the packet (see PacketTypes.hpp for definitions)
        uint8_t version; ///> Protocol version
        uint16_t size;   ///> Total size of the packet in bytes (network byte order)
    };

#pragma pack(pop)
} // namespace Network