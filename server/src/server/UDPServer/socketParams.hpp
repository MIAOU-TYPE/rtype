/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** socketParams
*/

#pragma once
#include <cstdint>

#if defined(_WIN32)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>

namespace net
{
    using family_t = ADDRESS_FAMILY;
}
#else
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>

namespace net
{
    using family_t = sa_family_t;
}
#endif

/**
 * @namespace net
 * @brief Contains network-related structures and configurations.
 */
namespace net
{
    /**
     * @struct SocketConfig
     * @brief Configuration parameters for creating a socket.
     */
    struct SocketConfig {
        family_t family = AF_INET;
        int type = SOCK_DGRAM;
        int proto = IPPROTO_UDP;
    };

    /**
     * @struct SocketOptions
     * @brief Options for configuring socket behavior.
     */
    struct SocketOptions {
        int level = SOL_SOCKET;
        int optname = SO_REUSEADDR;
        int optval = 1;
    };
} // namespace net
