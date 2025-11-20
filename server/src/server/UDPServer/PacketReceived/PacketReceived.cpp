/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketReceived
*/

#include "PacketReceived.hpp"

using namespace net;

PacketReceived::PacketReceived()
{
    std::memset(&_addr, 0, sizeof(_addr));
    std::memset(_buffer, 0, sizeof(_buffer));
}

uint8_t *PacketReceived::buffer()
{
    return _buffer;
}

const uint8_t *PacketReceived::buffer() const
{
    return _buffer;
}

sockaddr_in *PacketReceived::address()
{
    return &_addr;
}

size_t PacketReceived::size() const
{
    return _size;
}
void PacketReceived::setSize(size_t s)
{
    _size = s;
}
