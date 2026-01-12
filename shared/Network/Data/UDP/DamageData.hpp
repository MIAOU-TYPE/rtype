/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DamageData
*/

#pragma once
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @brief Structure representing damage data sent from server to clients.
 * @details This structure contains the header information, entity ID,
 * the amount of damage dealt, and whether the damage killed the entity.
 */
struct DamageData {
    HeaderData header;   ///> The packet header containing type, version, and size.
    uint32_t id = 0;     ///> The unique identifier of the entity receiving damage.
    uint16_t amount = 0; ///> The amount of damage to apply.
    uint8_t wasKilled = 0; ///> True (1) if this damage killed the entity, false (0) otherwise
};

#pragma pack(pop)

static_assert(sizeof(DamageData) == sizeof(HeaderData) + 4 + 2 + 1, "DamageData layout mismatch");