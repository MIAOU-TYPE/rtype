/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetWrapper
*/

#include "NetWrapper.hpp"

namespace Network
{

    socketHandle NetWrapper::socket(int domain, int type, int protocol)
    {
        socketHandle sock = ::socket(domain, type, protocol);

        if (sock == kInvalidSocket) {
            throw NetWrapperError("Failed to create socket");
        }
        return sock;
    }

#ifdef _WIN32
    void NetWrapper::closeSocket(socketHandle s)
    {
        if (s != kInvalidSocket)
            closesocket(s);
    }

    int NetWrapper::setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen)
    {
        return ::setsockopt(s, level, optName, static_cast<const char *>(optVal), optLen);
    }

    recvfrom_return_t NetWrapper::recvFrom(
        socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen)
    {
        return ::recvfrom(sockFd, static_cast<char *>(buf), static_cast<int>(len), flags, srcAddr, addrLen);
    }

    sendto_return_t NetWrapper::sendTo(
        socketHandle sockFd, const void *buf, size_t len, int flags, const struct sockaddr *destAddr, socklen_t addrLen)
    {
        return ::sendto(sockFd, static_cast<const char *>(buf), static_cast<int>(len), flags, destAddr, addrLen);
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
        int result = ::setsockopt(s, level, optName, optVal, static_cast<socklen_t>(optLen));

        if (result == -1) {
            throw NetWrapperError("Failed to set socket option");
        }
        return result;
    }

    recvfrom_return_t NetWrapper::recvFrom(
        socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen)
    {
        recvfrom_return_t result = ::recvfrom(sockFd, buf, len, flags, srcAddr, addrLen);

        if (result == -1) {
            throw NetWrapperError("Failed to receive data from socket");
        }
        return result;
    }

    sendto_return_t NetWrapper::sendTo(
        socketHandle sockFd, const void *buf, size_t len, int flags, const struct sockaddr *destAddr, socklen_t addrLen)
    {
        sendto_return_t result = ::sendto(sockFd, buf, len, flags, destAddr, addrLen);

        if (result == -1) {
            throw NetWrapperError("Failed to send data to socket");
        }
        return result;
    }
#endif
} // namespace Network
