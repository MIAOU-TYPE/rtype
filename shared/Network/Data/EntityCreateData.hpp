/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntityPacket
*/

#pragma once
#include <cstdint>
#include <string>
#include "HeaderData.hpp"
#pragma pack(push, 1)

/**
 * @brief Structure representing the data for creating a new entity.
 * @details This structure contains the header information, entity ID,
 * position (x, y), and sprite identifier.
 */
struct EntityCreateData {
    HeaderData header;     ///> The packet header containing type, version, and size.
    uint64_t id = 0;       ///> The unique identifier of the entity to be created.
    uint32_t x = 0;        ///> The x-coordinate of the entity's position.
    uint32_t y = 0;        ///> The y-coordinate of the entity's position.
    uint32_t spriteId = 0; ///> The identifier for the entity's sprite/visual representation.
};

#pragma pack(pop)

/**
 * @brief High-level ECS representation of an entity to be created.
 */
struct EntityCreate {
    size_t id;    ///> Entity ID
    float x;      ///> X position
    float y;      ///> Y position
    int spriteId; ///> Sprite identifier
};