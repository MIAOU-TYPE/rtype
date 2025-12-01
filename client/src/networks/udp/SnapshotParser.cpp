/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SnapshotParser
*/

#include "SnapshotParser.hpp"

namespace Network
{
    PacketSnapshot SnapshotParser::parse(const uint8_t *buffer, size_t bufferSize)
    {
        const size_t minSize = sizeof(PacketHeader) + sizeof(uint32_t) + sizeof(uint16_t);
        if (bufferSize < minSize) {
            throw ParserError("Buffer too small for snapshot packet");
        }
        PacketSnapshot snapshot;
        size_t offset = 0;
        std::memcpy(&snapshot.header, buffer + offset, sizeof(PacketHeader));
        offset += sizeof(PacketHeader);
        if (snapshot.header.type != SNAPSHOT) {
            throw ParserError("Invalid packet type for snapshot");
        }
        uint32_t networkTick;
        std::memcpy(&networkTick, buffer + offset, sizeof(uint32_t));
        snapshot.tick = ntohl(networkTick);
        offset += sizeof(uint32_t);
        snapshot.entityCount = parseEntityCount(buffer + offset);
        offset += sizeof(uint16_t);
        const size_t expectedSize = minSize + (snapshot.entityCount * sizeof(SnapshotEntity));
        if (bufferSize < expectedSize) {
            throw ParserError("Buffer too small for declared entity count");
        }
        snapshot.entities.reserve(snapshot.entityCount);
        for (uint16_t i = 0; i < snapshot.entityCount; ++i) {
            SnapshotEntity entity = parseEntity(buffer + offset);
            snapshot.entities.push_back(entity);
            offset += sizeof(SnapshotEntity);
        }
        return snapshot;
    }

    PacketSnapshot SnapshotParser::parse(const std::vector<uint8_t> &buffer)
    {
        return parse(buffer.data(), buffer.size());
    }

    uint16_t SnapshotParser::parseEntityCount(const uint8_t *buffer)
    {
        uint16_t networkCount;
        std::memcpy(&networkCount, buffer, sizeof(uint16_t));
        return ntohs(networkCount);
    }

    SnapshotEntity SnapshotParser::parseEntity(const uint8_t *buffer)
    {
        SnapshotEntity entity;
        size_t offset = 0;

        // Read networkId (uint32_t - convert from network byte order)
        uint32_t networkId;
        std::memcpy(&networkId, buffer + offset, sizeof(uint32_t));
        entity.id = ntohl(networkId);
        offset += sizeof(uint32_t);

        // Read position (x, y) - floats don't need byte order conversion
        std::memcpy(&entity.x, buffer + offset, sizeof(float));
        offset += sizeof(float);
        std::memcpy(&entity.y, buffer + offset, sizeof(float));
        offset += sizeof(float);

        // Read velocity (vx, vy) - floats don't need byte order conversion
        std::memcpy(&entity.vx, buffer + offset, sizeof(float));
        offset += sizeof(float);
        std::memcpy(&entity.vy, buffer + offset, sizeof(float));
        offset += sizeof(float);

        // Read spriteId (uint16_t - convert from network byte order)
        uint16_t networkSprite;
        std::memcpy(&networkSprite, buffer + offset, sizeof(uint16_t));
        entity.sprite = ntohs(networkSprite);

        return entity;
    }

} // namespace Network