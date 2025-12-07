/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetWrapper
*/

#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include "DLLoader.hpp"

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
 * @brief Namespace for networking-related classes and functions.
 */
namespace Net
{
    /**
     * @class NetWrapperError
     * @brief Exception class for NetWrapper-related errors.
     * @details Inherits from std::exception and is used to represent errors
     * that occur within the NetWrapper class.
     */
    class NetWrapperError : public std::exception {
      public:
        /**
         * @brief Constructor for NetWrapperError.
         * @param message The error message associated with the exception.
         */
        explicit NetWrapperError(const std::string &message) : _message(message)
        {
        }

        /**
         * @brief Override of the what() function from std::exception.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message = ""; ///> The error message associated with the exception.
    };

    /**
     * @class NetWrapper
     * @brief A wrapper class for network socket operations.
     * @details This class provides an abstraction layer for creating and managing
     * network sockets, including sending and receiving data.
     */
    class NetWrapper {
      public:
        /**
         * @brief Default constructor for NetWrapper.
         */
        explicit NetWrapper() = default;

        /**
         * @brief Constructor for NetWrapper that loads functions from a shared library.
         * @param pluginPath The path to the shared library containing the network functions.
         * @throws NetWrapperError if loading the library or functions fails.
         */
        explicit NetWrapper(const std::string &pluginPath, const std::string &baseDir = "build/");

        /**
         * @brief Destructor for NetWrapper.
         */
        ~NetWrapper();

        /**
         * @brief Creates a new socket.
         * @param domain The communication domain (e.g., AF_INET).
         * @param type The socket type (e.g., SOCK_STREAM).
         * @param protocol The protocol to be used (e.g., IPPROTO_TCP).
         * @return The handle to the created socket.
         */
        socketHandle socket(int domain, int type, int protocol);

        /**
         * @brief Closes a socket.
         * @param s The socket handle to close.
         */
        void closeSocket(socketHandle s);

        /**
         * @brief Sets options on a socket.
         * @param s The socket handle.
         * @param level The level at which the option is defined.
         * @param optName The name of the option.
         * @param optVal A pointer to the option value.
         * @param optLen The size of the option value.
         * @return 0 on success, or -1 on error.
         */
        int setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen);

        /**
         * @brief Receives data from a socket.
         * @param sockFd The socket file descriptor.
         * @param buf The buffer to receive data.
         * @param len The length of the buffer.
         * @param flags Flags for the receive operation.
         * @param srcAddr The source address.
         * @param addrLen The length of the source address.
         * @return The number of bytes received.
         */
        recvfrom_return_t recvFrom(
            socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen);

        /**
         * @brief Sends data to a socket.
         * @param sockFd The socket file descriptor.
         * @param buf The buffer containing data to send.
         * @param len The length of the buffer.
         * @param flags Flags for the send operation.
         * @param destAddr The destination address.
         * @param addrLen The length of the destination address.
         * @return The number of bytes sent.
         */
        sendto_return_t sendTo(socketHandle sockFd, const void *buf, size_t len, int flags,
            const struct sockaddr *destAddr, socklen_t addrLen);

        /**
         * @brief Initializes the network (e.g., WSAStartup on Windows).
         * @return 0 on success, or an error code on failure.
         */
        int initNetwork();
      private:
        std::unique_ptr<Library::DLLoader> _loader = nullptr; ///> Dynamic library loader.

        socketHandle (*_socketFn)(int, int, int) = nullptr; ///> Function pointer for creating a socket.
        void (*_closeFn)(socketHandle) = nullptr;           ///> Function pointer for closing a socket.
        int (*_setOptFn)(
            socketHandle, int, int, const void *, int) = nullptr; ///> Function pointer for setting socket options.
        recvfrom_return_t (*_recvFn)(socketHandle, void *, size_t, int, struct sockaddr *,
            socklen_t *) = nullptr; ///> Function pointer for receiving data from a socket.
        sendto_return_t (*_sendFn)(socketHandle, const void *, size_t, int, const struct sockaddr *,
            socklen_t) = nullptr; ///> Function pointer for sending data to a socket.

        int (*_initNetworkFn)() = nullptr; ///> Function pointer for initializing the network.
    };
} // namespace Net
