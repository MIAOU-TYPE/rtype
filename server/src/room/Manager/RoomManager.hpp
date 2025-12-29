#pragma once

#include <memory>
#include <mutex>
#include <utility>
#include <unordered_map>

#include "Room.hpp"

namespace Engine
{
    /**
     * @brief Type alias for Room IDs
     */
    using RoomId = std::uint32_t;

    /**
     * @class RoomManager
     * @brief Manages game rooms and player assignments
     */
    class RoomManager {
      public:
        /**
         * @brief Constructor for RoomManager
         * @param sessions shared pointer to the session manager
         * @param server shared pointer to the server
         * @param packetFactory shared pointer to the packet factory
         * @param levelPath path to the game level data
         */
        RoomManager(std::shared_ptr<Net::Server::ISessionManager> sessions,
            std::shared_ptr<Net::Server::IServer> server, std::shared_ptr<Net::Factory::PacketFactory> packetFactory,
            std::string levelPath);

        /**
         * @brief Creates a new game room
         * @return The ID of the newly created room
         */
        [[nodiscard]] RoomId createRoom();

        /**
         * @brief Removes a game room
         * @param roomId The ID of the room to be removed
         */
        void removeRoom(RoomId roomId);

        /**
         * @brief Adds a player to a specified room
         * @param roomId The ID of the room
         * @param sessionId The session ID of the player to be added
         */
        void addPlayerToRoom(RoomId roomId, int sessionId);

        /**
         * @brief Removes a player from their assigned room
         * @param sessionId The session ID of the player to be removed
         */
        void removePlayer(int sessionId);

        /**
         * @brief Gets the room ID of the room a player is assigned to
         * @param sessionId The session ID of the player
         * @return The ID of the room the player is assigned to
         */
        [[nodiscard]] RoomId getRoomOfPlayer(int sessionId) const;

        /**
         * @brief Gets a reference to a room by its ID
         * @param roomId The ID of the room
         * @return Reference to the Room instance
         */
        [[nodiscard]] Room &getRoom(RoomId roomId) const;

        /**
         * @brief Executes a function for each room managed by the RoomManager
         * @tparam Func The type of the function to be executed
         * @param func The function to be executed for each room
         */
        template <typename Func>
        void forEachRoom(Func &&func);

        RoomId createRoomUnlocked()
        {
            RoomId id = _nextRoomId++;

            auto room = std::make_unique<Room>(_sessions, _server, _packetFactory, _levelPath);
            room->start();
            _rooms.emplace(id, std::move(room));

            return id;
        }

        void onPlayerConnect(const int sessionId)
        {
            RoomId roomId;

            {
                std::scoped_lock lock(_mutex);
                if (_rooms.empty())
                    roomId = createRoomUnlocked();
                else
                    roomId = createRoomUnlocked();
            }
            addPlayerToRoom(roomId, sessionId);
        }

        void onPlayerInput(const int sessionId, const Game::InputComponent &input)
        {
            const Room *room = nullptr;

            {
                std::scoped_lock lock(_mutex);
                const RoomId roomId = _playerToRoom.at(sessionId);
                room = _rooms.at(roomId).get();
            }

            room->gameServer().onPlayerInput(sessionId, input);
        }

        void onPlayerDisconnect(const int sessionId)
        {
            removePlayer(sessionId);
        }

        void onPing(const int sessionId)
        {
            (void) sessionId;
            std::cout << "[RoomManager] Ping received from session " << sessionId << std::endl;
        }

      private:
        std::unordered_map<RoomId, std::unique_ptr<Room>>
            _rooms;                                          ///>  Maps room IDs to their corresponding Room instances
        std::unordered_map<SessionId, RoomId> _playerToRoom; ///> Maps session IDs to their corresponding room IDs

        RoomId _nextRoomId = 1; ///> Counter for generating unique room IDs

        std::shared_ptr<Net::Server::ISessionManager> _sessions;     ///> Session manager for handling player sessions
        std::shared_ptr<Net::Server::IServer> _server;               ///> Server instance for network communication
        std::shared_ptr<Net::Factory::PacketFactory> _packetFactory; ///> Packet factory for creating network packets
        std::string _levelPath;                                      ///> Path to the game level data

        mutable std::mutex _mutex; ///> Mutex for synchronizing access to shared resources
    };
} // namespace Engine

#include "RoomManager.tpp"