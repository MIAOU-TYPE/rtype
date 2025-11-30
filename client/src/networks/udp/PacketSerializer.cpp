/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketSerializer
*/

#include "PacketSerializer.hpp"

namespace Network
{
    std::vector<uint8_t> PacketSerializer::serialize(const PacketSnapshot& packet, uint16_t entityCount)
    {
        size_t totalSize = sizeof(PacketSnapshot) + entityCount * sizeof(SnapshotEntity);
        std::vector<uint8_t> buffer(totalSize);
        std::memcpy(buffer.data(), &packet, sizeof(PacketSnapshot));
        std::memcpy(buffer.data() + sizeof(PacketSnapshot), packet.entities, entityCount * sizeof(SnapshotEntity));
        return buffer;
    }
} // namespace Network