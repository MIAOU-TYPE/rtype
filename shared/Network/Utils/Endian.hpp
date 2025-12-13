/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Endian
*/
#pragma once

#include <cstring>

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
