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
 * and the amount of damage dealt.
 */
struct DamageData {
    HeaderData header;   ///> The packet header containing type, version, and size.
    uint32_t id = 0;     ///> The unique identifier of the entity receiving damage.
    uint16_t amount = 0; ///> The amount of damage to apply.
};

#pragma pack(pop)

static_assert(sizeof(DamageData) == 10, "DamageData layout mismatch");