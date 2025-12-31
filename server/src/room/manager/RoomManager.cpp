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

    RoomId RoomManager::createRoom() noexcept
    {
        try {
            auto room = std::make_shared<Room>(_sessions, _server, _packetFactory, _levelPath);
            std::scoped_lock lock(_mutex);
            auto id = _nextRoomId++;
            _rooms.emplace(id, room);
            return id;
        } catch (...) {
            return InvalidRoomId;
        }
    }

    void RoomManager::removeRoom(const RoomId roomId) noexcept
    {
        std::shared_ptr<Room> room;

        {
            std::scoped_lock lock(_mutex);
            const auto it = _rooms.find(roomId);
            if (it == _rooms.end())
                return;

            for (auto pit = _playerToRoom.begin(); pit != _playerToRoom.end();) {
                if (pit->second == roomId)
                    pit = _playerToRoom.erase(pit);
                else
                    ++pit;
            }

            room = it->second;
            _rooms.erase(it);
        }

        try {
            room->stop();
        } catch (...) {
            std::cerr << "{RoomManager::removeRoom} failed to stop room " << roomId << std::endl;
        }
    }

    void RoomManager::start(const RoomId roomId) const noexcept
    {
        const auto room = getRoomById(roomId);
        if (!room)
            return;
        try {
            room->start();
        } catch (...) {
            std::cerr << "{RoomManager::start} failed to start room " << roomId << std::endl;
        }
    }

    void RoomManager::addPlayerToRoom(const RoomId roomId, const int sessionId) noexcept
    {
        const auto room = getRoomById(roomId);
        if (!room)
            return;
        try {
            room->join(sessionId);
        } catch (...) {
            return;
        }
        std::scoped_lock lock(_mutex);
        _playerToRoom[sessionId] = roomId;
    }

    void RoomManager::removePlayer(const int sessionId) noexcept
    {
        const auto room = getRoomOfPlayer(sessionId);

        if (!room)
            return;
        try {
            room->leave(sessionId);
            if (room->empty())
                removeRoom(getRoomIdOfPlayer(sessionId));
        } catch (...) {
            return;
        }
        std::scoped_lock lock(_mutex);
        _playerToRoom.erase(sessionId);
    }

    RoomId RoomManager::getRoomIdOfPlayer(const int sessionId) const noexcept
    {
        std::scoped_lock lock(_mutex);
        if (const auto it = _playerToRoom.find(sessionId); it != _playerToRoom.end())
            return it->second;
        return InvalidRoomId;
    }

    std::shared_ptr<Room> RoomManager::getRoomById(const RoomId roomId) const noexcept
    {
        std::scoped_lock lock(_mutex);
        if (const auto it = _rooms.find(roomId); it != _rooms.end())
            return it->second;
        return nullptr;
    }

    std::shared_ptr<Room> RoomManager::getRoomOfPlayer(const int sessionId) const noexcept
    {
        const auto roomId = getRoomIdOfPlayer(sessionId);
        if (roomId == InvalidRoomId)
            return nullptr;
        return getRoomById(roomId);
    }

    void RoomManager::onPlayerConnect(const int sessionId) noexcept
    {
        if (const auto room = getRoomOfPlayer(sessionId)) {
            room->gameServer().onPlayerConnect(sessionId);
            return;
        }
        const auto id = createRoom();
        if (id == InvalidRoomId)
            return;
        addPlayerToRoom(id, sessionId);
        if (const auto room = getRoomById(id)) {
            room->start();
        }
    }

    void RoomManager::onPlayerDisconnect(const int sessionId) noexcept
    {
        if (const auto room = getRoomOfPlayer(sessionId))
            room->gameServer().onPlayerDisconnect(sessionId);
        removePlayer(sessionId);
    }

    void RoomManager::onPlayerInput(const int sessionId, const Game::InputComponent &input) const noexcept
    {
        if (const auto room = getRoomOfPlayer(sessionId))
            room->gameServer().onPlayerInput(sessionId, input);
    }

    void RoomManager::onPing(int sessionId) const noexcept
    {
        if (const auto room = getRoomOfPlayer(sessionId))
            room->gameServer().onPing(sessionId);
    }
} // namespace Engine