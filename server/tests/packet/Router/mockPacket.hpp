/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** mockPacket
*/

#pragma once
#include "IPacket.hpp"
#include <vector>
#include <memory>

class MockPacket : public Net::IPacket {
public:
    MockPacket(size_t cap = 256) : _capacity(cap)
    {
        _buffer.resize(cap);
        _size = 0;
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

    void setAddress(const sockaddr_in &a) override
    {
        _addr = a;
    }

    std::shared_ptr<Net::IPacket> clone() const override
    {
        return std::make_shared<MockPacket>(_capacity);
    }

    size_t capacity() const noexcept override
    {
        return _capacity;
    }

private:
    std::vector<uint8_t> _buffer;
    sockaddr_in _addr{};
    size_t _size;
    size_t _capacity;
};
