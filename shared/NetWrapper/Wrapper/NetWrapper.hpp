/*
** EPITECH PROJECT, 2025
** NetWrapperDL
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

namespace Net
{
    class NetWrapperError : public std::exception {
      public:
        explicit NetWrapperError(const std::string &message) : _message(message)
        {
        }

        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message = "";
    };

    class NetWrapper {
      public:
        explicit NetWrapper() = default;
        explicit NetWrapper(const std::string &pluginPath);

        ~NetWrapper();

        socketHandle socket(int domain, int type, int protocol);

        void closeSocket(socketHandle s);

        int setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen);

        recvfrom_return_t recvFrom(
            socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen);

        sendto_return_t sendTo(socketHandle sockFd, const void *buf, size_t len, int flags,
            const struct sockaddr *destAddr, socklen_t addrLen);

      private:
        std::unique_ptr<Library::DLLoader> _loader = nullptr;

        socketHandle (*_socketFn)(int, int, int) = nullptr;
        void (*_closeFn)(socketHandle) = nullptr;
        int (*_setOptFn)(socketHandle, int, int, const void *, int) = nullptr;
        recvfrom_return_t (*_recvFn)(socketHandle, void *, size_t, int, struct sockaddr *, socklen_t *) = nullptr;
        sendto_return_t (*_sendFn)(
            socketHandle, const void *, size_t, int, const struct sockaddr *, socklen_t) = nullptr;
    };
} // namespace Net
