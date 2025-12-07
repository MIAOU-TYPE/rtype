/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntityPacket
*/

#pragma once
#include <cstdint>
#include "HeaderData.hpp"
#pragma pack(push, 1)

struct EntityCreateData {
    HeaderData header;
    uint64_t id = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t sprite = 0;
};

#pragma pack(pop)
