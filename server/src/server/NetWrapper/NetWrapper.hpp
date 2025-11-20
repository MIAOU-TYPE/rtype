/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetWrapper
*/

#pragma once
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
using socketHandle = SOCKET;
constexpr socketHandle kInvalidSocket = INVALID_SOCKET;
using recvfrom_return_t = int;
using sendto_return_t = int;
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
using socketHandle = int;
constexpr socketHandle kInvalidSocket = -1;

using recvfrom_return_t = ssize_t;
using sendto_return_t = ssize_t;
#endif

/**
 * @namespace Net
 * @brief Namespace for network-related classes and functions.
 */
namespace Net
{
    /**
     * @class NetWrapper
     * @brief A wrapper class for network socket operations.
     */
    class NetWrapper {
      public:
        /**
         * @brief Creates a socket.
         * @param domain The communication domain (e.g., AF_INET).
         * @param type The communication type (e.g., SOCK_STREAM).
         * @param protocol The protocol to be used (e.g., IPPROTO_TCP).
         * @return A handle to the created socket, or kInvalidSocket on failure.
         */
        static socketHandle socket(int domain, int type, int protocol);

        /**
         * @brief Closes a socket.
         * @param s The handle of the socket to be closed.
         */
        static void closeSocket(socketHandle s);

        /**
         * @brief Sets options on a socket.
         * @param s The handle of the socket.
         * @param level The level at which the option is defined.
         * @param optName The option name.
         * @param optVal A pointer to the option value.
         * @param optLen The size of the option value.
         * @return 0 on success, or -1 on failure.
         */
        static int setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen);

        /**
         * @brief Receives data from a socket.
         * @param sockFd The handle of the socket.
         * @param buf A pointer to the buffer where received data will be stored.
         * @param len The length of the buffer.
         * @param flags Flags for the receive operation.
         * @param src_addr A pointer to a sockaddr structure to store the source address.
         * @param addrLen A pointer to a socklen_t variable that specifies the size of src_addr.
         * @return The number of bytes received, or -1 on failure.
         */
        static recvfrom_return_t recvFrom(
            socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrLen);

        /**
         * @brief Sends data to a specific address using a socket.
         * @param sockFd The handle of the socket.
         * @param buf A pointer to the buffer containing the data to be sent.
         * @param len The length of the data in the buffer.
         * @param flags Flags for the send operation.
         * @param destAddr A pointer to a sockaddr structure containing the destination address.
         * @param addrLen The size of the destination address structure.
         * @return The number of bytes sent, or -1 on failure.
         */
        static sendto_return_t sendTo(socketHandle sockFd, const void *buf, size_t len, int flags,
            const struct sockaddr *destAddr, socklen_t addrLen);
    };
} // namespace Net
