/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** IMessageHandler
*/

#pragma once
#include <cstdint>
#include <functional>
#include "ConnectData.hpp"
#include "RoomData.hpp"

namespace Network
{
    /**
     * @interface IMessageHandler
     * @brief Interface for handling various network messages.
     */
    class IMessageHandler {
      public:
        /**
         * @brief Virtual destructor for IMessageHandler interface.
         * Ensures proper cleanup of derived classes.
         * @note Always declare destructors in interfaces as virtual.
         */
        virtual ~IMessageHandler() = default;

        /**
         * @brief onWelcome is called when a welcome message is received.
         * @param req The request ID.
         * @param ver The protocol version.
         * @param sessionId The session ID assigned to the client.
         * @param udpPort The UDP port assigned to the client.
         * @param token The authentication token.
         */
        virtual void onWelcome(std::uint32_t req, std::uint16_t ver, std::uint32_t sessionId, std::uint16_t udpPort,
            std::uint64_t token) = 0;

        /**
         * @brief onError is called when an error message is received.
         * @param req The request ID.
         * @param code The error code.
         * @param msg The error message.
         */
        virtual void onError(std::uint32_t req, std::uint16_t code, std::string_view msg) = 0;

        /**
         * @brief onRoomsList is called when a list of rooms is received.
         * @param req The request ID.
         * @param rooms The list of rooms.
         */
        virtual void onRoomsList(std::uint32_t req, const std::vector<RoomData> &rooms) = 0;

        /**
         * @brief onRoomCreated is called when a room is created.
         * @param req The request ID.
         * @param roomId The ID of the created room.
         */
        virtual void onRoomCreated(std::uint32_t req, std::uint32_t roomId) = 0;

        /**
         * @brief onRoomJoined is called when a room is joined.
         * @param req The request ID.
         * @param roomId The ID of the joined room.
         */
        virtual void onRoomJoined(std::uint32_t req, std::uint32_t roomId) = 0;

        /**
         * @brief onRoomLeft is called when a room is left.
         * @param req The request ID.
         * @param roomId The ID of the left room.
         */
        virtual void onRoomLeft(std::uint32_t req, std::uint32_t roomId) = 0;

        /**
         * @brief onGameStart is called when a game starts.
         * @param req The request ID.
         * @param roomId The ID of the room where the game starts.
         */
        virtual void onGameStart(std::uint32_t req, std::uint32_t roomId) = 0;

        /**
         * @brief onProtocolError is called when a protocol error occurs.
         * @param req The request ID.
         * @param msg The error message.
         */
        virtual void onProtocolError(std::uint32_t req, std::string_view msg) = 0;

        using WelcomeCb = std::function<void(std::uint32_t, std::uint16_t, std::uint32_t, std::uint16_t,
            std::uint64_t)>; ///> Callback type for welcome messages
        using ErrorCb =
            std::function<void(std::uint32_t, std::uint16_t, std::string_view)>; ///> Callback type for error messages
        using RoomsListCb = std::function<void(
            std::uint32_t, const std::vector<RoomData> &)>;                 ///> Callback type for rooms list messages
        using RoomIdCb = std::function<void(std::uint32_t, std::uint32_t)>; ///> Callback type for room ID messages
        using ProtoErrCb =
            std::function<void(std::uint32_t, std::string_view)>; ///> Callback type for protocol error messages

        /**
         * @brief Subscribe to welcome message events.
         * @param cb The callback function to be invoked on welcome messages.
         */
        virtual void onWelcomeSubscribe(WelcomeCb cb) = 0;

        /**
         * @brief Subscribe to error message events.
         * @param cb The callback function to be invoked on error messages.
         */
        virtual void onErrorSubscribe(ErrorCb cb) = 0;

        /**
         * @brief Subscribe to rooms list message events.
         * @param cb The callback function to be invoked on rooms list messages.
         */
        virtual void onRoomsListSubscribe(RoomsListCb cb) = 0;

        /**
         * @brief Subscribe to room created message events.
         * @param cb The callback function to be invoked on room created messages.
         */
        virtual void onRoomCreatedSubscribe(RoomIdCb cb) = 0;

        /**
         * @brief Subscribe to room joined message events.
         * @param cb The callback function to be invoked on room joined messages.
         */
        virtual void onRoomJoinedSubscribe(RoomIdCb cb) = 0;

        /**
         * @brief Subscribe to room left message events.
         * @param cb The callback function to be invoked on room left messages.
         */
        virtual void onRoomLeftSubscribe(RoomIdCb cb) = 0;

        /**
         * @brief Subscribe to game start message events.
         * @param cb The callback function to be invoked on game start messages.
         */
        virtual void onGameStartSubscribe(RoomIdCb cb) = 0;

        /**
         * @brief Subscribe to protocol error message events.
         * @param cb The callback function to be invoked on protocol error messages.
         */
        virtual void onProtocolErrorSubscribe(ProtoErrCb cb) = 0;

        /**
         * @brief Get the connection information.
         * @return The connection information.
         */
        [[nodiscard]] virtual ConnectInfo getConnectInfo() const noexcept = 0;

        /**
         * @brief Check if the client is connected.
         * @return True if connected, false otherwise.
         */
        [[nodiscard]] virtual bool isConnected() const noexcept = 0;
    };
} // namespace Network