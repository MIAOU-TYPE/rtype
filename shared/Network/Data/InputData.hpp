/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketInput
*/

#pragma once

#include "HeaderData.hpp"

#pragma pack(push, 1)

struct InputData {
    HeaderData header;
    uint32_t entity;
    int8_t dx;
    int8_t dy;
    uint8_t shooting;
};

#pragma pack(pop)
