/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ANetworkClient
*/

#include "ANetworkClient.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <fcntl.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif

namespace Network
{
    ANetworkClient::ANetworkClient() : _socketFd(kInvalidSocket), _running(false)
    {
#ifdef _WIN32
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            throw Client::Exception::SocketException("WSAStartup failed", result);
        }
#endif
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
        return _running && _socketFd != kInvalidSocket;
    }

    void ANetworkClient::setupSocket(const SocketConfig &config)
    {
        _socketFd = NetWrapper::socket(config.family, config.type, config.protocol);

        if (_socketFd == kInvalidSocket) {
#ifdef _WIN32
            throw Client::Exception::SocketException("Failed to create socket", WSAGetLastError());
#else
            throw Client::Exception::SocketException("Failed to create socket", errno);
#endif
        }

        _socketConfig = config;
    }

    void ANetworkClient::applySocketOptions(const SocketOptions &options)
    {
        if (_socketFd == kInvalidSocket) {
            throw Client::Exception::SocketException("Socket not initialized", 0);
        }

        int opt = options.optVal;
        if (NetWrapper::setSocketOpt(_socketFd, options.level, options.optName, &opt, sizeof(opt)) < 0) {
#ifdef _WIN32
            throw Client::Exception::SocketException("Failed to set socket options", WSAGetLastError());
#else
            throw Client::Exception::SocketException("Failed to set socket options", errno);
#endif
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
        _serverAddr.sin_family = static_cast<sa_family_t>(_socketConfig.family);
        _serverAddr.sin_port = htons(port);

        if (inet_pton(_socketConfig.family, ip.c_str(), &_serverAddr.sin_addr) <= 0) {
            throw Client::Exception::ConnectionException("Invalid server IP address: " + ip);
        }
    }

    void ANetworkClient::setNonBlocking(bool nonBlocking)
    {
        if (_socketFd == kInvalidSocket) {
            throw Client::Exception::SocketException("Socket not initialized", 0);
        }

#ifdef _WIN32
        u_long mode = nonBlocking ? 1 : 0;
        if (ioctlsocket(_socketFd, FIONBIO, &mode) != 0) {
            throw Client::Exception::SocketException("Failed to set socket non-blocking mode", WSAGetLastError());
        }
#else
        int flags = fcntl(_socketFd, F_GETFL, 0);
        if (flags == -1) {
            throw Client::Exception::SocketException("Failed to get socket flags", errno);
        }

        const int newFlags = nonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);

        if (fcntl(_socketFd, F_SETFL, newFlags) == -1) {
            throw Client::Exception::SocketException("Failed to set socket flags", errno);
        }
#endif
    }

    bool ANetworkClient::isStoredIpCorrect() const noexcept
    {
        return !_serverIp.empty();
    }

    bool ANetworkClient::isStoredPortCorrect() const noexcept
    {
        return _serverPort > 0;
    }
} // namespace Network
