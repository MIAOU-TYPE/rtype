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
        std::shared_ptr<Net::Server::IServer> server, std::shared_ptr<Net::Factory::UDPPacketFactory> udpPacketFactory,
        std::string levelPath)
        : _sessions(std::move(sessions)), _server(std::move(server)), _udpPacketFactory(std::move(udpPacketFactory)),
          _levelPath(std::move(levelPath))
    {
    }

    RoomId RoomManager::createRoom(const std::string &name, size_t maxPlayers) noexcept
    {
        try {
            auto room = std::make_shared<Room>(_sessions, _server, _udpPacketFactory, _levelPath, name, maxPlayers);
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

    bool RoomManager::start(const RoomId roomId) const noexcept
    {
        const auto room = getRoomById(roomId);
        if (!room || room->getCurrentPlayers() != room->getMaxPlayers())
            return false;
        try {
            room->start();
        } catch (...) {
            std::cerr << "{RoomManager::start} failed to start room " << roomId << std::endl;
            return false;
        }
        return true;
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

    RoomId RoomManager::removePlayer(const int sessionId) noexcept
    {
        const auto room = getRoomOfPlayer(sessionId);
        auto roomId = InvalidRoomId;

        if (!room)
            return roomId;
        try {
            roomId = getRoomIdOfPlayer(sessionId);
            room->leave(sessionId);
            if (room->empty())
                removeRoom(roomId);
        } catch (...) {
            return InvalidRoomId;
        }
        std::scoped_lock lock(_mutex);
        _playerToRoom.erase(sessionId);
        return roomId;
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
        const auto id = createRoom("basic", 4);
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
        (void) removePlayer(sessionId);
    }

    void RoomManager::onPlayerInput(const int sessionId, const Game::InputComponent &input) const noexcept
    {
        if (const auto room = getRoomOfPlayer(sessionId))
            room->gameServer().onPlayerInput(sessionId, input);
    }

    void RoomManager::onPing(const int sessionId) const noexcept
    {
        if (const auto room = getRoomOfPlayer(sessionId))
            room->gameServer().onPing(sessionId);
    }

    std::vector<RoomManager::RoomEntry> RoomManager::listRooms() const noexcept
    {
        try {
            std::vector<RoomEntry> roomsList;
            std::scoped_lock lock(_mutex);

            for (const auto &[id, room] : _rooms) {
                RoomEntry entry;
                entry.id = id;
                entry.name = room->getName();
                entry.currentPlayers = room->getCurrentPlayers();
                entry.maxPlayers = room->getMaxPlayers();
                roomsList.push_back(entry);
            }
            return roomsList;
        } catch (...) {
            std::cerr << "{RoomManager::listRooms} failed to list rooms" << std::endl;
            return {};
        }
    }
} // namespace Engine