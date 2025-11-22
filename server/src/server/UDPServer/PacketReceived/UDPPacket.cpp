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
    std::memset(_buffer, 0, sizeof(_buffer));
}

uint8_t *UDPPacket::buffer()
{
    return _buffer;
}

const uint8_t *UDPPacket::buffer() const
{
    return _buffer;
}

size_t UDPPacket::size() const
{
    return _size;
}

void UDPPacket::setSize(size_t s)
{
    _size = s;
}

const sockaddr_in *UDPPacket::address() const
{
    return &_addr;
}

std::shared_ptr<IServerPacket> UDPPacket::clone() const
{
    return std::make_shared<UDPPacket>();
}
