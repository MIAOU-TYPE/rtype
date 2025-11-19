/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** RingBuffer
*/

#include "RingBuffer.hpp"

namespace Buffer
{
    template <typename data>
    RingBuffer<data>::RingBuffer(size_t capacity)
        : _capacity(capacity), _readIndex(0), _writeIndex(0), _count(0), _buffer(std::make_unique<data[]>(capacity))
    {
    }

    template <typename data>
    bool RingBuffer<data>::push(const data &data) noexcept
    {
        if (isFull())
            return false;
        _buffer[_writeIndex] = data;
        _writeIndex = (_writeIndex + 1) % _capacity;
        _count++;
        return true;
    }

    template <typename data>
    bool RingBuffer<data>::pop(data &data) noexcept
    {
        if (isEmpty())
            return false;
        data = _buffer[_readIndex];
        _readIndex = (_readIndex + 1) % _capacity;
        _count--;
        return true;
    }

    template <typename data>
    const data &RingBuffer<data>::top() noexcept
    {
        if (isEmpty())
            throw BufferError("{RingBuffer::top} Buffer is empty");
        return _buffer[_readIndex];
    }

    template <typename data>
    void RingBuffer<data>::clear() noexcept
    {
        _readIndex = 0;
        _writeIndex = 0;
        _count = 0;
    }

    template <typename data>
    bool RingBuffer<data>::isEmpty() const noexcept
    {
        return _count == 0;
    }

    template <typename data>
    bool RingBuffer<data>::isFull() const noexcept
    {
        return _count == _capacity;
    }
} // namespace Buffer
