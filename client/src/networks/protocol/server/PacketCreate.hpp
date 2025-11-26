/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketCreate
*/

#pragma once

#include <PacketHeader.hpp>

namespace Network
{
/**
 * @brief Entity creation packet structure
 *
 * This packet is used by the server to inform clients about new entities.
 */
#pragma pack(push, 1)

    struct PacketEntityCreate {
        PacketHeader header; ///< Common packet header
        uint32_t id;         ///< Entity ID (network byte order)
        uint32_t x;          ///< X position (network byte order)
        uint32_t y;          ///< Y position (network byte order)
        uint16_t sprite;     ///< Sprite ID (network byte order)
    };

#pragma pack(pop)
} // namespace Network