/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntityDestroyData
*/

#pragma once
#include <cstdint>
#include "HeaderData.hpp"
#pragma pack(push, 1)

/**
 * @brief Structure representing the data for destroying an entity.
 * @details This structure contains the header information and the ID of the entity to be destroyed.
 */
struct EntityDestroyData {
    HeaderData header; ///> The packet header containing type, version, and size.
    uint64_t id = 0;   ///> The unique identifier of the entity to be destroyed.
};

#pragma pack(pop)
