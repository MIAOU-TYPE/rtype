/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPayload
*/

#include "TCPPayload.hpp"

namespace Net::TCP
{
    std::vector<uint8_t> buildPayload(const uint8_t type, const uint32_t requestId, const std::vector<uint8_t> &body)
    {
        Writer w;
        w.u8(type);
        w.u32(requestId);
        auto out = w.bytes();
        out.insert(out.end(), body.begin(), body.end());
        return out;
    }

    Header parseHeader(const uint8_t *payload, const size_t n)
    {
        Reader r(payload, n);
        return Header{r.u8(), r.u32()};
    }

    Reader bodyReader(const uint8_t *payload, const size_t n)
    {
        Reader r(payload, n);
        (void) r.u8();
        (void) r.u32();
        return r;
    }
} // namespace Net::TCP
