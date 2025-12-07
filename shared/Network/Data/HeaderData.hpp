/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** HeaderData
*/

#pragma once
#include <cstdint>

#pragma pack(push, 1)

struct HeaderData {
    uint8_t type = 0;
    uint8_t version = 0;
    uint16_t size = 0;
};

#pragma pack(pop)
