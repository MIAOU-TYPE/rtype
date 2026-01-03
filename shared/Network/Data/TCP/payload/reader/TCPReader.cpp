/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPReader
*/

#include "TCPReader.hpp"

namespace Net::TCP
{
    Reader::Reader(const uint8_t *p, const size_t n) : _cursor(p), _end(p + n)
    {
    }

    uint8_t Reader::u8() noexcept
    {
        if (!need(1))
            return 0;
        return *_cursor++;
    }

    uint16_t Reader::u16() noexcept
    {
        if (!need(2))
            return 0;
        uint16_t v;
        std::memcpy(&v, _cursor, 2);
        _cursor += 2;
        return ntohs(v);
    }

    uint32_t Reader::u32() noexcept
    {
        if (!need(4))
            return 0;
        uint32_t v;
        std::memcpy(&v, _cursor, 4);
        _cursor += 4;
        return ntohl(v);
    }

    std::string Reader::str16() noexcept
    {
        if (!need(2))
            return "";
        const uint16_t len = u16();
        if (!need(len))
            return "";
        std::string s(reinterpret_cast<const char *>(_cursor), reinterpret_cast<const char *>(_cursor + len));
        _cursor += len;
        return s;
    }

    size_t Reader::remaining() const noexcept
    {
        return static_cast<size_t>(_end - _cursor);
    }

    bool Reader::need(const size_t n) const noexcept
    {
        if (std::cmp_less(_end - _cursor, n))
            return false;
        return true;
    }
} // namespace Net::TCP