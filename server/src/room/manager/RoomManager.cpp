/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomManager
*/

#include "RoomManager.hpp"

namespace Engine
{
    RoomManager::RoomManager(std::shared_ptr<Net::Server::ISessionManager> sessions,
        std::shared_ptr<Net::Server::IServer> server, std::shared_ptr<Net::Factory::PacketFactory> packetFactory,
        std::string levelPath)
        : _sessions(std::move(sessions)), _server(std::move(server)), _packetFactory(std::move(packetFactory)),
          _levelPath(std::move(levelPath))
    {
    }

    RoomId RoomManager::createRoom()
    {
        RoomId id = _nextRoomId++;

        auto room = std::make_unique<Room>(_sessions, _server, _packetFactory, _levelPath);
        room->start();
        _rooms.emplace(id, std::move(room));

        return id;
    }

    void RoomManager::removeRoom(const RoomId roomId)
    {
        std::scoped_lock lock(_mutex);

        const auto it = _rooms.find(roomId);
        if (it == _rooms.end())
            return;

        auto &room = *(it->second);
        room.stop();
        _rooms.erase(it);
    }

    void RoomManager::addPlayerToRoom(const RoomId roomId, const int sessionId)
    {
        std::scoped_lock lock(_mutex);

        auto &room = *_rooms.at(roomId);

        room.join(sessionId);
        _playerToRoom[sessionId] = roomId;
    }

    void RoomManager::removePlayer(const int sessionId)
    {
        std::scoped_lock lock(_mutex);

        const auto it = _playerToRoom.find(sessionId);
        if (it == _playerToRoom.end())
            return;

        const RoomId roomId = it->second;
        auto &room = *_rooms.at(roomId);

        room.leave(sessionId);
        _playerToRoom.erase(it);

        if (room.empty()) {
            room.stop();
            _rooms.erase(roomId);
        }
    }

    RoomId RoomManager::getRoomOfPlayer(const int sessionId) const
    {
        std::scoped_lock lock(_mutex);
        return _playerToRoom.at(sessionId);
    }

    Room &RoomManager::getRoom(const RoomId roomId) const
    {
        std::scoped_lock lock(_mutex);
        return *_rooms.at(roomId);
    }

    void RoomManager::onPlayerConnect(const int sessionId)
    {
        RoomId roomId;

        {
            std::scoped_lock lock(_mutex);
            if (_rooms.empty())
                roomId = createRoom();
            else
                roomId = _rooms.begin()->first;
        }
        addPlayerToRoom(roomId, sessionId);
    }

    void RoomManager::onPlayerInput(const int sessionId, const Game::InputComponent &input) const
    {
        const Room *room = nullptr;

        {
            std::scoped_lock lock(_mutex);
            const RoomId roomId = _playerToRoom.at(sessionId);
            room = _rooms.at(roomId).get();
        }

        room->gameServer().onPlayerInput(sessionId, input);
    }

    void RoomManager::onPlayerDisconnect(const int sessionId)
    {
        removePlayer(sessionId);
    }

    void RoomManager::onPing(const int sessionId) const
    {
        const Room *room = nullptr;

        {
            std::scoped_lock lock(_mutex);
            const RoomId roomId = _playerToRoom.at(sessionId);
            room = _rooms.at(roomId).get();
        }

        room->gameServer().onPing(sessionId);
    }
} // namespace Engine