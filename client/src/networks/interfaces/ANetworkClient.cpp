/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ANetworkClient
*/

#include "ANetworkClient.hpp"
#include <fcntl.h>

namespace Network
{
    ANetworkClient::ANetworkClient() : _socketFd(-1), _serverPort(0), _running(false)
    {
        // Initialize default socket configuration
        _socketConfig.family = AF_INET;
        _socketConfig.type = SOCK_DGRAM;
        _socketConfig.protocol = IPPROTO_UDP;
    }

    std::vector<std::shared_ptr<IClientPacket>> ANetworkClient::pollPackets()
    {
        std::vector<std::shared_ptr<IClientPacket>> packets;
        std::lock_guard<std::mutex> lock(_receiveMutex);
        while (!_receivedPackets.empty()) {
            packets.push_back(_receivedPackets.front());
            _receivedPackets.pop();
        }
        return packets;
    }

    bool ANetworkClient::isConnected() const
    {
        return _running && _socketFd >= 0;
    }

    void ANetworkClient::setupSocket(const SocketConfig &config)
    {
        _socketFd = socket(config.family, config.type, config.protocol);

        if (_socketFd < 0) {
            throw Client::Exception::SocketException("Failed to create socket", errno);
        }

        _socketConfig = config;
    }

    void ANetworkClient::applySocketOptions(const SocketOptions &options)
    {
        if (_socketFd < 0) {
            throw Client::Exception::SocketException("Socket not initialized", 0);
        }

        int opt = options.optVal;
        if (setsockopt(_socketFd, options.level, options.optName, &opt, sizeof(opt)) < 0) {
            throw Client::Exception::SocketException("Failed to set socket options", errno);
        }
    }

    void ANetworkClient::configureServerAddress(const std::string &ip, uint16_t port)
    {
        _serverIp = ip;
        _serverPort = port;

        if (!isStoredIpCorrect() || !isStoredPortCorrect()) {
            throw Client::Exception::ConnectionException("Invalid IP address or port number");
        }

        std::memset(&_serverAddr, 0, sizeof(_serverAddr));
        _serverAddr.sin_family = _socketConfig.family;
        _serverAddr.sin_port = htons(port);

        if (inet_pton(_socketConfig.family, ip.c_str(), &_serverAddr.sin_addr) <= 0) {
            throw Client::Exception::ConnectionException("Invalid server IP address : " + ip);
        }
    }

    void ANetworkClient::setNonBlocking(bool nonBlocking)
    {
        if (_socketFd < 0) {
            throw Client::Exception::SocketException("Socket not initialized", 0);
        }

        int flags = fcntl(_socketFd, F_GETFL, 0);
        if (flags == -1) {
            throw Client::Exception::SocketException("Failed to get socket flags", errno);
        }

        const int newFlags = nonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);

        if (fcntl(_socketFd, F_SETFL, newFlags) == -1) {
            throw Client::Exception::SocketException("Failed to set socket flags", errno);
        }
    }

    bool ANetworkClient::isStoredIpCorrect() const noexcept
    {
        return !_serverIp.empty();
    }

    bool ANetworkClient::isStoredPortCorrect() const noexcept
    {
        return _serverPort > 0 && _serverPort <= 65535;
    }
} // namespace Network
