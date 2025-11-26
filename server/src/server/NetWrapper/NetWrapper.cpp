/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetWrapper
*/

#include "NetWrapper.hpp"

namespace Net
{

    socketHandle NetWrapper::socket(int domain, int type, int protocol)
    {
        return ::socket(domain, type, protocol);
    }

#ifdef _WIN32
    void NetWrapper::closeSocket(socketHandle s)
    {
        if (s != kInvalidSocket)
            closesocket(s);
    }

    int NetWrapper::setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen)
    {
        return ::setsockopt(s, level, optName, (const char *) optVal, optLen);
    }

    recvfrom_return_t NetWrapper::recvFrom(
        socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen)
    {
        return ::recvfrom(sockFd, (char *) buf, static_cast<int>(len), flags, srcAddr, addrLen);
    }

    sendto_return_t NetWrapper::sendTo(
        socketHandle sockFd, const void *buf, size_t len, int flags, const struct sockaddr *destAddr, socklen_t addrLen)
    {
        return ::sendto(sockFd, (const char *) buf, static_cast<int>(len), flags, destAddr, addrLen);
    }
#endif

#ifndef _WIN32
    void NetWrapper::closeSocket(socketHandle s)
    {
        if (s != kInvalidSocket)
            close(s);
    }

    int NetWrapper::setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen)
    {
        return ::setsockopt(s, level, optName, optVal, static_cast<socklen_t>(optLen));
    }

    recvfrom_return_t NetWrapper::recvFrom(
        socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen)
    {
        return ::recvfrom(sockFd, buf, len, flags, srcAddr, addrLen);
    }

    sendto_return_t NetWrapper::sendTo(
        socketHandle sockFd, const void *buf, size_t len, int flags, const struct sockaddr *destAddr, socklen_t addrLen)
    {
        return ::sendto(sockFd, buf, len, flags, destAddr, addrLen);
    }
#endif
} // namespace Net
