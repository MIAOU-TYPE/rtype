/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DefaultData
*/

#pragma once
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @brief Structure representing a default data packet.
 * @details This structure contains only the header information.
 */
struct DefaultData {
    HeaderData header; ///> The packet header containing type, version, and size.
};

#pragma pack(pop)

static_assert(sizeof(DefaultData) == 4, "DefaultData layout mismatch");