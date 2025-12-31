/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Endian
*/
#pragma once

#include <cstring>

/**
 * @brief Read a 32-bit unsigned integer from a big-endian byte array.
 * @param p Pointer to the byte array.
 * @return The 32-bit unsigned integer.
 */
inline uint32_t readU32BE(const uint8_t *p)
{
    return (static_cast<uint32_t>(p[0]) << 24) | (static_cast<uint32_t>(p[1]) << 16)
        | (static_cast<uint32_t>(p[2]) << 8) | static_cast<uint32_t>(p[3]);
}

/**
 * @brief Write a 32-bit unsigned integer to a big-endian byte array.
 * @param p Pointer to the byte array.
 * @param v The 32-bit unsigned integer to write.
 */
inline void writeU32BE(uint8_t *p, uint32_t v)
{
    p[0] = static_cast<uint8_t>((v >> 24) & 0xFF);
    p[1] = static_cast<uint8_t>((v >> 16) & 0xFF);
    p[2] = static_cast<uint8_t>((v >> 8) & 0xFF);
    p[3] = static_cast<uint8_t>(v & 0xFF);
}

struct AddressKey {
    uint32_t ip;   ///> The IP address in network byte order.
    uint16_t port; ///> The port number in network byte order.

    /**
     * @brief Equality operator for AddressKey.
     */
    bool operator==(const AddressKey &other) const noexcept
    {
        return ip == other.ip && port == other.port;
    }
};

/**
 * @brief Hash function for AddressKey to be used in unordered_map.
 */
struct AddressKeyHash {
    /**
     * @brief Hash function implementation.
     * @param k The AddressKey to hash.
     * @return The computed hash value.
     */
    std::size_t operator()(const AddressKey &k) const noexcept
    {
        return std::hash<uint64_t>{}((static_cast<uint64_t>(k.ip) << 16) | k.port);
    }
};

#if defined(__APPLE__)

    #include <libkern/OSByteOrder.h>

/**
 * @brief Convert 64-bit integer from host to big-endian byte order.
 * @param x The 64-bit integer in host byte order.
 * @return The 64-bit integer in big-endian byte order.
 */
inline uint64_t htobe64(uint64_t x)
{
    return OSSwapHostToBigInt64(x);
}

/**
 * @brief Convert 64-bit integer from big-endian to host byte order.
 * @param x The 64-bit integer in big-endian byte order.
 * @return The 64-bit integer in host byte order.
 */
inline uint64_t be64toh(uint64_t x)
{
    return OSSwapBigToHostInt64(x);
}

#elif defined(__linux__)
    #include <arpa/inet.h>
    #include <cstdint>
    #include <endian.h>

/**
 * @brief Convert 64-bit integer from network to host byte order.
 * @param x The 64-bit integer in network byte order.
 * @return The 64-bit integer in host byte order.
 */
static inline std::uint64_t ntohll(std::uint64_t x)
{
    return be64toh(x);
}

/**
 * @brief Convert 64-bit integer from host to network byte order.
 * @param x The 64-bit integer in host byte order.
 * @return The 64-bit integer in network byte order.
 */
static inline std::uint64_t htonll(std::uint64_t x)
{
    return htobe64(x);
}

#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <intrin.h>
    #define NOMINMAX
    #include <windows.h>
    #include <winsock2.h>

/**
 * @brief Convert 64-bit integer from host to big-endian byte order.
 * @param x The 64-bit integer in host byte order.
 * @return The 64-bit integer in big-endian byte order.
 */
inline uint64_t htobe64(uint64_t x)
{
    return _byteswap_uint64(x);
}

/**
 * @brief Convert 64-bit integer from big-endian to host byte order.
 * @param x The 64-bit integer in big-endian byte order.
 * @return The 64-bit integer in host byte order.
 */
inline uint64_t be64toh(uint64_t x)
{
    return _byteswap_uint64(x);
}

#endif

#ifndef _WIN32

/**
 * @brief Convert float from host to network byte order.
 * @param f The float in host byte order.
 * @return The float in network byte order as uint32_t.
 */
inline uint32_t htonf(float f)
{
    uint32_t u;
    std::memcpy(&u, &f, sizeof(float));
    u = htonl(u);
    return u;
}

/**
 * @brief Convert float from network to host byte order.
 * @param u The float in network byte order as uint32_t.
 * @return The float in host byte order.
 */
inline float ntohf(uint32_t u)
{
    u = ntohl(u);
    float f;
    std::memcpy(&f, &u, sizeof(uint32_t));
    return f;
}

#endif
