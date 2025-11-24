/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacket
*/

#include "UDPPacket.hpp"

using namespace Net;

UDPPacket::UDPPacket()
{
    std::memset(&_addr, 0, sizeof(_addr));
    std::memset(_buffer.data(), 0, _buffer.size());
}

uint8_t *UDPPacket::buffer()
{
    return _buffer.data();
}

const uint8_t *UDPPacket::buffer() const
{
    return _buffer.data();
}

sockaddr_in *UDPPacket::address()
{
    return &_addr;
}

const sockaddr_in *UDPPacket::address() const
{
    return &_addr;
}

size_t UDPPacket::size() const
{
    return _size;
}

void UDPPacket::setSize(size_t s)
{
    _size = s;
}
