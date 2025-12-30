/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPServer
*/

#include "UDPServer.hpp"

using namespace Net::Server;

UDPServer::UDPServer() : AServer(), _rxBuffer(1024), _netWrapper("NetPluginLib")
{
    AServer::setRunning(false);
    if (_netWrapper.initNetwork() != 0)
        throw ServerError("{UDPServer::UDPServer} Failed to initialize network");
}

UDPServer::~UDPServer()
{
    UDPServer::stop();
}

void UDPServer::start()
{
    if (isRunning() || _socketFd != kInvalidSocket)
        throw ServerError("{UDPServer::start} Server is already running");

    try {
        constexpr SocketConfig socketParams = {AF_INET, SOCK_DGRAM, IPPROTO_UDP};
        constexpr SocketOptions socketOptions = {SOL_SOCKET, SO_REUSEADDR, 1};
        setupSocket(socketParams, socketOptions);
        bindSocket(socketParams.family);
        setNonBlocking(true);
    } catch (const ServerError &e) {
        if (_socketFd != kInvalidSocket)
            _netWrapper.closeSocket(_socketFd);
        _socketFd = kInvalidSocket;
        throw ServerError(std::string("{UDPServer::start}") + e.what());
    }
    setRunning(true);
    std::cout << "{UDPServer::start} UDP Server started on " << _ip << ":" << _port << std::endl;
}

void UDPServer::stop()
{
    setRunning(false);
    if (_socketFd != kInvalidSocket) {
        _netWrapper.closeSocket(_socketFd);
        _socketFd = kInvalidSocket;
        std::cout << "{UDPServer::stop} UDP Server stopped." << std::endl;
    }
    if (const auto result = _netWrapper.cleanupNetwork(); result != 0)
        throw ServerError("{UDPServer::stop} Failed to cleanup network");
}

void UDPServer::setNonBlocking(const bool nonBlocking)
{
    if (_socketFd == kInvalidSocket)
        throw ServerError("{UDPServer::setNonBlocking} Socket not initialized");

    if (_netWrapper.setNonBlocking(_socketFd, nonBlocking ? 1 : 0) < 0)
        throw ServerError("{UDPServer::setNonBlocking} Failed to set socket non-blocking mode");
}

void UDPServer::readPackets()
{
    if (!isRunning() || _socketFd == kInvalidSocket)
        return;
    const auto pkt = std::make_shared<Net::UDPPacket>();
    socklen_t addrLen = sizeof(sockaddr_in);

    const recvfrom_return_t received = _netWrapper.recvFrom(_socketFd, pkt->buffer(), Net::UDPPacket::MAX_SIZE, 0,
        reinterpret_cast<sockaddr *>(const_cast<sockaddr_in *>(pkt->address())), &addrLen);
    if (received <= 0)
        return;
    pkt->setSize(static_cast<size_t>(received));

    {
        std::scoped_lock lock(_rxMutex);
        if (!_rxBuffer.push(pkt))
            std::cerr << "{UDPServer::readPackets} Warning: RX buffer overflow, packet dropped\n";
    }
}

bool UDPServer::sendPacket(const Net::IPacket &pkt)
{
    return _netWrapper.sendTo(_socketFd, pkt.buffer(), pkt.size(), 0, reinterpret_cast<const sockaddr *>(pkt.address()),
               sizeof(sockaddr_in))
        != -1;
}

bool UDPServer::popPacket(std::shared_ptr<Net::IPacket> &pkt)
{
    std::scoped_lock lock(_rxMutex);

    return _rxBuffer.pop(pkt);
}

void UDPServer::setupSocket(const Net::SocketConfig &params, const Net::SocketOptions &optParams)
{
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::setupSocket} Invalid IP address or port number");

    const socketHandle sockFd = _netWrapper.socket(static_cast<int>(params.family), params.type, params.proto);
    if (sockFd == kInvalidSocket)
        throw ServerError("{UDPServer::setupSocket} Failed to create socket");

    int opt = optParams.optVal;
    if (_netWrapper.setSocketOpt(
            sockFd, optParams.level, optParams.optName, reinterpret_cast<const char *>(&opt), sizeof(opt))
        < 0) {
        _netWrapper.closeSocket(sockFd);
        throw ServerError("{UDPServer::setupSocket} Failed to set socket options");
    }

    _socketFd = sockFd;
}

void UDPServer::bindSocket(family_t family) const
{
    if (_socketFd == kInvalidSocket)
        throw ServerError("{UDPServer::bindSocket} Socket not initialized");
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::bindSocket} Invalid IP address or port number");

    sockaddr_in addr = {};
    addr.sin_family = family;
    addr.sin_port = htons(static_cast<uint16_t>(_port));
    if (inet_pton(family, _ip.c_str(), &addr.sin_addr) <= 0)
        throw ServerError("{UDPServer::bindSocket} Invalid IP address format");

    if (const int result = _netWrapper.bind(_socketFd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
        result != 0)
        throw ServerError("{UDPServer::bindSocket} Failed to bind socket");
}
