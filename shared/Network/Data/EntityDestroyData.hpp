/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntityDestroyData
*/

#pragma once
#include <cstdint>
#include "HeaderData.hpp"

/**
 * @brief High-level ECS representation of an entity to be destroyed.
 */
struct EntityDestroy {
    size_t id; ///> Entity ID
};