/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetPlugin
*/

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

#include "NetPlugin.hpp"

extern "C"
{
    EXPORT int net_initNetwork()
    {
#ifdef _WIN32
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData);
#else
        return 0;
#endif
    }

    EXPORT int net_cleanupNetwork()

    {
#ifdef _WIN32
        return WSACleanup();
#else
        return 0;
#endif
    }

    EXPORT socketHandle net_socket(const int domain, const int type, const int protocol)
    {
        return ::socket(domain, type, protocol);
    }

    EXPORT int net_listen(const socketHandle sockFd, const int backlog)
    {
        return ::listen(sockFd, backlog);
    }

    EXPORT socketHandle net_accept(const socketHandle sockFd, sockaddr *addr, socklen_t *addrLen)
    {
        return ::accept(sockFd, addr, addrLen);
    }

    EXPORT int net_bind(const socketHandle sockFd, const sockaddr *addr, const socklen_t addrLen)
    {
        return ::bind(sockFd, addr, addrLen);
    }

#ifdef _WIN32
    EXPORT void net_close(socketHandle s)
    {
        if (s != kInvalidSocket)
            closesocket(s);
    }

    EXPORT int net_setOpt(socketHandle s, int level, int optName, const void *optVal, int optLen)
    {
        return ::setsockopt(s, level, optName, (const char *) optVal, optLen);
    }

    EXPORT recvfrom_return_t net_recvFrom(
        socketHandle sockFd, void *buf, size_t len, int flags, sockaddr *srcAddr, socklen_t *addrLen)
    {
        return ::recvfrom(sockFd, (char *) buf, static_cast<int>(len), flags, srcAddr, (int *) addrLen);
    }

    EXPORT sendto_return_t net_sendTo(
        socketHandle sockFd, const void *buf, size_t len, int flags, const sockaddr *destAddr, socklen_t addrLen)
    {
        return ::sendto(sockFd, (const char *) buf, static_cast<int>(len), flags, destAddr, static_cast<int>(addrLen));
    }

    EXPORT recv_return_t net_recv(const socketHandle sockFd, void *buf, const size_t len, const int flags)
    {
        return ::recv(sockFd, (char *) buf, static_cast<int>(len), flags);
    }

    EXPORT send_return_t net_send(const socketHandle sockFd, const void *buf, const size_t len, const int flags)
    {
        return ::send(sockFd, (const char *) buf, static_cast<int>(len), flags);
    }

    EXPORT int net_connect(const socketHandle sockFd, const sockaddr *addr, const socklen_t addrLen)
    {
        return ::connect(sockFd, addr, static_cast<int>(addrLen));
    }
#endif

#ifndef _WIN32
    EXPORT void net_close(socketHandle s)
    {
        if (s != kInvalidSocket)
            close(s);
    }

    EXPORT int net_setOpt(
        const socketHandle s, const int level, const int optName, const void *optVal, const int optLen)
    {
        return ::setsockopt(s, level, optName, static_cast<const void *>(optVal), static_cast<socklen_t>(optLen));
    }

    EXPORT recvfrom_return_t net_recvFrom(
        const socketHandle sockFd, void *buf, const size_t len, const int flags, sockaddr *srcAddr, socklen_t *addrLen)
    {
        return ::recvfrom(sockFd, buf, len, flags, srcAddr, addrLen);
    }

    EXPORT sendto_return_t net_sendTo(const socketHandle sockFd, const void *buf, const size_t len, const int flags,
        const sockaddr *destAddr, const socklen_t addrLen)
    {
        return ::sendto(sockFd, buf, len, flags, destAddr, addrLen);
    }

    EXPORT recv_return_t net_recv(const socketHandle sockFd, void *buf, const size_t len, const int flags)
    {
        return ::recv(sockFd, buf, len, flags);
    }

    EXPORT send_return_t net_send(const socketHandle sockFd, const void *buf, const size_t len, const int flags)
    {
        return ::send(sockFd, buf, len, flags);
    }

    EXPORT int net_connect(const socketHandle sockFd, const sockaddr *addr, const socklen_t addrLen)
    {
        return ::connect(sockFd, addr, addrLen);
    }
#endif
}
