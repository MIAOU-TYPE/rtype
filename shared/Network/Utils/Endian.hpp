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
        #include <cstdint>

static inline std::uint64_t ntohll(std::uint64_t x)
{
    return be64toh(x);
}

static inline std::uint64_t htonll(std::uint64_t x)
{
    return htobe64(x);
}

#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <intrin.h>
    #include <windows.h>
    #include <winsock2.h>

inline uint64_t htobe64(uint64_t x)
{
    return _byteswap_uint64(x);
}

inline uint64_t be64toh(uint64_t x)
{
    return _byteswap_uint64(x);
}

#endif

#ifndef _WIN32

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

#endif
