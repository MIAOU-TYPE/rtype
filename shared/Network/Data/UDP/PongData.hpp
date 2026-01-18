/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PongData
*/

#pragma once
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @brief Structure representing a pong data packet.
 * @details This structure contains only the header information.
 */
struct PongData {
    HeaderData header;      ///> The packet header containing type, version, and size.
    uint64_t pongTimestamp; ///> The timestamp sent in the pong response.
};

#pragma pack(pop)

static_assert(sizeof(PongData) == sizeof(HeaderData) + 8, "PongData layout mismatch");