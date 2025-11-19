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
#endif
} // namespace net
