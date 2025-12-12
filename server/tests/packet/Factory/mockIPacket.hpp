/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** mockIPacket
*/

#pragma once
#include <cstring>
#include <memory>
#include <vector>
#include "IPacket.hpp"

class MockPacket : public Net::IPacket {
  public:
    explicit MockPacket(size_t cap = 512) : _capacity(cap), _size(0)
    {
        _buffer.resize(cap);
    }

    std::shared_ptr<Net::IPacket> New() const override
    {
        return std::make_shared<MockPacket>(_capacity);
    }

    std::shared_ptr<Net::IPacket> clone() const override
    {
        auto pkt = std::make_shared<MockPacket>(_capacity);
        pkt->setSize(_size);
        std::memcpy(pkt->buffer(), _buffer.data(), _size);
        pkt->setAddress(_addr);
        return pkt;
    }

    uint8_t *buffer() override
    {
        return _buffer.data();
    }

    const uint8_t *buffer() const override
    {
        return _buffer.data();
    }

    size_t size() const override
    {
        return _size;
    }

    void setSize(size_t s) override
    {
        _size = s;
    }

    const sockaddr_in *address() const override
    {
        return &_addr;
    }

    void setAddress(const sockaddr_in &addr) override
    {
        _addr = addr;
    }

    size_t capacity() const noexcept override
    {
        return _capacity;
    }

  private:
    std::vector<uint8_t> _buffer;
    size_t _capacity;
    size_t _size;
    sockaddr_in _addr{};
};
