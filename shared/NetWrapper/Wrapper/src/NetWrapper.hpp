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
using recv_return_t = int;
using send_return_t = int;
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
using socketHandle = int;
constexpr socketHandle kInvalidSocket = -1;
using recvfrom_return_t = ssize_t;
using sendto_return_t = ssize_t;
using recv_return_t = ssize_t;
using send_return_t = ssize_t;
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
        explicit NetWrapper(const std::string &pluginPath, const std::string &baseDir = "./libraries/");

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
        socketHandle socket(int domain, int type, int protocol) const;

        /**
         * @brief Closes a socket.
         * @param s The socket handle to close.
         */
        void closeSocket(socketHandle s) const;

        /**
         * @brief Sets options on a socket.
         * @param s The socket handle.
         * @param level The level at which the option is defined.
         * @param optName The name of the option.
         * @param optVal A pointer to the option value.
         * @param optLen The size of the option value.
         * @return 0 on success, or -1 on error.
         */
        int setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen) const;

        /**
         * @brief Receives data from a socket.
         * @param sockFd The socket file descriptor.
         * @param buf The buffer to receive data.
         * @param len The length of the buffer.
         * @param flags Flags for the reception operation.
         * @param srcAddr The source address.
         * @param addrLen The length of the source address.
         * @return The number of bytes received.
         */
        recvfrom_return_t recvFrom(
            socketHandle sockFd, void *buf, size_t len, int flags, sockaddr *srcAddr, socklen_t *addrLen) const;

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
        sendto_return_t sendTo(socketHandle sockFd, const void *buf, size_t len, int flags, const sockaddr *destAddr,
            socklen_t addrLen) const;

        /**
         * @brief Receives data from a socket.
         * @param sockFd The socket file descriptor.
         * @param buf The buffer to receive data.
         * @param len The length of the buffer.
         * @param flags Flags for the reception operation.
         * @return The number of bytes received.
         */
        recv_return_t recv(socketHandle sockFd, void *buf, size_t len, int flags) const;

        /**
         * @brief Sends data to a socket.
         * @param sockFd The socket file descriptor.
         * @param buf The buffer containing data to send.
         * @param len The length of the buffer.
         * @param flags Flags for the send operation.
         * @return The number of bytes sent.
         */
        send_return_t send(socketHandle sockFd, const void *buf, size_t len, int flags) const;

        /**
         * @brief Initializes the network (e.g., WSAStartup on Windows).
         * @return 0 on success, or an error code on failure.
         */
        int initNetwork() const;

        /**
         * @brief Cleans up the network (e.g., WSACleanup on Windows).
         * @return 0 on success, or an error code on failure.
         */
        int cleanupNetwork() const;

        /**
         * @brief Puts the socket into listening mode.
         * @param sockFd The socket file descriptor.
         * @param backlog The maximum length of the queue of pending connections.
         * @return 0 on success, or -1 on error.
         */
        int listen(socketHandle sockFd, int backlog) const;

        /**
         * @brief Accepts a new connection on a listening socket.
         * @param sockFd The listening socket file descriptor.
         * @param addr Pointer to a sockaddr structure to receive the address of the connecting entity.
         * @param addrLen Pointer to a socklen_t variable that initially contains the size of the addr structure.
         * @return The handle to the accepted socket.
         */
        socketHandle accept(socketHandle sockFd, sockaddr *addr, socklen_t *addrLen) const;

        /**
         * @brief Binds a socket to an address.
         * @param sockFd The socket file descriptor.
         * @param addr Pointer to the sockaddr structure containing the address to bind to.
         * @param addrLen The length of the address structure.
         * @return 0 on success, or -1 on error.
         */
        int bind(socketHandle sockFd, const sockaddr *addr, socklen_t addrLen) const;

        /**
         * @brief Connects a socket to a remote address.
         * @param sockFd The socket file descriptor.
         * @param addr Pointer to the sockaddr structure containing the remote address.
         * @param addrLen The length of the address structure.
         * @return 0 on success, or -1 on error.
         */
        int connect(socketHandle sockFd, const sockaddr *addr, socklen_t addrLen) const;

      private:
        std::unique_ptr<Library::DLLoader> _loader = nullptr; ///> Dynamic library loader.

        /**
         * @brief Pointer to the socket creation function.
         */
        socketHandle (*_socketFn)(int, int, int) = nullptr;

        /**
         * @brief Pointer to the socket close function.
         */
        void (*_closeFn)(socketHandle) = nullptr;

        /**
         * @brief Pointer to the set socket options function.
         */
        int (*_setOptFn)(socketHandle, int, int, const void *, int) = nullptr;

        /**
         * @brief Pointer to the reception from function.
         */
        recvfrom_return_t (*_recvFromFn)(socketHandle, void *, size_t, int, sockaddr *, socklen_t *) = nullptr;

        /**
         * @brief Pointer to the send to function.
         */
        sendto_return_t (*_sendToFn)(socketHandle, const void *, size_t, int, const sockaddr *, socklen_t) = nullptr;

        /**
         * @brief Pointer to the reception function.
         */
        recv_return_t (*_recvFn)(socketHandle, void *, size_t, int) = nullptr;

        /**
         * @brief Pointer to the send function.
         */
        send_return_t (*_sendFn)(socketHandle, const void *, size_t, int) = nullptr;

        /**
         * @brief Pointer to the network initialization function.
         */
        int (*_initNetworkFn)() = nullptr;

        /**
         * @brief Pointer to the network cleanup function.
         */
        int (*_cleanupNetworkFn)() = nullptr;

        /**
         * @brief Pointer to the accept function.
         */
        socketHandle (*_acceptFn)(socketHandle, sockaddr *, socklen_t *) = nullptr;

        /**
         * @brief Pointer to the bind function.
         */
        int (*_bindFn)(socketHandle, const sockaddr *, socklen_t) = nullptr;

        /**
         * @brief Pointer to the listen function.
         */
        int (*_listenFn)(socketHandle, int) = nullptr;

        /**
         * @brief Pointer to the connect function.
         */
        int (*_connectFn)(socketHandle, const sockaddr *, socklen_t) = nullptr;
    };
} // namespace Net
