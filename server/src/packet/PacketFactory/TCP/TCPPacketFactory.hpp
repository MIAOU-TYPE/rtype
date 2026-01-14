/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** TCPPacketFactory
*/

#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string_view>

#include "IPacket.hpp"
#include "TCPPayload.hpp"

#include <cstring>
#include "RoomData.hpp"
#include "TCPTypesData.hpp"
#include "ScoreEntry.hpp"
#include "TCPWriter.hpp"

namespace Net::Factory
{
    /**
     * @class TCPPacketFactory
     * @brief Factory class for creating TCP packets.
     */
    class TCPPacketFactory {
      public:
        /**
         * @brief Type alias for request ID.
         * @typedef ReqId
         */
        using ReqId = uint32_t;

        /**
         * @brief Type alias for room ID.
         * @typedef RoomId
         */
        using RoomId = uint32_t;

        /**
         * @brief Type alias for session ID.
         * @typedef SessionId
         */
        using SessionId = uint32_t;

        /**
         * @brief Constructor for TCPPacketFactory.
         * @param proto A shared pointer to an IPacket used as a template for creating packets.
         */
        explicit TCPPacketFactory(const std::shared_ptr<IPacket> &proto);

        /**
         * @brief Creates a WELCOME packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param ver The protocol version.
         * @param sessionId The session ID.
         * @param udpPort The UDP port number.
         * @param token The authentication token.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeWelcome(const sockaddr_in &addr, ReqId req, uint16_t ver,
            SessionId sessionId, uint16_t udpPort, uint64_t token) const;

        /**
         * @brief Creates a ROOMS_LIST packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param rooms A vector of RoomInfo structures representing the rooms.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeRoomsList(
            const sockaddr_in &addr, ReqId req, const std::vector<RoomData> &rooms) const;

        /**
         * @brief Creates a ROOM_CREATED packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param roomId The ID of the created room.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeRoomCreated(const sockaddr_in &addr, ReqId req, RoomId roomId) const;

        /**
         * @brief Creates a ROOM_JOINED packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param roomId The ID of the joined room.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeRoomJoined(const sockaddr_in &addr, ReqId req, RoomId roomId) const;

        /**
         * @brief Creates a ROOM_LEFT packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param roomId The ID of the left room.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeRoomLeft(const sockaddr_in &addr, ReqId req, RoomId roomId) const;

        /**
         * @brief Creates a GAME_START packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param roomId The ID of the room where the game is starting.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeGameStart(const sockaddr_in &addr, ReqId req, RoomId roomId) const;

        /**
         * @brief Creates an ERROR_MESSAGE packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param code The error code.
         * @param msg The error message.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeError(
            const sockaddr_in &addr, ReqId req, uint16_t code, std::string_view msg) const;

        /**
         * @brief Creates an AUTH_OK packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param userId The user ID.
         * @param username The username of the authenticated user.
         * @param token
         * @param ttlSec
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeAuthOk(const sockaddr_in &addr, ReqId req, uint32_t userId,
            std::string_view username, uint64_t token, uint32_t ttlSec) const;

        /**
         * @brief Creates a SCOREBOARD_LIST packet.
         * @param addr The address of the client.
         * @param req The request ID.
         * @param scores A vector of ScoreEntry structures representing the scores.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeScoreboardList(
            const sockaddr_in &addr, ReqId req, const std::vector<ScoreEntry> &scores) const;

      private:
        /**
         * @brief Helper method to create a packet with the given address and payload.
         * @param addr The address of the client.
         * @param payload The payload data for the packet.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> make(const sockaddr_in &addr, const std::vector<uint8_t> &payload) const;

        std::shared_ptr<IPacket> _packet = nullptr; ///> Template packet for creating new packets
    };
} // namespace Net::Factory
