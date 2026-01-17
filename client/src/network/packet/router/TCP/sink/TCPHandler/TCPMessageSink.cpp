/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** TCPMessageSink
*/

#include "TCPMessageSink.hpp"

namespace Network
{
    void TCPMessageSink::onWelcomeSubscribe(WelcomeCb cb)
    {
        _welcomeCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onErrorSubscribe(ErrorCb cb)
    {
        _errorCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onRoomsListSubscribe(RoomsListCb cb)
    {
        _roomsListCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onRoomCreatedSubscribe(RoomIdCb cb)
    {
        _roomCreatedCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onRoomJoinedSubscribe(RoomIdCb cb)
    {
        _roomJoinedCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onRoomLeftSubscribe(RoomIdCb cb)
    {
        _roomLeftCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onGameStartSubscribe(RoomIdCb cb)
    {
        _gameStartCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onProtocolErrorSubscribe(ProtoErrCb cb)
    {
        _protocolErrorCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onAuthOkSubscribe(AuthOkCb cb)
    {
        _authOkCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onScoreboardListSubscribe(ScoreboardCb cb)
    {
        _scoreboardCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onRoomUpdatedSubscribe(RoomUpdatedCb cb)
    {
        _roomUpdatedCbs.emplace_back(std::move(cb));
    }

    void TCPMessageSink::onWelcome(
        const uint32_t req, const uint16_t ver, const uint32_t sessionId, const uint16_t udpPort, const uint64_t token)
    {
        _isConnected = true;
        _connectData = {sessionId, token, udpPort};

        emit(_welcomeCbs, req, ver, sessionId, udpPort, token);
    }

    void TCPMessageSink::onError(const uint32_t req, const uint16_t code, const std::string_view msg)
    {
        emit(_errorCbs, req, code, msg);
    }

    void TCPMessageSink::onRoomsList(const uint32_t req, const std::vector<RoomData> &rooms)
    {
        _rooms = rooms;

        emit(_roomsListCbs, req, _rooms);
    }

    void TCPMessageSink::onRoomCreated(const uint32_t req, const uint32_t roomId)
    {
        emit(_roomCreatedCbs, req, roomId);
    }

    void TCPMessageSink::onRoomJoined(const uint32_t req, const uint32_t roomId)
    {
        emit(_roomJoinedCbs, req, roomId);
    }

    void TCPMessageSink::onRoomLeft(const uint32_t req, const uint32_t roomId)
    {
        _isConnected = false;
        _connectData = {0, 0, 0};

        emit(_roomLeftCbs, req, roomId);
    }

    void TCPMessageSink::onGameStart(const uint32_t req, const uint32_t roomId)
    {
        emit(_gameStartCbs, req, roomId);
    }

    void TCPMessageSink::onProtocolError(const uint32_t req, const std::string_view msg)
    {
        emit(_protocolErrorCbs, req, msg);
    }

    void TCPMessageSink::onAuthOk(const uint32_t req, const uint32_t userId, const std::string_view username,
        const uint64_t token, const uint32_t ttlSec)
    {
        if (!_isConnected) {
            emit(_protocolErrorCbs, req, "AUTH_OK received while not connected");
            return;
        }
        if (token == 0) {
            emit(_protocolErrorCbs, req, "AUTH_OK token == 0");
            return;
        }
        _identity = Identity{userId, std::string(username)};
        _connectData.token = token;
        emit(_authOkCbs, req, userId, _identity->username, token, ttlSec);
    }

    void TCPMessageSink::onScoreboardList(uint32_t req, const std::vector<ScoreEntry> &scores)
    {
        emit(_scoreboardCbs, req, scores);
    }

    void TCPMessageSink::onRoomUpdated(uint32_t req, const RoomData &room)
    {
        emit(_roomUpdatedCbs, req, room);
    }

    ConnectInfo TCPMessageSink::getConnectInfo() const noexcept
    {
        return _connectData;
    }

    bool TCPMessageSink::isConnected() const noexcept
    {
        return _isConnected;
    }
} // namespace Network
