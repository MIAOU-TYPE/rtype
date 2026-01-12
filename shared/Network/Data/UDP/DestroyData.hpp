/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DestroyData
*/

#pragma once

#include <cstddef>
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @brief Structure representing entity destruction data.
 * @details This structure contains the header information, entity ID, and whether it was killed.
 */
struct DestroyData {
    HeaderData header; ///> The packet header containing type, version, and size.
    uint32_t id;       ///> The ID of the entity to be destroyed.
    uint8_t wasKilled; ///> True (1) if entity was killed, false (0) otherwise
};

#pragma pack(pop)

static_assert(sizeof(DestroyData) == sizeof(HeaderData) + 5, "DestroyData layout mismatch");
