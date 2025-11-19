/*
** EPITECH PROJECT, 2025
** serverCPP
** File description:
** AServer
*/

#include "AServer.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <errno.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

using namespace Server;

void AServer::configure(const std::string &ip, uint16_t port)
{
    _ip = ip;
    _port = port;
    if (!isStoredIpCorrect() || !isStoredPortCorrect()) {
        _ip = "";
        _port = 0;
        throw ServerError("{AServer::configure} Invalid IP address or port number");
    }
}

void AServer::setNonBlocking(bool nonBlocking)
{
    if (_socketFd == kInvalidSocket)
        return;
#ifdef _WIN32
    u_long mode = nonBlocking ? 1UL : 0UL;
    if (ioctlsocket(_socketFd, FIONBIO, &mode) != 0) {
        throw ServerError("{AServer::setNonBlocking} ioctlsocket(FIONBIO) failed");
    }
#else
    int flags = fcntl(_socketFd, F_GETFL, 0);
    if (flags == -1)
        throw ServerError("{AServer::setNonBlocking} Failed to get socket flags");

    const int newFlags = nonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);

    if (fcntl(_socketFd, F_SETFL, newFlags) == -1)
        throw ServerError("{AServer::setNonBlocking} Failed to set socket flags");
#endif
}

bool AServer::isRunning() const noexcept
{
    return _isRunning;
}

void AServer::setRunning(bool running) noexcept
{
    _isRunning = running;
}

bool AServer::isStoredIpCorrect() const noexcept
{
    return !_ip.empty();
}

bool AServer::isStoredPortCorrect() const noexcept
{
    return _port > 0 && _port <= 65535;
}
