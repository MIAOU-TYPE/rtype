/*
** EPITECH PROJECT, 2025
** serverCPP
** File description:
** UDPServer
*/

#include "UDPServer.hpp"

using namespace Server;

UDPServer::UDPServer() : AServer()
{
#ifdef _WIN32
    WSADATA wsa;
    const int r = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (r != 0) {
        std::fprintf(stderr, "WSAStartup failed: %d\n", r);
        return 1;
    }
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
    if (_isRunning || _socketFd != -1)
        throw ServerError("{UDPServer::start} Server is already running");

    try {
        net::SocketConfig socketParams;
        net::SocketOptions socketOptions;
        setupSocket(socketParams, socketOptions);
        bindSocket(socketParams.family);
        setNonBlocking(true);
    } catch (const ServerError &e) {
        throw ServerError(std::string("{UDPServer::start}") + e.what());
    }
    _isRunning = true;
    std::cout << "{UDPServer::start} UDP Server started on " << _ip << ":" << _port << std::endl;
}

void UDPServer::stop()
{
    _isRunning = false;
    close_socket(_socketFd);
    _socketFd = kInvalidSocket;
    std::cout << "{UDPServer::stop} UDP Server stopped." << std::endl;
}

void UDPServer::pollOnce(int timeout)
{
    (void) timeout;
}

void UDPServer::setupSocket(const net::SocketConfig &params, const net::SocketOptions &optParams)
{
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::setupSocket} Invalid IP address or port number");

    socket_handle sockfd = ::socket(static_cast<int>(params.family), params.type, params.proto);
    if (sockfd == kInvalidSocket)
        throw ServerError("{UDPServer::setupSocket} Failed to create socket");

    int opt = optParams.optval;

#ifdef _WIN32
    if (::setsockopt(sockfd, optParams.level, optParams.optname, reinterpret_cast<const char *>(&opt),
            static_cast<int>(sizeof(opt)))
        == SOCKET_ERROR) {
        close_socket(sockfd);
        throw ServerError("{UDPServer::setupSocket} setsockopt failed");
    }
#else
    if (::setsockopt(sockfd, optParams.level, optParams.optname, &opt, static_cast<socklen_t>(sizeof(opt))) == -1) {
        close_socket(sockfd);
        throw ServerError("{UDPServer::setupSocket} setsockopt failed");
    }
#endif

    _socketFd = sockfd;
}

void UDPServer::bindSocket(net::family_t family)
{
    if (_socketFd < 0 || _socketFd == kInvalidSocket)
        throw ServerError("{UDPServer::bindSocket} Socket not initialized");
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::bindSocket} Invalid IP address or port number");

    sockaddr_in addr;
    addr.sin_family = family;
    addr.sin_port = htons(_port);
    inet_pton(family, _ip.c_str(), &addr.sin_addr);

    int result = bind(_socketFd, (struct sockaddr *) &addr, sizeof(addr));
    if (result != 0)
        throw ServerError("{UDPServer::bindSocket} Failed to bind socket");
}
