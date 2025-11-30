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
    static std::vector<uint8_t> serialize(const T& packet) {
        std::vector<uint8_t> buffer(sizeof(T));
        std::memcpy(buffer.data(), &packet, sizeof(T));
        return buffer;
    }
} // namespace Network