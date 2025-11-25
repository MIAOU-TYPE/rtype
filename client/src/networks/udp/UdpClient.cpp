/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UdpClient
*/

#include "UdpClient.hpp"
#include <chrono>
#include <thread>

namespace Network
{

    UdpClient::UdpClient() : ANetworkClient()
    {
        _socketConfig.family = AF_INET;
        _socketConfig.type = SOCK_DGRAM;
        _socketConfig.protocol = IPPROTO_UDP;
    }

    UdpClient::~UdpClient()
    {
        disconnectFromServer();
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
        if (_socketFd >= 0) {
            close(_socketFd);
            _socketFd = -1;
        }
    }

    void UdpClient::sendPacket(const std::vector<uint8_t> &data)
    {
        ssize_t sent = sendto(
            _socketFd, data.data(), data.size(), 0, reinterpret_cast<sockaddr *>(&_serverAddr), sizeof(_serverAddr));

        if (sent < 0) {
            throw Client::Exception::SendException("Failed to send UDP packet", errno);
        }
    }

    void UdpClient::receiveLoop()
    {
        try {
            while (_running) {
                auto packet = std::make_shared<UdpClientPacket>();
                socklen_t senderAddrLen = sizeof(sockaddr_in);
                
                ssize_t receivedBytes = recvfrom(
                    _socketFd, 
                    packet->buffer(), 
                    AClientPacket::MAX_SIZE, 
                    0, 
                    reinterpret_cast<sockaddr *>(packet->address()), 
                    &senderAddrLen
                );
                
                if (receivedBytes > 0) {
                    packet->setSize(receivedBytes);
                    std::lock_guard<std::mutex> lock(_receiveMutex);
                    _receivedPackets.push(packet);
                } else if (receivedBytes < 0) {
                    if (errno == EWOULDBLOCK || errno == EAGAIN) {
                        // Non-blocking socket: no data available, sleep briefly to avoid CPU spinning
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        continue;
                    }
                    throw Client::Exception::ReceiveException("Failed to receive UDP packet", errno);
                }
            }
        } catch (const Client::Exception::ReceiveException &e) {
            // To Do: Handle receive exception (log it, cleanup, etc.)
            _running = false;
        }
    }

} // namespace Network