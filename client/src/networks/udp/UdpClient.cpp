/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UdpClient
*/

#include "UdpClient.hpp"
#include <chrono>
#include <thread>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <cerrno>
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif

namespace Network
{

    UdpClient::UdpClient() : ANetworkClient()
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

    UdpClient::~UdpClient()
    {
        disconnectFromServer();
#ifdef _WIN32
        WSACleanup();
#endif
    }

    void UdpClient::connectToServer(const std::string &ip, uint16_t port)
    {
        setupSocket(_socketConfig);
        configureServerAddress(ip, port);
        setNonBlocking(true);

        _running = true;
        _receiverThread = std::thread(&UdpClient::receiveLoop, this);
    }

    void UdpClient::disconnectFromServer()
    {
        _running = false;
        if (_receiverThread.joinable()) {
            _receiverThread.join();
        }
        if (_socketFd != kInvalidSocket) {
            NetWrapper::closeSocket(_socketFd);
            _socketFd = kInvalidSocket;
        }
    }

    void UdpClient::sendPacket(const std::vector<uint8_t> &data)
    {
        sendto_return_t sent = NetWrapper::sendTo(
            _socketFd, data.data(), data.size(), 0, reinterpret_cast<sockaddr *>(&_serverAddr), sizeof(_serverAddr));

        if (sent < 0) {
#ifdef _WIN32
            throw Client::Exception::SendException("Failed to send UDP packet", WSAGetLastError());
#else
            throw Client::Exception::SendException("Failed to send UDP packet", errno);
#endif
        }
    }

    void UdpClient::receiveLoop()
    {
        try {
            while (_running) {
                auto packet = std::make_shared<UdpClientPacket>();
                socklen_t senderAddrLen = sizeof(sockaddr_in);

                recvfrom_return_t receivedBytes = NetWrapper::recvFrom(_socketFd, packet->buffer(),
                    AClientPacket::MAX_SIZE, 0, reinterpret_cast<sockaddr *>(packet->address()), &senderAddrLen);

                if (receivedBytes > 0) {
                    packet->setSize(static_cast<size_t>(receivedBytes));
                    std::lock_guard<std::mutex> lock(_receiveMutex);
                    _receivedPackets.push(packet);
                } else if (receivedBytes < 0) {
#ifdef _WIN32
                    int error = WSAGetLastError();
                    if (error == WSAEWOULDBLOCK) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        continue;
                    }
                    throw Client::Exception::ReceiveException("Failed to receive UDP packet", error);
#else
                    if (errno == EWOULDBLOCK || errno == EAGAIN) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        continue;
                    }
                    throw Client::Exception::ReceiveException("Failed to receive UDP packet", errno);
#endif
                }
            }
        } catch (const Client::Exception::ReceiveException &e) {
            // TODO: Handle receive exception (log it, cleanup, etc.)
            _running = false;
        }
    }

} // namespace Network