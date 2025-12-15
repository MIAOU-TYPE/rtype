/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ANetClient
*/

#include "ANetClient.hpp"

namespace Network
{

    void ANetClient::configure(const std::string &ip, uint32_t port)
    {
        _ip = ip;
        _port = port;
        if (!isStoredIpCorrect() || !isStoredPortCorrect()) {
            _ip = "";
            _port = 0;
            throw NetClientError("{ANetClient::configure} Invalid IP address or port number");
        }
    }

    void ANetClient::setNonBlocking(bool nonBlocking)
    {
        if (_socketFd == kInvalidSocket)
            return;
#ifdef _WIN32
        u_long mode = nonBlocking ? 1UL : 0UL;
        if (ioctlsocket(_socketFd, FIONBIO, &mode) != 0) {
            throw NetClientError("{ANetClient::setNonBlocking} ioctlsocket(FIONBIO) failed");
        }
#else
        int flags = fcntl(_socketFd, F_GETFL, 0);
        if (flags == -1)
            throw NetClientError("{ANetClient::setNonBlocking} Failed to get socket flags");

        const int newFlags = nonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
        if (fcntl(_socketFd, F_SETFL, newFlags) == -1)
            throw NetClientError("{ANetClient::setNonBlocking} Failed to set socket flags");
#endif
    }

    bool ANetClient::isRunning() const noexcept
    {
        return _isRunning.load();
    }

    void ANetClient::setRunning(bool running) noexcept
    {
        _isRunning.store(running);
    }

    bool ANetClient::isStoredIpCorrect() const noexcept
    {
        return !_ip.empty();
    }

    bool ANetClient::isStoredPortCorrect() const noexcept
    {
        return _port > 0 && _port <= 65535;
    }

} // namespace Network