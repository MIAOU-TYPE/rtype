/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetClient
*/

#include "NetClient.hpp"

namespace Network
{

    NetClient::NetClient()
    {
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
        _serverAddr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &_serverAddr.sin_addr);
        sendConnectPacket();
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

    void NetClient::receivePackets()
    {
        Net::UDPPacket packet;
        sockaddr_in from;
        socklen_t fromLen = sizeof(from);

        while (true) {
            auto bytesReceived = _netWrapper->recvFrom(
                _socket, packet.buffer(), packet.capacity(), 0, reinterpret_cast<struct sockaddr *>(&from), &fromLen);
            if (bytesReceived <= 0) {
                break;
            }
            packet.setSize(static_cast<size_t>(bytesReceived));
            packet.setAddress(from);
            handlePacket(packet);
        }
    }

    void NetClient::handlePacket(const Net::UDPPacket &packet)
    {
        const HeaderData *header = reinterpret_cast<const HeaderData *>(packet.buffer());
        switch (header->type) {
            case Net::Protocol::ACCEPT: {
                _isConnected = true;
                const uint32_t *entityId = reinterpret_cast<const uint32_t *>(packet.buffer() + sizeof(HeaderData));
                _playerEntityId = *entityId;
                DefaultData acceptData;
                acceptData.header = *header;
                {
                    std::lock_guard<std::mutex> lock(_packetDataMutex);
                    _packetDataList.push_back(acceptData);
                }
                break;
            }

            case Net::Protocol::REJECT: {
                _isConnected = false;
                DefaultData rejectData;
                rejectData.header = *header;
                {
                    std::lock_guard<std::mutex> lock(_packetDataMutex);
                    _packetDataList.push_back(rejectData);
                }
                break;
            }

            case Net::Protocol::SNAPSHOT: {
                SnapshotBatchHeader const *batchHeader = reinterpret_cast<SnapshotBatchHeader const *>(packet.buffer());
                SnapshotEntityData const *data =
                    reinterpret_cast<SnapshotEntityData const *>(packet.buffer() + sizeof(SnapshotBatchHeader));
                {
                    std::lock_guard<std::mutex> lock(_packetDataMutex);
                    for (size_t i = 0; i < batchHeader->count; ++i) {
                        _packetDataList.push_back(data[i]);
                    }
                }
                break;
            }

            case Net::Protocol::ENTITY_CREATE: {
                EntityCreateData const *data = reinterpret_cast<EntityCreateData const *>(packet.buffer());
                {
                    std::lock_guard<std::mutex> lock(_packetDataMutex);
                    _packetDataList.push_back(*data);
                }
                break;
            }

            case Net::Protocol::ENTITY_DESTROY: {
                EntityDestroyData const *data = reinterpret_cast<EntityDestroyData const *>(packet.buffer());
                {
                    std::lock_guard<std::mutex> lock(_packetDataMutex);
                    _packetDataList.push_back(*data);
                }
                break;
            }

            case Net::Protocol::PONG: {
                DefaultData pongData;
                pongData.header = *header;
                {
                    std::lock_guard<std::mutex> lock(_packetDataMutex);
                    _packetDataList.push_back(pongData);
                }
                if (_waitingForPong) {
                    _latency = _pingTimer - _lastPingTime;
                    _waitingForPong = false;
                    _missedPongCount = 0;
                }
                break;
            }

            case Net::Protocol::DAMAGE_EVENT: {
                DamageData const *data = reinterpret_cast<DamageData const *>(packet.buffer());
                {
                    std::lock_guard<std::mutex> lock(_packetDataMutex);
                    _packetDataList.push_back(*data);
                }
                break;
            }

            case Net::Protocol::GAME_OVER: {
                DefaultData gameOverData;
                gameOverData.header = *header;
                {
                    std::lock_guard<std::mutex> lock(_packetDataMutex);
                    _packetDataList.push_back(gameOverData);
                }
                break;
            }

            default: break;
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
        if (_isConnected) {
            sendDisconnectPacket();
        }
        if (_socket != kInvalidSocket) {
            _netWrapper->closeSocket(_socket);
            _socket = kInvalidSocket;
        }
#ifdef _WIN32
        WSACleanup();
#endif
    }

    std::vector<PacketData> NetClient::getAndClearPacketData()
    {
        std::lock_guard<std::mutex> lock(_packetDataMutex);
        std::vector<PacketData> data = std::move(_packetDataList);
        _packetDataList.clear();
        return data;
    }

} // namespace Network