/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** HealthData
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
struct HealthData {
    HeaderData header;    ///> The packet header containing type, version, and size.
    uint16_t currentLife; ///> The current life of the entity.
    uint16_t maxLife;     ///> The maximum life of the entity.
};

#pragma pack(pop)

static_assert(sizeof(HealthData) == sizeof(HeaderData) + 2 + 2, "HealthData layout mismatch");
