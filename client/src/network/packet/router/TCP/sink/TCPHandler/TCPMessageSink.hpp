/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** TCPMessageSink
*/

#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>
#include "ConnectData.hpp"
#include "IMessageHandler.hpp"
#include "RoomData.hpp"
#include <string_view>

namespace Network
{
    /**
     * @class TCPMessageSink
     * @brief Implementation of IMessageHandler for handling TCP messages.
     */
    class TCPMessageSink final : public IMessageHandler {
      public:
        /**
         * @brief Virtual destructor for TCPMessageSink.
         * Ensures proper cleanup of derived classes.
         * @note Always declare destructors in interfaces as virtual.
         */
        ~TCPMessageSink() override = default;

        /**
         * @brief Subscription methods for various events.
         * @param cb The callback function to be invoked on the respective event.
         */
        void onWelcomeSubscribe(WelcomeCb cb) override;

        /**
         * @brief Subscribe to error message events.
         * @param cb The callback function to be invoked on error messages.
         */
        void onErrorSubscribe(ErrorCb cb) override;

        /**
         * @brief Subscribe to rooms list message events.
         * @param cb The callback function to be invoked on rooms list messages.
         */
        void onRoomsListSubscribe(RoomsListCb cb) override;

        /**
         * @brief Subscribe to room created message events.
         * @param cb The callback function to be invoked on room created messages.
         */
        void onRoomCreatedSubscribe(RoomIdCb cb) override;

        /**
         * @brief Subscribe to room joined message events.
         * @param cb The callback function to be invoked on room joined messages.
         */
        void onRoomJoinedSubscribe(RoomIdCb cb) override;

        /**
         * @brief Subscribe to room left message events.
         * @param cb The callback function to be invoked on room left messages.
         */
        void onRoomLeftSubscribe(RoomIdCb cb) override;

        /**
         * @brief Subscribe to game start message events.
         * @param cb The callback function to be invoked on game start messages.
         */
        void onGameStartSubscribe(RoomIdCb cb) override;

        /**
         * @brief Subscribe to protocol error message events.
         * @param cb The callback function to be invoked on protocol error messages.
         */
        void onProtocolErrorSubscribe(ProtoErrCb cb) override;

        /**
         * @brief Subscribe to authentication success message events.
         * @param cb The callback function to be invoked on authentication success messages.
         */
        void onAuthOkSubscribe(AuthOkCb cb) override;

        /**
         * @brief Event handler methods for various TCP messages.
         * @param req The request ID associated with the message.
         * @param ver The protocol version.
         * @param sessionId The session ID of the client.
         * @param udpPort The UDP port assigned to the client.
         * @param token The authentication token.
         */
        void onWelcome(uint32_t req, uint16_t ver, uint32_t sessionId, uint16_t udpPort, uint64_t token) override;

        /**
         * @brief Event handler for error messages.
         * @param req The request ID associated with the error.
         * @param code The error code.
         * @param msg The error message.
         */
        void onError(uint32_t req, uint16_t code, std::string_view msg) override;

        /**
         * @brief Event handler for rooms list messages.
         * @param req The request ID associated with the message.
         * @param rooms The list of available rooms.
         */
        void onRoomsList(uint32_t req, const std::vector<RoomData> &rooms) override;

        /**
         * @brief Event handler for room created messages.
         * @param req The request ID associated with the message.
         * @param roomId The ID of the created room.
         */
        void onRoomCreated(uint32_t req, uint32_t roomId) override;

        /**
         * @brief Event handler for room joined messages.
         * @param req The request ID associated with the message.
         * @param roomId The ID of the joined room.
         */
        void onRoomJoined(uint32_t req, uint32_t roomId) override;

        /**
         * @brief Event handler for room left messages.
         * @param req The request ID associated with the message.
         * @param roomId The ID of the left room.
         */
        void onRoomLeft(uint32_t req, uint32_t roomId) override;

        /**
         * @brief Event handler for game start messages.
         * @param req The request ID associated with the message.
         * @param roomId The ID of the room where the game starts.
         */
        void onGameStart(uint32_t req, uint32_t roomId) override;

        /**
         * @brief Event handler for protocol error messages.
         * @param req The request ID associated with the error.
         * @param msg The error message.
         */
        void onProtocolError(uint32_t req, std::string_view msg) override;

        /**
         * @brief Event handler for authentication success messages.
         * @param req The request ID associated with the message.
         * @param userId The authenticated user's ID.
         * @param username The authenticated user's username.
         * @param token The authentication token.
         * @param ttlSec The time-to-live in seconds for the authentication token.
         */
        void onAuthOk(
            uint32_t req, uint32_t userId, std::string_view username, uint64_t token, uint32_t ttlSec) override;

        /**
         * @brief Retrieves the current connection information.
         * @return A ConnectInfo structure containing session ID, token, and UDP port.
         */
        [[nodiscard]] ConnectInfo getConnectInfo() const noexcept override;

        /**
         * @brief Checks if the client is currently connected.
         * @return True if connected, false otherwise.
         */
        [[nodiscard]] bool isConnected() const noexcept override;

      private:
        /**
         * @brief Emits events to all registered callbacks.
         * @tparam Cbs The type of the callback container.
         * @tparam Args The types of the arguments to pass to the callbacks.
         * @param cbs The container of callbacks.
         * @param args The arguments to pass to the callbacks.
         */
        template <class Cbs, class... Args>
        static void emit(Cbs &cbs, Args &&...args);

        std::vector<WelcomeCb> _welcomeCbs;        ///> Callbacks for welcome events
        std::vector<ErrorCb> _errorCbs;            ///> Callbacks for error events
        std::vector<RoomsListCb> _roomsListCbs;    ///> Callbacks for rooms list events
        std::vector<RoomIdCb> _roomCreatedCbs;     ///> Callbacks for room created events
        std::vector<RoomIdCb> _roomJoinedCbs;      ///> Callbacks for room joined events
        std::vector<RoomIdCb> _roomLeftCbs;        ///> Callbacks for room left events
        std::vector<RoomIdCb> _gameStartCbs;       ///> Callbacks for game start events
        std::vector<ProtoErrCb> _protocolErrorCbs; ///> Callbacks for protocol error events
        std::vector<AuthOkCb> _authOkCbs;   ///> Callbacks for authentication success events

        bool _isConnected = false;    ///> Connection status
        ConnectInfo _connectData{};   ///> Connection information
        std::vector<RoomData> _rooms; ///> List of available rooms

        /**
         * @brief Structure representing the identity of the authenticated user.
         */
        struct Identity {
            uint32_t userId = 0; ///> User ID
            std::string username;   ///> Username
        };

        std::optional<Identity> _identity{}; ///> Authenticated user identity
    };
} // namespace Network

#include "TCPMessageSink.tpp"