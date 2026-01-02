/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Room
*/

#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <unordered_set>

#include "GameServer.hpp"

namespace Engine
{
    /**
     * @brief room identifier type
     */
    using RoomId = std::uint32_t;

    /**
     * @class Room
     * @brief Represents a game room managing player sessions and game state
     */
    class Room {
      public:
        /**
         * @brief Constructor for Room
         * @param sessions shared pointer to the session manager
         * @param server shared pointer to the server
         * @param udpPacketFactory shared pointer to the packet factory
         * @param levelPath path to the game level data
         * @param name name of the room
         * @param maxPlayers maximum number of players allowed in the room
         */
        explicit Room(const std::shared_ptr<Net::Server::ISessionManager> &sessions,
            const std::shared_ptr<Net::Server::IServer> &server,
            const std::shared_ptr<Net::Factory::UDPPacketFactory> &udpPacketFactory, const std::string &levelPath,
            std::string name = "room", size_t maxPlayers = 4);

        /**
         * @brief Destructor for Room
         */
        ~Room();

        /**
         * @brief Starts the room's game server thread
         */
        void start();

        /**
         * @brief Stops the room's game server thread
         */
        void stop();

        /**
         * @brief Adds a player session to the room
         * @param sessionId The session ID of the player to be added
         */
        void join(int sessionId);

        /**
         * @brief Removes a player session from the room
         * @param sessionId The session ID of the player to be removed
         */
        void leave(int sessionId);

        /**
         * @brief Checks if the room is empty (no player sessions)
         * @return true if the room has no player sessions, false otherwise
         */
        bool empty() const;

        /**
         * @brief Gets the set of player session IDs in the room
         * @return A constant reference to the set of session IDs
         */
        const std::unordered_set<int> &sessions() const;

        /**
         * @brief Gets a reference to the room's game server
         * @return A reference to the GameServer instance
         */
        Game::GameServer &gameServer() const;

        size_t getCurrentPlayers() const noexcept
        {
            return _sessions.size();
        }

        size_t getMaxPlayers() const noexcept
        {
            return _maxPlayers;
        }

      private:
        /**
         * @brief Main loop for the room's game server
         */
        void run() const;

        std::unordered_set<int> _sessions; ///> Set of player session IDs in the room

        std::unique_ptr<Game::GameServer> _gameServer = nullptr; ///> Unique pointer to the room's game server

        std::atomic<bool> _running{false}; ///> Atomic flag indicating if the room is running
        std::thread _thread;               ///> Thread for the room's game server loop
        size_t _maxPlayers = 0;            ///> Maximum number of players allowed in the room
        std::string _name = "";            ///> Name of the room
    };
} // namespace Engine