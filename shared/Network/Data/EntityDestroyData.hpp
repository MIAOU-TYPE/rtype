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

struct EntityDestroyData {
    HeaderData header;
    uint64_t id = 0;
};

#pragma pack(pop)
