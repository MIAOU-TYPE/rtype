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

/**
 * @brief High-level ECS representation of an entity to be created.
 */
struct EntityCreate {
    size_t id;             ///> Entity ID
    float x;               ///> X position
    float y;               ///> Y position
    unsigned int spriteId; ///> Sprite identifier
};