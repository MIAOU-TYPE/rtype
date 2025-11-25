/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SocketConfig
*/

#pragma once
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

/**
 * @namespace Network
 * @brief Contains network-related structures and configurations for the client.
 */
namespace Network
{
    /**
     * @struct SocketConfig
     * @brief Configuration parameters for creating a client socket.
     */
    struct SocketConfig {
        int family = AF_INET;      ///< Address family (AF_INET for IPv4, AF_INET6 for IPv6)
        int type = SOCK_DGRAM;     ///< Socket type (SOCK_DGRAM for UDP, SOCK_STREAM for TCP)
        int protocol = IPPROTO_UDP; ///< Protocol (IPPROTO_UDP for UDP, IPPROTO_TCP for TCP)
    };

    /**
     * @struct SocketOptions
     * @brief Options for configuring socket behavior.
     */
    struct SocketOptions {
        int level = SOL_SOCKET;      ///< Socket level (SOL_SOCKET for socket-level options)
        int optName = SO_REUSEADDR;  ///< Option name (SO_REUSEADDR, SO_RCVTIMEO, etc.)
        int optVal = 1;              ///< Option value
    };

    /**
     * @struct TimeoutConfig
     * @brief Configuration for socket timeouts.
     */
    struct TimeoutConfig {
        long seconds = 0;      ///< Timeout in seconds
        long microseconds = 0; ///< Timeout in microseconds
    };
} // namespace Network
