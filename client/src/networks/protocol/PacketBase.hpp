/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketBase
*/

#pragma once

#include <PacketHeader.hpp>

namespace Network
{
/**
 * @brief Base packet structure for connect, ping & disconnect packets
 *
 * This packet serves as a base for simple packets that only require a header
 * without additional data.
 */
#pragma pack(push, 1)

    struct PacketBase {
        PacketHeader header; ///< Common packet header
    };

#pragma pack(pop)
} // namespace Network