/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ConnectPacket
*/

#pragma once
#include "HeaderPacket.hpp"

#pragma pack(push, 1)

struct ConnectPacket {
    HeaderPacket header;
    uint32_t clientId;
};

#pragma pack(pop)
