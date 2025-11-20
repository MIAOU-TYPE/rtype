/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetWrapper
*/

#include "NetWrapper.hpp"

namespace net
{

    socket_handle NetWrapper::socket(int domain, int type, int protocol)
    {
        return ::socket(domain, type, protocol);
    }

#ifdef _WIN32
    void NetWrapper::close_socket(socket_handle s)
    {
        if (s != kInvalidSocket)
            closesocket(s);
    }

    int NetWrapper::setsocketopt(socket_handle s, int level, int optname, const void *optval, int optlen)
    {
        return ::setsockopt(s, level, optname, (const char *) optval, optlen);
    }

    recvfrom_return_t NetWrapper::recvfrom(
        socket_handle sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
    {
        return ::recvfrom(sockfd, (char *) buf, len, flags, src_addr, addrlen);
    }

    ssize_t NetWrapper::sendto(
        socket_handle sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
    {
        return ::sendto(sockfd, (const char *) buf, len, flags, dest_addr, addrlen);
    }
#endif

#ifndef _WIN32
    void NetWrapper::close_socket(socket_handle s)
    {
        if (s != kInvalidSocket)
            close(s);
    }

    int NetWrapper::setsocketopt(socket_handle s, int level, int optname, const void *optval, int optlen)
    {
        return ::setsockopt(s, level, optname, (const void *) optval, optlen);
    }

    recvfrom_return_t NetWrapper::recvfrom(
        socket_handle sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
    {
        return ::recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    }

    sendto_return_t NetWrapper::sendto(
        socket_handle sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
    {
        return ::sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    }
#endif
} // namespace net
