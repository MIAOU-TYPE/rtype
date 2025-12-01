/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketSerializer
*/

#include "PacketSerializer.hpp"

namespace Network
{
    template<typename T>
    std::vector<uint8_t> PacketSerializer::serialize(const T& packet) {

        if (packet.header.type == SNAPSHOT) {
            throw SerializerError("Use the specialized serialize method for PacketSnapshot");
        }
        std::vector<uint8_t> buffer(sizeof(T));
        std::memcpy(buffer.data(), &packet, sizeof(T));
        return buffer;
    }
} // namespace Network