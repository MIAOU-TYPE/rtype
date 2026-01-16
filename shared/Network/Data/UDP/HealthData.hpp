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
 * @brief Structure representing health data in a UDP packet.
 * This structure contains the header and health information of an entity.
 * It is packed to ensure no padding bytes are added.
 */
struct HealthData {
    HeaderData header;    ///> The packet header containing type, version, and size.
    uint16_t currentLife; ///> The current life of the entity.
    uint16_t maxLife;     ///> The maximum life of the entity.
};

#pragma pack(pop)

static_assert(sizeof(HealthData) == sizeof(HeaderData) + 2 + 2, "HealthData layout mismatch");
