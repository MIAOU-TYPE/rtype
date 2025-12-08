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
