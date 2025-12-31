/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacket
*/

#include "TCPPacket.hpp"

namespace Net
{
    TCPPacket::TCPPacket(const size_t capacity) : _buf(capacity)
    {
    }

    uint8_t *TCPPacket::buffer()
    {
        return _buf.data();
    }

    const uint8_t *TCPPacket::buffer() const
    {
        return _buf.data();
    }

    size_t TCPPacket::size() const
    {
        return _size;
    }

    void TCPPacket::setSize(const size_t s)
    {
        _size = (s <= _buf.size() ? s : _buf.size());
    }

    const sockaddr_in *TCPPacket::address() const
    {
        return &_addr;
    }

    void TCPPacket::setAddress(const sockaddr_in &addr)
    {
        _addr = addr;
    }

    std::shared_ptr<IPacket> TCPPacket::clone() const
    {
        auto p = std::make_shared<TCPPacket>(_buf.size());
        p->_addr = _addr;
        p->_size = _size;
        std::memcpy(p->_buf.data(), _buf.data(), _size);
        return p;
    }

    std::shared_ptr<IPacket> TCPPacket::newPacket() const
    {
        return std::make_shared<TCPPacket>(_buf.size());
    }

    size_t TCPPacket::capacity() const noexcept
    {
        return _buf.size();
    }
} // namespace Net