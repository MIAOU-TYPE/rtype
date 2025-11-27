/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketConnect
*/

#pragma once

#include "PacketHeader.hpp"

namespace Network
{
/**
 * @brief Connect packet structure
 *
 * This packet is used by clients to request a connection to the server.
 * It contains a header and a unique client identifier.
 */
#pragma pack(push, 1)

    struct PacketConnect {
        PacketHeader header; ///> Common packet header
        uint32_t clientId;   ///> Unique client identifier (network byte order)
    };

#pragma pack(pop)
} // namespace Network