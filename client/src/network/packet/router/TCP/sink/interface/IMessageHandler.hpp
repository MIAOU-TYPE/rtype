/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IMessageHandler
*/

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "RoomData.hpp"
#include <string_view>

namespace Network
{
    /**
     * @interface IMessageHandler
     * @brief Interface for handling TCP messages from the server.
     * @details This interface defines the methods that must be implemented
     * to handle various TCP messages such as welcome, error, room list,
     * room creation, joining, leaving, and game start notifications.
     */
    class IMessageHandler {
      public:
        /**
         * @brief Virtual destructor for IMessageHandler interface.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~IMessageHandler() = default;

        /**
         * @brief Handle the welcome message from the server.
         * @param req The request ID.
         * @param ver The protocol version.
         * @param sessionId The session ID assigned by the server.
         * @param udpPort The UDP port for communication.
         * @param token The authentication token.
         */
        virtual void onWelcome(std::uint32_t req, std::uint16_t ver, std::uint32_t sessionId, std::uint16_t udpPort,
            std::uint64_t token) = 0;

        /**
         * @brief Handle an error message from the server.
         * @param req The request ID.
         * @param code The error code.
         * @param msg The error message.
         */
        virtual void onError(std::uint32_t req, std::uint16_t code, std::string_view msg) = 0;

        /**
         * @brief Handle the list of rooms received from the server.
         * @param req The request ID.
         * @param rooms The vector of RoomData structures representing the rooms.
         */
        virtual void onRoomsList(std::uint32_t req, const std::vector<RoomData> &rooms) = 0;

        /**
         * @brief Handle the room creation confirmation from the server.
         * @param req The request ID.
         * @param roomId The ID of the created room.
         */
        virtual void onRoomCreated(std::uint32_t req, std::uint32_t roomId) = 0;

        /**
         * @brief Handle the room joining confirmation from the server.
         * @param req The request ID.
         * @param roomId The ID of the joined room.
         */
        virtual void onRoomJoined(std::uint32_t req, std::uint32_t roomId) = 0;

        /**
         * @brief Handle the room leaving confirmation from the server.
         * @param req The request ID.
         * @param roomId The ID of the left room.
         */
        virtual void onRoomLeft(std::uint32_t req, std::uint32_t roomId) = 0;

        /**
         * @brief Handle the game start notification from the server.
         * @param req The request ID.
         * @param roomId The ID of the room where the game is starting.
         */
        virtual void onGameStart(std::uint32_t req, std::uint32_t roomId) = 0;

        /**
         * @brief Handle protocol errors.
         * @param req The request ID.
         * @param msg The error message.
         */
        virtual void onProtocolError(std::uint32_t req, std::string_view msg) = 0;
    };
} // namespace Network