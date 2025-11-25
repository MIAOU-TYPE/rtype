/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AClientPacket
*/

#include "AClientPacket.hpp"

namespace Network
{
    AClientPacket::AClientPacket() : _size(0)
    {
        std::memset(_buffer, 0, MAX_SIZE);
        std::memset(&_addr, 0, sizeof(_addr));
    }

    uint8_t *AClientPacket::buffer()
    {
        return _buffer;
    }

    const uint8_t *AClientPacket::buffer() const
    {
        return _buffer;
    }

    sockaddr_in *AClientPacket::address()
    {
        return &_addr;
    }

    const sockaddr_in *AClientPacket::address() const
    {
        return &_addr;
    }

    size_t AClientPacket::size() const
    {
        return _size;
    }

    void AClientPacket::setSize(size_t s)
    {
        _size = (s > MAX_SIZE) ? MAX_SIZE : s;
    }

    std::vector<uint8_t> AClientPacket::toVector() const
    {
        return std::vector<uint8_t>(_buffer, _buffer + _size);
    }
} // namespace Network
