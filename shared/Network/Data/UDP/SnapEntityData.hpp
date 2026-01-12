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
    HeaderData header;   ///> Common header data
    uint16_t count;      ///> Number of entities in the batch
    uint32_t serverTick; ///> Server tick at which the snapshot was taken
    uint16_t chunkIndex; ///> Index of the current chunk
    uint16_t chunkCount; ///> Total number of chunks
};

#pragma pack(pop)
static_assert(sizeof(SnapshotBatchHeader) == 22, "SnapshotBatchHeader layout mismatch");

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

#pragma pack(push, 1)

/**
 * @brief Header for a compressed batch of snapshot entities.
 */
struct SnapshotCompressedHeader {
    HeaderData header; ///> Common header data
    uint16_t count; ///> Number of entities in the batch
    uint32_t serverTick; ///> Server tick at which the snapshot was taken
    uint16_t chunkIndex; ///> Index of the current chunk
    uint16_t chunkCount; ///> Total number of chunks

    uint16_t rawSize; ///> Size of the uncompressed data
    uint16_t compSize; ///> Size of the compressed data
};

#pragma pack(pop)

static_assert(sizeof(SnapshotCompressedHeader) == 26, "SnapshotCompressedHeader layout mismatch");

static_assert(sizeof(SnapshotEntityData) == 9, "SnapshotEntityData layout mismatch");
