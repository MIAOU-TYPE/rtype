/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Password
*/

#pragma once

namespace Auth
{
    [[nodiscard]] constexpr int hexNibble(const char c) noexcept
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (const auto lc = static_cast<char>(std::tolower(static_cast<unsigned char>(c))); lc >= 'a' && lc <= 'f')
            return 10 + (lc - 'a');
        return -1;
    }

    template <size_t N>
    [[nodiscard]] bool hexDecodeFixed(const std::string_view hex, std::array<unsigned char, N> &out) noexcept
    {
        if (hex.size() != N * 2)
            return false;
        for (size_t i = 0; i < N; ++i) {
            const int hi = hexNibble(hex.at(i * 2));
            const int lo = hexNibble(hex.at(i * 2 + 1));
            if (hi < 0 || lo < 0)
                return false;
            out[i] = static_cast<unsigned char>(hi << 4 | lo);
        }
        return true;
    }

    template <size_t N>
    [[nodiscard]] std::string hexEncodeFixed(const std::array<unsigned char, N> &buf)
    {
        static constexpr char kHex[] = "0123456789abcdef";

        std::string out;
        out.resize(N * 2);
        for (size_t i = 0; i < N; ++i) {
            out.at(i * 2 + 0) = kHex[buf[i] >> 4 & 0x0F];
            out.at(i * 2 + 1) = kHex[buf[i] >> 0 & 0x0F];
        }
        return out;
    }
} // namespace Auth
