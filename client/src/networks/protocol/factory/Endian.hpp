/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Endian
*/

#pragma once

#include <cstring>

#if defined(__APPLE__)

    #include <libkern/OSByteOrder.h>

inline uint64_t htobe64(uint64_t x)
{
    return OSSwapHostToBigInt64(x);
}

inline uint64_t be64toh(uint64_t x)
{
    return OSSwapBigToHostInt64(x);
}

#elif defined(__linux__)
    #include <arpa/inet.h>
    #include <endian.h>

#elif defined(_WIN32)
    #include <intrin.h>
    #include <windows.h>
    #include <winsock2.h>

static inline uint64_t htobe64(uint64_t x)
{
    return _byteswap_uint64(x);
}

static inline uint64_t be64toh(uint64_t x)
{
    return _byteswap_uint64(x);
}
#endif

inline uint32_t htonf(float f)
{
    uint32_t u;
    std::memcpy(&u, &f, sizeof(float));
    u = htonl(u);
    return u;
}

inline float ntohf(uint32_t u)
{
    u = ntohl(u);
    float f;
    std::memcpy(&f, &u, sizeof(uint32_t));
    return f;
}
