/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SnapEntityData
*/

#pragma once
#include <cstdint>
#include <string>
#include "HeaderData.hpp"

/**
 * @brief High-level ECS snapshot representation.
 */
struct SnapshotEntity {
    size_t entity;
    float x;
    float y;
    std::string sprite;
};

#pragma pack(push, 1)

struct SnapshotBatchHeader {
    HeaderData header;
    uint16_t count;
};

#pragma pack(pop)

#pragma pack(push, 1)

struct SnapshotEntityData {
    uint64_t entity;
    uint32_t x;
    uint32_t y;
    uint8_t spriteId;
};

#pragma pack(pop)