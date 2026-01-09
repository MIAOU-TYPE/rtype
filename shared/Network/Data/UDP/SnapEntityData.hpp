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
    size_t id;             ///> Entity ID
    float x;               ///> X position
    float y;               ///> Y position
    unsigned int spriteId; ///> Sprite identifier
};

#pragma pack(push, 1)

/**
 * @brief Header for a batch of snapshot entities.
 */
struct SnapshotBatchHeader {
    HeaderData header; ///> Common header data
    uint32_t sequence; ///> Sequence number of the snapshot
    uint16_t count;    ///> Number of entities in the batch
};

#pragma pack(pop)
static_assert(sizeof(SnapshotBatchHeader) == 14, "SnapshotBatchHeader layout mismatch");

#pragma pack(push, 1)

/**
 * @brief Serialized snapshot entity data.
 */
struct SnapshotEntityData {
    uint32_t id;      ///> Entity ID
    uint16_t x;       ///> X position
    uint16_t y;       ///> Y position
    uint8_t spriteId; ///> Sprite identifier
};

#pragma pack(pop)

static_assert(sizeof(SnapshotEntityData) == 9, "SnapshotEntityData layout mismatch");
