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
#ifdef _WIN32
    WSADATA wsa;
    const int r = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (r != 0)
        throw ServerError("{UDPServer::UDPServer} WSAStartup failed");
#endif
}

UDPServer::~UDPServer()
{
    if (_isRunning) {
        stop();
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

void UDPServer::start()
{
    if (_isRunning || _socketFd != kInvalidSocket)
        throw ServerError("{UDPServer::start} Server is already running");

    try {
        Net::SocketConfig socketParams = {AF_INET, SOCK_DGRAM, IPPROTO_UDP};
        Net::SocketOptions socketOptions = {SOL_SOCKET, SO_REUSEADDR, 1};
        setupSocket(socketParams, socketOptions);
        bindSocket(socketParams.family);
        setNonBlocking(true);
    } catch (const ServerError &e) {
        if (_socketFd != kInvalidSocket)
            Net::NetWrapper::closeSocket(_socketFd);
        _socketFd = kInvalidSocket;
        throw ServerError(std::string("{UDPServer::start}") + e.what());
    }
    _isRunning = true;
    std::cout << "{UDPServer::start} UDP Server started on " << _ip << ":" << _port << std::endl;
}

void UDPServer::stop()
{
    _isRunning = false;
    Net::NetWrapper::closeSocket(_socketFd);
    _socketFd = kInvalidSocket;
    std::cout << "{UDPServer::stop} UDP Server stopped." << std::endl;
}

void UDPServer::readPackets()
{
    std::shared_ptr<Net::IServerPacket> pkt = std::make_shared<Net::UDPPacket>();
    socklen_t addrLen = sizeof(sockaddr_in);

    recvfrom_return_t received = Net::NetWrapper::recvFrom(_socketFd, pkt->buffer(), Net::UDPPacket::MAX_SIZE, 0,
        reinterpret_cast<sockaddr *>(const_cast<sockaddr_in *>(pkt->address())), &addrLen);
    if (received <= 0)
        return;
    pkt->setSize(received);

    if (!_rxBuffer.push(pkt))
        std::cerr << "Warning: RX buffer overflow, packet dropped\n";
    std::cout << pkt << std::endl;
}

bool UDPServer::sendPacket(const Net::IServerPacket &pkt)
{
    return Net::NetWrapper::sendTo(_socketFd, pkt.buffer(), pkt.size(), 0,
               reinterpret_cast<const sockaddr *>(pkt.address()), sizeof(sockaddr_in))
        != -1;
}

void UDPServer::setupSocket(const Net::SocketConfig &params, const Net::SocketOptions &optParams)
{
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::setupSocket} Invalid IP address or port number");

    socket_handle sockfd = Net::NetWrapper::socket(static_cast<int>(params.family), params.type, params.proto);
    if (sockfd == kInvalidSocket)
        throw ServerError("{UDPServer::setupSocket} Failed to create socket");

    int opt = optParams.optval;
    if (Net::NetWrapper::setSocketOpt(
            sockfd, optParams.level, optParams.optname, reinterpret_cast<const char *>(&opt), sizeof(opt))
        < 0) {
        Net::NetWrapper::closeSocket(sockfd);
        throw ServerError("{UDPServer::setupSocket} Failed to set socket options");
    }

    _socketFd = sockfd;
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
