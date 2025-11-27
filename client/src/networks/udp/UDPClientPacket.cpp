/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPClientPacket
*/

#include "UDPClientPacket.hpp"

namespace Network
{
    UDPClientPacket::UDPClientPacket() = default;

    uint8_t *UDPClientPacket::buffer()
    {
        return _buffer;
    }

    const uint8_t *UDPClientPacket::buffer() const
    {
        return _buffer;
    }

    sockaddr_in *UDPClientPacket::address()
    {
        return &_addr;
    }

    const sockaddr_in *UDPClientPacket::address() const
    {
        return &_addr;
    }

    size_t UDPClientPacket::size() const
    {
        return _size;
    }

    void UDPClientPacket::setSize(size_t s)
    {
        _size = (s > MAX_SIZE) ? MAX_SIZE : s;
    }

    std::vector<uint8_t> UDPClientPacket::toVector() const
    {
        return std::vector<uint8_t>(_buffer, _buffer + _size);
    }
} // namespace Network
