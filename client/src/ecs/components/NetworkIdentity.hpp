/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkIdentity
*/

#pragma once
#include <cstdint>

namespace Ecs
{
    /**
     * @struct NetworkIdentity
     * @brief Component that holds network identity information for an entity.
     */
    struct NetworkIdentity {
        uint64_t id;                     ///> Unique network ID
        bool authoritative = false;      ///> Is this entity authoritative
    };
} // namespace Ecs