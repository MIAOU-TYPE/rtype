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
    struct NetworkIdentity {
        uint64_t id;
        bool authoritative = false;
    };
}