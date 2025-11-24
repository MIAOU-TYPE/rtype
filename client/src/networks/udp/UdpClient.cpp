/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UdpClient
*/

#include "UdpClient.hpp"

namespace Network
{

    UdpClient::UdpClient()
    {
    }

    UdpClient::~UdpClient()
    {
        disconnectFromServer();
    }

    void UdpClient::connectToServer(const std::string &ip, uint16_t port)
    {
        _socketFd = socket(AF_INET, SOCK_DGRAM, 0);

        if (_socketFd < 0) {
            throw Client::Exception::SocketException("Failed to create UDP socket", errno);
        }

        memset(&_serverAddr, 0, sizeof(_serverAddr));
        _serverAddr.sin_family = AF_INET;
        _serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &_serverAddr.sin_addr) <= 0) {
            throw Client::Exception::ConnectionException("Invalid server IP address : " + ip);
        }

        _running = true;
        _receiverThread = std::thread(&UdpClient::receiveLoop, this);
    }

    void UdpClient::disconnectFromServer()
    {
        _running = false;
        if (_receiverThread.joinable()) {
            _receiverThread.join();
        }
        close(_socketFd);
    }

    void UdpClient::sendPacket(const std::vector<uint8_t> &data)
    {
        ssize_t sent = sendto(
            _socketFd, data.data(), data.size(), 0, reinterpret_cast<sockaddr *>(&_serverAddr), sizeof(_serverAddr));

        if (sent < 0) {
            throw Client::Exception::SendException("Failed to send UDP packet", errno);
        }
    }

    std::vector<std::vector<uint8_t>> UdpClient::poll()
    {
        std::vector<std::vector<uint8_t>> packets;
        std::lock_guard<std::mutex> lock(_receiveMutex);
        while (!_receivedPackets.empty()) {
            packets.push_back(_receivedPackets.front());
            _receivedPackets.pop();
        }
        return packets;
    }

    void UdpClient::receiveLoop()
    {
        try {
            while (_running) {
                uint8_t buffer[2048];
                sockaddr_in senderAddr;
                socklen_t senderAddrLen = sizeof(senderAddr);
                ssize_t receivedBytes = recvfrom(
                    _socketFd, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr *>(&senderAddr), &senderAddrLen);
                if (receivedBytes > 0) {
                    std::vector<uint8_t> packet(buffer, buffer + receivedBytes);
                    std::lock_guard<std::mutex> lock(_receiveMutex);
                    _receivedPackets.push(packet);
                } else if (receivedBytes < 0) {
                    if (errno == EWOULDBLOCK || errno == EAGAIN) {
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

    bool UdpClient::isConnected() const
    {
        return _running && _socketFd >= 0;
    }

} // namespace Network