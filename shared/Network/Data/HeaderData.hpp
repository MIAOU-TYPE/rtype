/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** HeaderData
*/

#pragma once
#include <cstdint>

#pragma pack(push, 1)

/**
 * @brief Structure representing the header of a network packet.
 * @details This structure contains the type, version, and size of the packet.
 */
struct HeaderData {
    uint8_t type = 0;    ///> The type of the packet (e.g., CONNECT, DISCONNECT, INPUT, etc.).
    uint8_t version = 0; ///> The version of the packet format.
    uint16_t size = 0;   ///> The size of the entire packet, including the header.
};

#pragma pack(pop)
