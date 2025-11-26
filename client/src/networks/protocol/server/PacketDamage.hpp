/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketDamage
*/

#pragma once

#include <PacketHeader.hpp>

namespace Network
{
/**
 * @brief Damage packet structure
 *
 * This packet is used by the server to inform clients about damage dealt to entities.
 */
#pragma pack(push, 1)

    struct PacketDamage {
        PacketHeader header; ///< Common packet header
        uint32_t id;         ///< Entity ID (network byte order)
        uint16_t amount;     ///< Damage amount (network byte order)
    };

#pragma pack(pop)
} // namespace Network
