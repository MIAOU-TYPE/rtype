/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** tcpSink
*/

#pragma once
#include <cstdint>
#include <iostream>
#include "ConnectData.hpp"
#include "IMessageHandler.hpp"

namespace Network
{
    /**
     * @class TCPMessageSink
     * @brief Implementation of IMessageHandler to handle TCP messages.
     * @details This class processes various TCP messages such as welcome,
     * error, room list, room creation, joining/leaving rooms, and game start.
     */
    class TCPMessageSink final : public IMessageHandler {
      public:
        /**
         * @brief Destructor for TCPMessageSink.
         */
        ~TCPMessageSink() override = default;

        /**
         * @brief Handle the welcome message from the server.
         * @param req The request ID.
         * @param ver The protocol version.
         * @param sessionId The session ID assigned by the server.
         * @param udpPort The UDP port for communication.
         * @param token The authentication token.
         */
        void onWelcome(std::uint32_t req, std::uint16_t ver, std::uint32_t sessionId, std::uint16_t udpPort,
            std::uint64_t token) override;

        /**
         * @brief Handle error messages from the server.
         * @param req The request ID.
         * @param code The error code.
         * @param msg The error message.
         */
        void onError(std::uint32_t req, std::uint16_t code, std::string_view msg) override;

        /**
         * @brief Handle the list of rooms received from the server.
         * @param req The request ID.
         * @param rooms The list of available rooms.
         */
        void onRoomsList(std::uint32_t req, const std::vector<RoomData> &rooms) override;

        /**
         * @brief Handle the room creation confirmation from the server.
         * @param req The request ID.
         * @param roomId The ID of the created room.
         */
        void onRoomCreated(std::uint32_t req, std::uint32_t roomId) override;

        /**
         * @brief Handle the confirmation of joining a room from the server.
         * @param req The request ID.
         * @param roomId The ID of the joined room.
         */
        void onRoomJoined(std::uint32_t req, std::uint32_t roomId) override;

        /**
         * @brief Handle the confirmation of leaving a room from the server.
         * @param req The request ID.
         * @param roomId The ID of the left room.
         */
        void onRoomLeft(std::uint32_t req, std::uint32_t roomId) override;

        /**
         * @brief Handle the game start notification from the server.
         * @param req The request ID.
         * @param roomId The ID of the room where the game is starting.
         */
        void onGameStart(std::uint32_t req, std::uint32_t roomId) override;

        /**
         * @brief Handle protocol errors.
         * @param req The request ID.
         * @param msg The error message.
         */
        void onProtocolError(std::uint32_t req, std::string_view msg) override;

        /**
         * @brief Get the connection information.
         * @return The ConnectInfo structure containing session ID, token, and UDP port.
         */
        ConnectInfo getConnectInfo() const noexcept;

        /**
         * @brief Check if the client is currently connected.
         * @return True if connected, false otherwise.
         */
        bool isConnected() const;

      private:
        bool _isConnected = false;     ///> Connection status
        ConnectInfo _connectData = {}; ///> Connection information
        std::vector<RoomData> _rooms;  ///> List of available rooms
    };
} // namespace Network