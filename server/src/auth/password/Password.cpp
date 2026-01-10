/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Password
*/

#include "Password.hpp"

#include <charconv>
#include <cstdint>

namespace Auth
{
    namespace
    {
        constexpr uint64_t kN = 1ull << 15;
        constexpr uint64_t kr = 8;
        constexpr uint64_t kp = 1;
        constexpr uint64_t kMaxMem = 64ull * 1024ull * 1024ull;

        constexpr std::size_t kSaltLen = 16;
        constexpr std::size_t kDkLen = 32;

        constexpr uint64_t kMinN = 1ull << 14;
        constexpr uint64_t kMaxN = 1ull << 18;
        constexpr uint64_t kMinR = 1;
        constexpr uint64_t kMaxR = 16;
        constexpr uint64_t kMinP = 1;
        constexpr uint64_t kMaxP = 4;

        [[nodiscard]] bool isPowerOfTwo(const uint64_t x) noexcept
        {
            return x != 0 && ((x & (x - 1)) == 0);
        }

        [[nodiscard]] bool parseU64(const std::string_view s, uint64_t &out) noexcept
        {
            if (s.empty())
                return false;
            uint64_t v = 0;
            const auto *b = s.data();
            const auto *e = s.data() + s.size();
            if (const auto [ptr, ec] = std::from_chars(b, e, v); ec != std::errc{} || ptr != e)
                return false;
            out = v;
            return true;
        }

        [[nodiscard]] bool validateParams(const uint64_t N, const uint64_t r, const uint64_t p) noexcept
        {
            if (!isPowerOfTwo(N) || N < kMinN || N > kMaxN || r < kMinR || r > kMaxR || p < kMinP || p > kMaxP
                || (r != 0 && N > UINT64_MAX / r) || r * N > UINT64_MAX / 128)
                return false;
            const uint64_t mem = 128 * r * N;
            return mem <= kMaxMem;
        }

        [[nodiscard]] bool split6(std::string_view s, std::string_view &a, std::string_view &b, std::string_view &c,
            std::string_view &d, std::string_view &e, std::string_view &f) noexcept
        {
            for (const std::array out{&a, &b, &c, &d, &e}; auto *v : out) {
                const auto pos = s.find('$');
                if (pos == std::string_view::npos)
                    return false;
                *v = s.substr(0, pos);
                s.remove_prefix(pos + 1);
            }
            f = s;
            return !f.empty();
        }

        [[nodiscard]] bool scryptDerive(const std::string_view password, const unsigned char *salt, std::size_t saltLen,
            const uint64_t N, const uint64_t r, const uint64_t p, unsigned char *out, std::size_t outLen) noexcept
        {
            return EVP_PBE_scrypt(password.data(), password.size(), salt, saltLen, N, r, p, kMaxMem, out, outLen) == 1;
        }

        std::string encode(uint64_t N, uint64_t r, uint64_t p, const std::array<unsigned char, kSaltLen> &salt,
            const std::array<unsigned char, kDkLen> &dk)
        {
            std::string out;
            out.reserve(7 + 3 * 20 + (kSaltLen + kDkLen) * 2 + 5);
            out += "scrypt$";
            out += std::to_string(N);
            out += "$";
            out += std::to_string(r);
            out += "$";
            out += std::to_string(p);
            out += "$";
            out += hexEncodeFixed(salt);
            out += "$";
            out += hexEncodeFixed(dk);
            return out;
        }
    } // namespace

    std::string hashPassword(const std::string_view password)
    {
        std::array<unsigned char, kSaltLen> salt{};
        if (RAND_bytes(salt.data(), salt.size()) != 1)
            throw PasswordError("{RAND_bytes} failed");
        std::array<unsigned char, kDkLen> dk{};
        if (!scryptDerive(password, salt.data(), salt.size(), kN, kr, kp, dk.data(), dk.size()))
            throw PasswordError("{EVP_PBE_scrypt} failed");
        return encode(kN, kr, kp, salt, dk);
    }

    bool verifyPassword(const std::string_view password, const std::string_view encoded) noexcept
    {
        std::string_view sN, sr, sp, saltHex, dkHex;
        if (std::string_view tag; !split6(encoded, tag, sN, sr, sp, saltHex, dkHex) || tag != "scrypt")
            return false;
        uint64_t N = 0, r = 0, p = 0;
        if (!parseU64(sN, N) || !parseU64(sr, r) || !parseU64(sp, p) || !validateParams(N, r, p))
            return false;
        std::array<unsigned char, kSaltLen> salt{};
        std::array<unsigned char, kDkLen> dkStored{};
        if (!hexDecodeFixed(saltHex, salt) || !hexDecodeFixed(dkHex, dkStored))
            return false;
        std::array<unsigned char, kDkLen> dk{};
        if (!scryptDerive(password, salt.data(), salt.size(), N, r, p, dk.data(), dk.size()))
            return false;
        return CRYPTO_memcmp(dk.data(), dkStored.data(), dk.size()) == 0;
    }
} // namespace Auth
