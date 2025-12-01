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
        if (entityCount != packet.entities.size() || entityCount != packet.entityCount) {
            throw SerializerError("Entity count does not match the size of the entities vector");
        }
        size_t totalSize = sizeof(PacketSnapshot) + entityCount * sizeof(SnapshotEntity);
        std::vector<uint8_t> buffer(totalSize);
        std::memcpy(buffer.data(), &packet, sizeof(PacketSnapshot));
        if (entityCount > 0) {
            std::memcpy(
                buffer.data() + sizeof(PacketSnapshot), packet.entities.data(), entityCount * sizeof(SnapshotEntity));
        }

        return buffer;
    }
} // namespace Network