/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketInput
*/

#pragma once

#include "PacketHeader.hpp"

namespace Network
{

/**
 * @brief Player input packet sent from client to server
 *
 * Contains player movement and action inputs for a specific entity.
 */
#pragma pack(push, 1)

    struct PacketInput {
        PacketHeader header; ///> Common packet header
        uint32_t entity;     ///> Entity ID to control (network byte order)
        uint32_t dx;         ///> Horizontal movement axis (-1.0 to 1.0)
        uint32_t dy;         ///> Vertical movement axis (-1.0 to 1.0)
        uint8_t shooting;    ///> Shooting action flag (0 = not shooting, 1 = shooting)
    };

#pragma pack(pop)
} // namespace Network