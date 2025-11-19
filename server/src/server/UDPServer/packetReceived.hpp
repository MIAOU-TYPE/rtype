/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** packetReceived
*/

#pragma once
#include <arpa/inet.h>
#include <cstring>
#include <iomanip>
#include <iostream>

class PacketReceived {
  public:
    PacketReceived() : _size(0)
    {
        std::memset(&_addr, 0, sizeof(_addr));
        std::memset(_buffer, 0, sizeof(_buffer));
    }

    uint8_t *buffer()
    {
        return _buffer;
    }

    const uint8_t *buffer() const
    {
        return _buffer;
    }

    sockaddr_in *address()
    {
        return &_addr;
    }
    const sockaddr_in &address() const
    {
        return _addr;
    }

    size_t size() const
    {
        return _size;
    }
    void setSize(size_t s)
    {
        _size = s;
    }

  private:
    uint8_t _buffer[2048];
    size_t _size;
    sockaddr_in _addr;
};

// Pretty print for debugging
inline std::ostream &operator<<(std::ostream &os, const PacketReceived &pkt)
{
    std::ostream &out = os;

    for (size_t i = 0; i < pkt.size(); ++i) {
        uint8_t c = pkt.buffer()[i];
        if (std::isprint(c))
            os << static_cast<char>(c);
        else
            os << ".";
    }

    os << std::endl;
    return out;
}
