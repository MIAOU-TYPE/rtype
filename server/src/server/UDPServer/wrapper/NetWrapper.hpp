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
using socket_handle = SOCKET;
constexpr socket_handle kInvalidSocket = INVALID_SOCKET;
#else
    #include <arpa/inet.h>
    #include <unistd.h>
using socket_handle = int;
constexpr socket_handle kInvalidSocket = -1;
#endif

namespace net
{
    class NetWrapper {
      public:
        static socket_handle socket(int domain, int type, int protocol);

        static void close_socket(socket_handle s);

        static int setsocketopt(socket_handle s, int level, int optname, const void *optval, int optlen);

    }; // class NetWrapper
} // namespace net
