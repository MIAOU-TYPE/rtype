/*
** EPITECH PROJECT, 2026
** RType
** File description:
** AdminConsole.hpp
*/

#pragma once

#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include "RoomManager.hpp"
#include "SessionManager.hpp"
#include <condition_variable>

#ifndef _WIN32
    #include <arpa/inet.h>
    #include <sys/select.h>
    #include <unistd.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

using ShutdownFn = std::function<void()>; ///> Shutdown function type

namespace Net::Admin
{
    /**
     * @class AdminConsole
     * @brief Text-mode administrative console for server management.
     */
    class AdminConsole {
      public:
        /**
         * @brief Constructs an AdminConsole.
         * @param sessions Shared pointer to the session manager.
         * @param rooms Shared pointer to the room manager.
         * @param shutdownFn Function to call for server shutdown.
         */
        AdminConsole(std::shared_ptr<Server::ISessionManager> sessions, std::shared_ptr<Engine::RoomManager> rooms,
            ShutdownFn shutdownFn);

        /**
         * @brief Destructor for AdminConsole.
         */
        ~AdminConsole();

        /**
         * @brief Starts the admin console thread.
         */
        void start();

        /**
         * @brief Stops the admin console thread.
         */
        void stop();

      private:
        /**
         * @brief Main loop for the admin console thread.
         */
        void run();

        /**
         * @brief Prints the help message.
         */
        void printHelp();

        /**
         * @brief Displays server status information.
         */
        void cmdStatus() const;

        /**
         * @brief Lists all active rooms.
         */
        void cmdRooms() const;

        /**
         * @brief Lists all active sessions.
         */
        void cmdSessions() const;

        /**
         * @brief Kicks a user from the server.
         * @param who Session ID or username of the user to kick.
         * @return True if the user was successfully kicked, false otherwise.
         */
        [[nodiscard]] bool cmdKick(const std::string &who) const;

        /**
         * @brief Bans a user from the server.
         * @param who Session ID or username of the user to ban.
         * @param duration Duration of the ban.
         * @return True if the user was successfully banned, false otherwise.
         */
        [[nodiscard]] bool cmdBan(const std::string &who, std::chrono::seconds duration) const;

        /**
         * @brief Kicks a user from a specific room.
         * @param roomId ID of the room.
         * @param who Session ID or username of the user to kick.
         * @return True if the user was successfully kicked from the room, false otherwise.
         */
        [[nodiscard]] bool cmdKickRoom(Engine::RoomId roomId, const std::string &who) const;

        /**
         * @brief Bans a user from a specific room.
         * @param roomId ID of the room.
         * @param who Session ID or username of the user to ban.
         * @return True if the user was successfully banned from the room, false otherwise.
         */
        [[nodiscard]] bool cmdBanRoom(Engine::RoomId roomId, const std::string &who) const;

        /**
         * @brief Unbans a user from a specific room.
         * @param roomId ID of the room.
         * @param who Session ID or username of the user to unban.
         * @return True if the user was successfully unbanned from the room, false otherwise.
         */
        [[nodiscard]] bool cmdUnbanRoom(Engine::RoomId roomId, const std::string &who) const;

        /**
         * @brief Bans an IP address.
         * @param ipStr IP address in string format.
         * @param duration Duration of the ban.
         * @return True if the IP was successfully banned, false otherwise.
         */
        [[nodiscard]] bool cmdBanIp(const std::string &ipStr, std::chrono::seconds duration) const;

        /**
         * @brief Unbans an IP address.
         * @param ipStr IP address in string format.
         * @return True if the IP was successfully unbanned, false otherwise.
         */
        [[nodiscard]] bool cmdUnbanIp(const std::string &ipStr) const;

        /**
         * @brief Lists all banned IP addresses.
         */
        void cmdBans() const;

        /**
         * @brief Converts an IP address from network byte order to string format.
         * @param ipNbo IP address in network byte order.
         * @return IP address as a string.
         */
        [[nodiscard]] static std::string ipToString(uint32_t ipNbo);

        /**
         * @brief Parses an IPv4 address from string format to network byte order.
         * @param s IP address as a string.
         * @param outIpNbo Output parameter for the IP address in network byte order.
         * @return True if parsing was successful, false otherwise.
         */
        [[nodiscard]] static bool parseIPv4(const std::string &s, uint32_t &outIpNbo);

        /**
         * @brief Checks if a string consists entirely of digits.
         * @param s Input string.
         * @return True if the string is all digits, false otherwise.
         */
        [[nodiscard]] static bool isAllDigits(const std::string &s);

        /**
         * @brief Resolves a session ID from a session ID string or username.
         * @param who Session ID string or username.
         * @return Optional session ID if found, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<int> resolveSessionId(const std::string &who) const;

        /**
         * @brief Parses a room ID from a string.
         * @param s Room ID as a string.
         * @return Optional room ID if parsing was successful, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<Engine::RoomId> parseRoomId(const std::string &s) const;

        /**
         * @brief Resolves a username from a session ID string or username.
         * @param who Session ID string or username.
         * @return Optional username if found, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<std::string> resolveUsername(const std::string &who) const;

        std::shared_ptr<Server::ISessionManager> _sessions; ///> Session manager
        std::shared_ptr<Engine::RoomManager> _rooms;        ///> Room manager
        ShutdownFn _shutdown;                               ///> Function to call for server shutdown

        std::atomic<bool> _running{false}; ///> Flag indicating if the console is running
        std::thread _thread;               ///> Thread for the admin console
    };
} // namespace Net::Admin
