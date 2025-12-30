/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacket
*/

#pragma once
#include <cstring>
#include <vector>
#include "IPacket.hpp"

namespace Net
{
    class TCPPacket final : public IPacket {
      public:
        explicit TCPPacket(size_t cap = 64 * 1024) : _buf(cap)
        {
        }

        uint8_t *buffer() override
        {
            return _buf.data();
        }

        const uint8_t *buffer() const override
        {
            return _buf.data();
        }

        size_t size() const override
        {
            return _size;
        }

        void setSize(size_t s) override
        {
            _size = (s <= _buf.size() ? s : _buf.size());
        }

        const sockaddr_in *address() const override
        {
            return &_addr;
        }

        void setAddress(const sockaddr_in &addr) override
        {
            _addr = addr;
        }

        std::shared_ptr<IPacket> clone() const override
        {
            auto p = std::make_shared<TCPPacket>(_buf.size());
            p->_addr = _addr;
            p->_size = _size;
            std::memcpy(p->_buf.data(), _buf.data(), _size);
            return p;
        }

        std::shared_ptr<IPacket> newPacket() const override
        {
            return std::make_shared<TCPPacket>(_buf.size());
        }

        size_t capacity() const noexcept override
        {
            return _buf.size();
        }

      private:
        sockaddr_in _addr{};
        std::vector<uint8_t> _buf;
        size_t _size = 0;
    };
} // namespace Net
