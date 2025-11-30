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

        if (sizeof(T) == 0) {
            throw SerializerError("Cannot serialize: Packet size is zero");
        }
        
        std::vector<uint8_t> buffer(sizeof(T));
        std::memcpy(buffer.data(), &packet, sizeof(T));
        return buffer;
    }
} // namespace Network