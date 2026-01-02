/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomManager
*/

#pragma once

#include <memory>
#include <mutex>
#include <utility>
#include <unordered_map>

#include "Room.hpp"

namespace Engine
{
    /**
     * @class RoomManager
     * @brief Manages game rooms and player assignments
     */
    class RoomManager {
      public:
        /**
         * @struct RoomEntry
         * @brief Represents a summary of a game room
         */
        struct RoomEntry {
            RoomId id;             ///> Unique identifier for the room
            std::string name;      ///> Name of the room
            size_t currentPlayers; ///> Current number of players in the room
            size_t maxPlayers;     ///> Maximum number of players allowed in the room
        };

        /**
         * @brief Constructor for RoomManager
         * @param sessions shared pointer to the session manager
         * @param server shared pointer to the server
         * @param UDPPacketFactory shared pointer to the packet factory
         * @param levelPath path to the game level data
         */
        RoomManager(std::shared_ptr<Net::Server::ISessionManager> sessions,
            std::shared_ptr<Net::Server::IServer> server,
            std::shared_ptr<Net::Factory::UDPPacketFactory> UDPPacketFactory, std::string levelPath);

        /**
         * @brief Creates a new game room
         * @return The ID of the newly created room
         */
        [[nodiscard]] RoomId createRoom(const std::string &name, size_t maxPlayers) noexcept;

        /**
         * @brief Removes a game room
         * @param roomId The ID of the room to be removed
         */
        void removeRoom(RoomId roomId) noexcept;

        /**
         * @brief Starts a game room
         * @param roomId The ID of the room to be started
         */
        [[nodiscard]] bool start(RoomId roomId) const noexcept;

        /**
         * @brief Adds a player to a specified room
         * @param roomId The ID of the room
         * @param sessionId The session ID of the player to be added
         */
        void addPlayerToRoom(RoomId roomId, int sessionId) noexcept;

        /**
         * @brief Removes a player from their assigned room
         * @param sessionId The session ID of the player to be removed
         */
        [[nodiscard]] RoomId removePlayer(int sessionId) noexcept;

        /**
         * @brief Executes a function for each room managed by the RoomManager
         * @tparam Func The type of the function to be executed
         * @param func The function to be executed for each room
         */
        template <typename Func>
        void forEachRoom(Func &&func);

        /**
         * @brief Handles player connection
         * @param sessionId The session ID of the connected player
         */
        void onPlayerConnect(int sessionId) noexcept;

        /**
         * @brief Handles player disconnection
         * @param sessionId The session ID of the disconnected player
         */
        void onPlayerDisconnect(int sessionId) noexcept;

        /**
         * @brief Handles player input
         * @param sessionId The session ID of the player
         * @param input The input component from the player
         */
        void onPlayerInput(int sessionId, const Game::InputComponent &input) const noexcept;

        /**
         * @brief Handles ping from a player
         * @param sessionId The session ID of the player
         */
        void onPing(int sessionId) const noexcept;

        /**
         * @brief Lists all game rooms with their details
         * @return A vector of RoomEntry structures representing the rooms
         */
        std::vector<RoomEntry> listRooms() const noexcept;

        /**
         * @brief Gets the room ID of the room a player is assigned to
         * @param sessionId The session ID of the player
         * @return The ID of the room the player is assigned to
         */
        [[nodiscard]] RoomId getRoomIdOfPlayer(int sessionId) const noexcept;

        /**
         * @brief Gets a reference to a room by its ID
         * @param roomId The ID of the room
         * @return Reference to the Room instance
         */
        [[nodiscard]] std::shared_ptr<Room> getRoomById(RoomId roomId) const noexcept;

      private:
        /**
         * @brief Gets a reference to the room a player is assigned to
         * @param sessionId The session ID of the player
         * @return Reference to the Room instance
         */
        [[nodiscard]] std::shared_ptr<Room> getRoomOfPlayer(int sessionId) const noexcept;

        std::unordered_map<RoomId, std::shared_ptr<Room>>
            _rooms;                                    ///>  Maps room IDs to their corresponding Room instances
        std::unordered_map<int, RoomId> _playerToRoom; ///> Maps session IDs to their corresponding room IDs

        RoomId _nextRoomId = 1;                    ///> Counter for generating unique room IDs
        static constexpr RoomId InvalidRoomId = 0; ///> Constant representing an invalid room ID

        std::shared_ptr<Net::Server::ISessionManager> _sessions; ///> Session manager for handling player sessions
        std::shared_ptr<Net::Server::IServer> _server;           ///> Server instance for network communication
        std::shared_ptr<Net::Factory::UDPPacketFactory>
            _udpPacketFactory;  ///> Packet factory for creating network packets
        std::string _levelPath; ///> Path to the game level data

        mutable std::mutex _mutex; ///> Mutex for synchronizing access to shared resources
    };
} // namespace Engine

#include "RoomManager.tpp"