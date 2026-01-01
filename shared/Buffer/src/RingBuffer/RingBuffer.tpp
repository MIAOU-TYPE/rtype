/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** RingBuffer
*/

#pragma once

namespace Buffer
{
    template <typename Tdata>
    RingBuffer<Tdata>::RingBuffer(const size_t capacity)
        : _capacity(capacity), _buffer(std::make_unique<Tdata[]>(capacity))
    {
    }

    template <typename Tdata>
    bool RingBuffer<Tdata>::push(const Tdata &data) noexcept
    {
        if (isFull())
            return false;
        _buffer[_writeIndex] = data;
        _writeIndex = (_writeIndex + 1) % _capacity;
        _count++;
        return true;
    }

    template <typename Tdata>
    bool RingBuffer<Tdata>::pop(Tdata &data) noexcept
    {
        if (isEmpty())
            return false;
        data = _buffer[_readIndex];
        _readIndex = (_readIndex + 1) % _capacity;
        _count--;
        return true;
    }

    template <typename Tdata>
    const Tdata &RingBuffer<Tdata>::top()
    {
        if (isEmpty())
            throw BufferError("{RingBuffer::top} Buffer is empty");
        return _buffer[_readIndex];
    }

    template <typename Tdata>
    void RingBuffer<Tdata>::clear() noexcept
    {
        _readIndex = 0;
        _writeIndex = 0;
        _count = 0;
    }

    template <typename Tdata>
    bool RingBuffer<Tdata>::isEmpty() const noexcept
    {
        return _count == 0;
    }

    template <typename Tdata>
    bool RingBuffer<Tdata>::isFull() const noexcept
    {
        return _count == _capacity;
    }

    template <typename Tdata>
    size_t RingBuffer<Tdata>::readable() const noexcept
    {
        return _count;
    }

    template <typename Tdata>
    size_t RingBuffer<Tdata>::writable() const noexcept
    {
        return _capacity - _count;
    }

    template <typename Tdata>
    bool RingBuffer<Tdata>::write(const Tdata *data, const size_t count) noexcept
    {
        if (count > writable())
            return false;

        for (size_t i = 0; i < count; ++i) {
            _buffer[_writeIndex] = data[i];
            _writeIndex = (_writeIndex + 1) % _capacity;
        }
        _count += count;
        return true;
    }

    template <typename Tdata>
    bool RingBuffer<Tdata>::peek(Tdata *data, const size_t count) const noexcept
    {
        if (count > readable())
            return false;

        size_t idx = _readIndex;
        for (size_t i = 0; i < count; ++i) {
            data[i] = _buffer[idx];
            idx = (idx + 1) % _capacity;
        }
        return true;
    }

    template <typename Tdata>
    bool RingBuffer<Tdata>::read(Tdata *data, const size_t count) noexcept
    {
        if (count > readable())
            return false;

        for (size_t i = 0; i < count; ++i) {
            if (data)
                data[i] = _buffer[_readIndex];
            _readIndex = (_readIndex + 1) % _capacity;
        }
        _count -= count;
        return true;
    }

} // namespace Buffer
