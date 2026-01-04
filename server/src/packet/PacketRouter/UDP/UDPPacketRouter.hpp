/*
** EPITECH PROJECT, 2025
** RType
** File description:
** UDPPacketRouter
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include <iostream>
#include "IPacket.hpp"
#include "InputData.hpp"
#include "RoomManager.hpp"
#include "SessionManager.hpp"
#include "UDPPacketFactory.hpp"

#ifndef _WIN32
    #include <arpa/inet.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

/**
 * @class UDPPacketRouter
 * @brief Routes incoming packets to appropriate handlers based on packet type.
 *
 * The UDPPacketRouter class is responsible for processing incoming packets,
 * validating their headers, and routing them to the correct handler functions
 * based on the packet type. It interacts with a SessionManager to manage
 * player sessions and an IMessageSink to notify about various events such as
 * player connections, disconnections, inputs, and pings.
 */
namespace Net
{
    class UDPPacketRouter {
      public:
        /**
         * @brief Constructs a UDPPacketRouter with the given SessionManager and IMessageSink.
         * @param sessions Shared pointer to the SessionManager for managing player sessions.
         * @param roomManager Shared pointer to the RoomManager for managing game rooms.
         */
        UDPPacketRouter(const std::shared_ptr<Server::ISessionManager> &sessions,
            const std::shared_ptr<Engine::RoomManager> &roomManager);

        /**
         * @brief Handles an incoming packet by routing it to the appropriate handler.
         * @param packet Shared pointer to the incoming IPacket to be processed.
         */
        void handlePacket(const std::shared_ptr<IPacket> &packet) const;

      private:
        /**
         * @brief Validates the header of an incoming packet.
         * @param pkt The incoming IPacket to validate.
         * @param header The HeaderData extracted from the incoming packet.
         * @return True if the header is valid, false otherwise.
         */
        [[nodiscard]] static bool validateHeader(const IPacket &pkt, const HeaderData &header);

        /**
         * @brief Handler for player connection packets.
         * @param sessionId The ID of the connected player.
         */
        void handleConnect(int sessionId) const;

        /**
         * @brief Handler for player input packets.
         * @param sessionId The ID of the player.
         * @param payload Pointer to the payload data of the input packet.
         * @param payloadSize Size of the payload data.
         */
        void handleInput(int sessionId, const std::uint8_t *payload, std::size_t payloadSize) const;

        /**
         * @brief Handler for player ping packets.
         * @param sessionId The ID of the player.
         */
        void handlePing(int sessionId) const;

        /**
         * @brief Handler for player disconnection packets.
         * @param sessionId The ID of the disconnected player.
         */
        void handleDisconnect(int sessionId) const;

        /**
         * @brief Validates the incoming packet.
         * @param packet Shared pointer to the incoming IPacket to validate.
         * @return True if the packet is valid, false otherwise.
         */
        static bool isPacketValid(const std::shared_ptr<IPacket> &packet) noexcept;

        /**
         * @brief Extracts the header from the incoming packet.
         * @param packet The incoming IPacket to extract the header from.
         * @param outHeader Reference to the HeaderData to populate with extracted data.
         * @return True if the header was successfully extracted and validated, false otherwise.
         */
        static bool extractHeader(const IPacket &packet, HeaderData &outHeader) noexcept;

        /**
         * @brief Resolves the session ID for the incoming packet.
         * @param packet The incoming IPacket to resolve the session for.
         * @return The session ID associated with the packet, or -1 if resolution fails.
         */
        [[nodiscard]] int resolveSession(const IPacket &packet) const;

        /**
         * @brief Dispatches the packet to the appropriate handler based on its type.
         * @param sessionId The ID of the player session.
         * @param header The HeaderData of the incoming packet.
         * @param payload Pointer to the payload data of the packet.
         * @param payloadSize Size of the payload data.
         */
        void dispatchPacket(
            int sessionId, const HeaderData &header, const uint8_t *payload, std::size_t payloadSize) const;

        std::shared_ptr<Server::ISessionManager>
            _sessions; ///> Pointer to the SessionManager for managing player sessions.
        std::shared_ptr<Engine::RoomManager> _roomManager; ///> Pointer to the RoomManager for managing game rooms.

        static constexpr std::uint8_t PROTOCOL_VERSION = 1; ///> Expected protocol version for incoming packets.
    };
} // namespace Net