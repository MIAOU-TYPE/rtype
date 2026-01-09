/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DestroyData
*/

#pragma once
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @brief Structure representing destroy data sent from server to clients.
 * @details This structure contains the header information and the entity ID
 * of the entity to be destroyed.
 */
struct DestroyData {
    HeaderData header;   ///> The packet header containing type, version, and size.
    uint32_t id = 0;     ///> The unique identifier of the entity to be destroyed.
};

#pragma pack(pop)

static_assert(sizeof(DestroyData) == sizeof(HeaderData) + 4, "DestroyData layout mismatch");