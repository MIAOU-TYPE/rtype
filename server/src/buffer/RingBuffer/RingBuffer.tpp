/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** RingBuffer
*/

namespace Buffer
{
    template <typename Tdata>
    RingBuffer<Tdata>::RingBuffer(size_t capacity)
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
} // namespace Buffer
