/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AServer
*/

#include "AServer.hpp"

using namespace Net::Server;

void AServer::configure(const std::string &ip, int32_t port)
{
    _ip = ip;
    _port = port;
    if (!isStoredIpCorrect() || !isStoredPortCorrect()) {
        _ip = "";
        _port = 0;
        throw ServerError("{AServer::configure} Invalid IP address or port number");
    }
}

bool AServer::isRunning() const noexcept
{
    return _isRunning.load();
}

void AServer::setRunning(bool running) noexcept
{
    _isRunning.store(running);
}

bool AServer::isStoredIpCorrect() const noexcept
{
    return !_ip.empty();
}

bool AServer::isStoredPortCorrect() const noexcept
{
    return _port > 0 && _port <= 65535;
}
