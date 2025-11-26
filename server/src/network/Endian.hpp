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

#elif defined(_WIN32)

    #include <windows.h>
    #include <winsock2.h>

inline uint64_t htobe64(uint64_t x)
{
    uint32_t high = htonl((uint32_t) (x >> 32));
    uint32_t low = htonl((uint32_t) (x & 0xFFFFFFFFULL));
    return (uint64_t) low << 32 | high;
}

inline uint64_t be64toh(uint64_t x)
{
    uint32_t high = ntohl((uint32_t) (x >> 32));
    uint32_t low = ntohl((uint32_t) (x & 0xFFFFFFFFULL));
    return (uint64_t) low << 32 | high;
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
