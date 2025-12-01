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
        if (packet.entities.empty() && entityCount > 0) {
            throw SerializerError("Cannot serialize: PacketSnapshot entities vector is empty");
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