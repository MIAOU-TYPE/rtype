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
    uint32_t dx;
    uint32_t dy;
    uint8_t shooting;
};

#pragma pack(pop)
