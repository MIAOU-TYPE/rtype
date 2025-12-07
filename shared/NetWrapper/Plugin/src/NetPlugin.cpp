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

    EXPORT socketHandle net_socket(int domain, int type, int protocol)
    {
        return ::socket(domain, type, protocol);
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
        socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen)
    {
        return ::recvfrom(sockFd, (char *) buf, static_cast<int>(len), flags, srcAddr, (int *) addrLen);
    }

    EXPORT sendto_return_t net_sendTo(
        socketHandle sockFd, const void *buf, size_t len, int flags, const struct sockaddr *destAddr, socklen_t addrLen)
    {
        return ::sendto(sockFd, (const char *) buf, static_cast<int>(len), flags, destAddr, static_cast<int>(addrLen));
    }
#endif

#ifndef _WIN32
    void net_close(socketHandle s)
    {
        if (s != kInvalidSocket)
            close(s);
    }

    int net_setOpt(socketHandle s, int level, int optName, const void *optVal, int optLen)
    {
        return ::setsockopt(s, level, optName, static_cast<const void *>(optVal), static_cast<socklen_t>(optLen));
    }

    recvfrom_return_t net_recvFrom(
        socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen)
    {
        return ::recvfrom(sockFd, buf, len, flags, srcAddr, addrLen);
    }

    sendto_return_t net_sendTo(
        socketHandle sockFd, const void *buf, size_t len, int flags, const struct sockaddr *destAddr, socklen_t addrLen)
    {
        return ::sendto(sockFd, buf, len, flags, destAddr, addrLen);
    }
#endif
}
