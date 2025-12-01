/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketSerializer
*/

#include "PacketSerializer.hpp"

namespace Network
{
    std::vector<uint8_t> PacketSerializer::serialize(const PacketSnapshot &packet, uint16_t entityCount)
    {
        if (entityCount != packet.entities.size()) {
            throw SerializerError("Entity count does not match the size of the entities vector");
        }
        size_t totalSize =
            sizeof(PacketHeader) + sizeof(uint32_t) + sizeof(uint16_t) + entityCount * sizeof(SnapshotEntity);
        std::vector<uint8_t> buffer(totalSize);
        size_t offset = 0;
        std::memcpy(buffer.data() + offset, &packet.header, sizeof(PacketHeader));
        offset += sizeof(PacketHeader);
        uint32_t netTick = htonl(packet.tick);
        std::memcpy(buffer.data() + offset, &netTick, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        uint16_t netCount = htons(entityCount);
        std::memcpy(buffer.data() + offset, &netCount, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        for (const auto &e : packet.entities) {
            uint32_t netId = htonl(e.id);
            std::memcpy(buffer.data() + offset, &netId, sizeof(uint32_t));
            offset += sizeof(uint32_t);
            std::memcpy(buffer.data() + offset, &e.x, sizeof(float));
            offset += sizeof(float);
            std::memcpy(buffer.data() + offset, &e.y, sizeof(float));
            offset += sizeof(float);
            std::memcpy(buffer.data() + offset, &e.vx, sizeof(float));
            offset += sizeof(float);
            std::memcpy(buffer.data() + offset, &e.vy, sizeof(float));
            offset += sizeof(float);
            uint16_t netSprite = htons(e.sprite);
            std::memcpy(buffer.data() + offset, &netSprite, sizeof(uint16_t));
            offset += sizeof(uint16_t);
        }
        return buffer;
    }

} // namespace Network