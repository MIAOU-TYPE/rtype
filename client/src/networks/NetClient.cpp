/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetClient
*/

#include "NetClient.hpp"

namespace Game
{

    NetClient::NetClient(std::shared_ptr<Command::CommandBuffer<NetworkCommand>> commandBuffer)
        : _commandBuffer(std::move(commandBuffer))
    {
        if (!_commandBuffer) {
            throw NetClientError("CommandBuffer cannot be null");
        }
#ifdef _WIN32
        WSADATA wsa;
        const int r = WSAStartup(MAKEWORD(2, 2), &wsa);
        if (r != 0) {
            throw NetClientError("WSAStartup failed with error: " + std::to_string(r));
        }
#endif
        _netWrapper = std::make_unique<Net::NetWrapper>("NetPluginLib");
        _socket = _netWrapper->socket(AF_INET, SOCK_DGRAM, 0);
        if (_socket == kInvalidSocket) {
#ifdef _WIN32
            WSACleanup();
#endif
            throw NetClientError("Failed to create UDP socket");
        }
        _serverAddr.sin_family = AF_INET;
        _serverAddr.sin_port = htons(4242);
        inet_pton(AF_INET, "127.0.0.1", &_serverAddr.sin_addr);
        sendConnectPacket();
        startReceptionThread();
    }

    NetClient::~NetClient()
    {
        close();
    }

    void NetClient::sendConnectPacket()
    {
        HeaderData header;
        header.type = Net::Protocol::CONNECT;
        header.version = 1;
        header.size = sizeof(HeaderData);

        _netWrapper->sendTo(_socket, &header, sizeof(header), 0, reinterpret_cast<struct sockaddr *>(&_serverAddr),
            sizeof(_serverAddr));
    }

    void NetClient::sendInputPacket(const int8_t dx, const int8_t dy, const bool shooting)
    {
        if (!_isConnected)
            return;
        InputData inputData;
        inputData.header.type = Net::Protocol::INPUT;
        inputData.header.version = 1;
        inputData.header.size = sizeof(InputData);
        inputData.entity = _playerEntityId;
        inputData.dx = dx;
        inputData.dy = dy;
        inputData.shooting = shooting ? 1 : 0;
        _netWrapper->sendTo(_socket, &inputData, sizeof(inputData), 0,
            reinterpret_cast<struct sockaddr *>(&_serverAddr), sizeof(_serverAddr));
    }

    void NetClient::startReceptionThread()
    {
        if (_running.load()) {
            return; // Already running
        }
        _running.store(true);
        _rxThread = std::thread(&NetClient::receptionLoop, this);
    }

    void NetClient::stopReceptionThread()
    {
        _running.store(false);

        // Close the socket to unblock recvfrom()
        if (_socket != kInvalidSocket && _netWrapper) {
            _netWrapper->closeSocket(_socket);
            _socket = kInvalidSocket;
        }

        // Join the thread if joinable
        if (_rxThread.joinable()) {
            _rxThread.join();
        }
    }

    void NetClient::receptionLoop()
    {
        Net::UDPPacket packet;
        sockaddr_in from{};
        socklen_t fromLen = sizeof(from);

        while (_running.load()) {
            // Blocking recvfrom() - will be unblocked when socket is closed
            auto bytesReceived = _netWrapper->recvFrom(
                _socket, packet.buffer(), packet.capacity(), 0, reinterpret_cast<struct sockaddr *>(&from), &fromLen);

            // Check if we should stop (socket closed or error)
            if (bytesReceived <= 0) {
                // If we're not running anymore, this is expected (clean shutdown)
                if (!_running.load()) {
                    break;
                }
                // Otherwise, it's an error or timeout - continue waiting
                continue;
            }

            // Set packet metadata
            packet.setSize(static_cast<size_t>(bytesReceived));
            packet.setAddress(from);

            // Minimal validation: check if we have at least a header
            if (packet.size() < sizeof(HeaderData)) {
                continue; // Drop malformed packet (UDP rules)
            }

            // Encapsulate packet into NetworkCommand and push to CommandBuffer
            NetworkCommand cmd(packet);

            // Push to CommandBuffer (non-blocking, drop if fails per UDP rules)
            _commandBuffer->push(cmd);
        }
    }

    void NetClient::sendDisconnectPacket()
    {
        if (!_isConnected)
            return;
        HeaderData header;
        header.type = Net::Protocol::DISCONNECT;
        header.version = 1;
        header.size = sizeof(HeaderData);

        _netWrapper->sendTo(_socket, &header, sizeof(header), 0, reinterpret_cast<struct sockaddr *>(&_serverAddr),
            sizeof(_serverAddr));
        _isConnected = false;
    }

    void NetClient::sendPingPacket()
    {
        if (!_isConnected)
            return;
        HeaderData header;
        header.type = Net::Protocol::PING;
        header.version = 1;
        header.size = sizeof(HeaderData);

        _netWrapper->sendTo(_socket, &header, sizeof(header), 0, reinterpret_cast<struct sockaddr *>(&_serverAddr),
            sizeof(_serverAddr));
    }

    void NetClient::updatePing(float deltaTime)
    {
        if (!_isConnected)
            return;

        _pingTimer += deltaTime;

        if (_waitingForPong && (_pingTimer - _lastPingTime) >= PONG_TIMEOUT) {
            _missedPongCount++;
            _waitingForPong = false;

            if (_missedPongCount >= MAX_MISSED_PONGS) {
                _isConnected = false;
                return;
            }
        }

        if (_pingTimer - _lastPingTime >= PING_INTERVAL) {
            _lastPingTime = _pingTimer;
            _waitingForPong = true;
            sendPingPacket();
        }
    }

    float NetClient::getLatency() const
    {
        return _latency;
    }

    bool NetClient::isConnected() const
    {
        return _isConnected;
    }

    void NetClient::close()
    {
        // Send disconnect packet before stopping the thread
        if (_isConnected) {
            sendDisconnectPacket();
        }

        // Stop the reception thread (this will also close the socket)
        stopReceptionThread();

#ifdef _WIN32
        WSACleanup();
#endif
    }

} // namespace Game