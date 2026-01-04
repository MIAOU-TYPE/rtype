/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPayload
*/

#include "TCPWriter.hpp"

namespace Net::TCP
{
    void Writer::u8(const uint8_t v)
    {
        _buf.push_back(v);
    }

    void Writer::u16(uint16_t v)
    {
        v = htons(v);
        push(&v, 2);
    }

    void Writer::u32(uint32_t v)
    {
        v = htonl(v);
        push(&v, 4);
    }

    void Writer::str16(const std::string_view s)
    {
        if (s.size() > 0xFFFFu)
            throw WriterError("{Net::TCP::Writer::str16} String size exceeds maximum length of 65535 characters");
        u16(static_cast<uint16_t>(s.size()));
        _buf.insert(_buf.end(), s.begin(), s.end());
    }

    std::vector<uint8_t> &Writer::bytes() noexcept
    {
        return _buf;
    }

    const std::vector<uint8_t> &Writer::bytes() const noexcept
    {
        return _buf;
    }

    void Writer::push(const void *p, const size_t n)
    {
        const auto *c = static_cast<const uint8_t *>(p);
        _buf.insert(_buf.end(), c, c + n);
    }
} // namespace Net::TCP