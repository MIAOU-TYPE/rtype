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
 * @brief Structure representing player input data with flags.
 * @details This structure contains the header information and a flags byte
 * where each bit represents a different input action (up, down, left, right, shoot).
 */
struct DestroyData {
    HeaderData header; ///> The packet header containing type, version, and size.
    uint32_t id; ///> The ID of the entity to be destroyed.
};

#pragma pack(pop)

static_assert(sizeof(DestroyData) == sizeof(HeaderData) + 4, "DestroyData layout mismatch");
