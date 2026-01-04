/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** tcpSink
*/

#include "TCPMessageSink.hpp"

namespace Network
{
    void TCPMessageSink::onWelcome(const std::uint32_t req, const std::uint16_t ver, const std::uint32_t sessionId,
        const std::uint16_t udpPort, const std::uint64_t token)
    {
        std::cout << "{TCPMessageSink::onWelcome}: request: " << req << " version: " << ver
                  << " sessionId: " << sessionId << " port: " << udpPort << " token: " << token << std::endl;
        _isConnected = true;
        _connectData = {sessionId, token, udpPort};
    }

    void TCPMessageSink::onError(const std::uint32_t req, const std::uint16_t code, const std::string_view msg)
    {
        std::cout << "{TCPMessageSink::onError}: request " << req << " code: " << code << "  msg: " << msg << std::endl;
    }

    void TCPMessageSink::onRoomsList(const std::uint32_t req, const std::vector<RoomData> &rooms)
    {
        _rooms = rooms;
        std::cout << "{TCPMessageSink::onRoomsList} request " << req;
        for (const auto &[roomId, roomName, currentPlayers, maxPlayers] : rooms) {
            std::cout << " \n\t - Room ID: " << roomId << ", Name: " << roomName << ", Players: " << currentPlayers
                      << "/" << maxPlayers;
        }
        std::cout << std::endl;
    }

    void TCPMessageSink::onRoomCreated(const std::uint32_t req, const std::uint32_t roomId)
    {
        std::cout << "{TCPMessageSink::onRoomCreated} request " << req << " roomId: " << roomId << std::endl;
    }

    void TCPMessageSink::onRoomJoined(const std::uint32_t req, const std::uint32_t roomId)
    {
        std::cout << "{TCPMessageSink::onRoomJoined} request " << req << " roomId: " << roomId << std::endl;
    }

    void TCPMessageSink::onRoomLeft(const std::uint32_t req, const std::uint32_t roomId)
    {
        std::cout << "{TCPMessageSink::onRoomLeft} request " << req << " roomId: " << roomId << std::endl;
        _isConnected = false;
        _connectData = {0, 0, 0};
    }

    void TCPMessageSink::onGameStart(const std::uint32_t req, const std::uint32_t roomId)
    {
        std::cout << "{TCPMessageSink::onGameStart} request " << req << " roomId: " << roomId << std::endl;
    }

    void TCPMessageSink::onProtocolError(const std::uint32_t req, const std::string_view msg)
    {
        std::cout << "{TCPMessageSink::onProtocolError} request " << req << " msg: " << msg << std::endl;
    }

    ConnectInfo TCPMessageSink::getConnectInfo() const noexcept
    {
        return _connectData;
    }

    bool TCPMessageSink::isConnected() const
    {
        return _isConnected;
    }
} // namespace Network