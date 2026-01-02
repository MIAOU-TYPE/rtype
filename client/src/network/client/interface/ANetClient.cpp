/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ANetClient
*/

#include "ANetClient.hpp"

namespace Network
{

    void ANetClient::configure(const std::string &ip, const int32_t port)
    {
        _ip = ip;
        _port = port;
        if (!isStoredIpCorrect() || !isStoredPortCorrect())
            throw NetClientError("{ANetClient::configure} Invalid IP address or port number");
    }

    bool ANetClient::isRunning() const noexcept
    {
        return _isRunning.load();
    }

    void ANetClient::setRunning(const bool running) noexcept
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