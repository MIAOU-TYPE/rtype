/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketDestroy
*/

#pragma once

#include "PacketHeader.hpp"

namespace Network
{
/**
 * @brief Entity destruction packet structure
 *
 * This packet is used by the server to inform clients about entity removals.
 */
#pragma pack(push, 1)

    struct PacketEntityDestroy {
        PacketHeader header;
        uint32_t id;
    };

#pragma pack(pop)
} // namespace Network
