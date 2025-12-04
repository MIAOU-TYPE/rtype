/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPServer
*/

#include "UDPServer.hpp"

using namespace Server;

UDPServer::UDPServer() : AServer(), _rxBuffer(1024)
{
    try {
        _netWrapper("shared/NetWrapper/build/libNetWrapperLib.so")
    } catch (const Net::NetWrapperError &e) {
        throw ServerError(std::string("{UDPServer::UDPServer} ") + e.what());
    }
    setRunning(false);
    #ifdef _WIN32
        WSADATA wsa;
        const int r = WSAStartup(MAKEWORD(2, 2), &wsa);
        if (r != 0)
            throw ServerError("{UDPServer::UDPServer} WSAStartup failed");
    #endif
}

UDPServer::~UDPServer()
{
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

void UDPServer::start()
{
    if (isRunning() || _socketFd != kInvalidSocket)
        throw ServerError("{UDPServer::start} Server is already running");

    try {
        Net::SocketConfig socketParams = {AF_INET, SOCK_DGRAM, IPPROTO_UDP};
        Net::SocketOptions socketOptions = {SOL_SOCKET, SO_REUSEADDR, 1};
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
}

void UDPServer::readPackets()
{
    if (!isRunning() || _socketFd == kInvalidSocket)
        return;
    auto pkt = std::make_shared<Net::UDPPacket>();
    socklen_t addrLen = sizeof(sockaddr_in);

    recvfrom_return_t received = _netWrapper.recvFrom(_socketFd, pkt->buffer(), Net::UDPPacket::MAX_SIZE, 0,
        reinterpret_cast<sockaddr *>(const_cast<sockaddr_in *>(pkt->address())), &addrLen);
    if (received <= 0)
        return;
    pkt->setSize(static_cast<size_t>(received));

    {
        std::lock_guard<std::mutex> lock(_rxMutex);
        if (!_rxBuffer.push(pkt)) {
            std::cerr << "{UDPServer::readPackets} Warning: RX buffer overflow, packet dropped\n";
            return;
        }
    }
}

bool UDPServer::sendPacket(const Net::IServerPacket &pkt)
{
    return _netWrapper.sendTo(_socketFd, pkt.buffer(), pkt.size(), 0,
               reinterpret_cast<const sockaddr *>(pkt.address()), sizeof(sockaddr_in))
        != -1;
}

bool UDPServer::popPacket(std::shared_ptr<Net::IServerPacket> &pkt)
{
    std::lock_guard<std::mutex> lock(_rxMutex);

    return _rxBuffer.pop(pkt);
}

void UDPServer::setupSocket(const Net::SocketConfig &params, const Net::SocketOptions &optParams)
{
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::setupSocket} Invalid IP address or port number");

    socketHandle sockFd = _netWrapper.socket(static_cast<int>(params.family), params.type, params.proto);
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

void UDPServer::bindSocket(Net::family_t family)
{
    if (_socketFd == kInvalidSocket)
        throw ServerError("{UDPServer::bindSocket} Socket not initialized");
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::bindSocket} Invalid IP address or port number");

    sockaddr_in addr = {};
    addr.sin_family = family;
    addr.sin_port = htons(_port);
    if (inet_pton(family, _ip.c_str(), &addr.sin_addr) <= 0)
        throw ServerError("{UDPServer::bindSocket} Invalid IP address format");

    int result = bind(_socketFd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
    if (result != 0)
        throw ServerError("{UDPServer::bindSocket} Failed to bind socket");
}
