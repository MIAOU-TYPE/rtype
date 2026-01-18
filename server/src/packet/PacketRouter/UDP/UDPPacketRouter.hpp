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

#include "ConnectData.hpp"
#include "GameEndData.hpp"
#include "IPacket.hpp"
#include "PongData.hpp"
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

        /**
         * @brief Handles a connection request packet.
         * @param header The HeaderData of the incoming packet.
         * @param raw Pointer to the raw packet data.
         * @param total Total size of the packet data.
         * @param from Pointer to the sockaddr_in structure representing the sender's address.
         * @return True if the packet was a connection request and was handled, false otherwise.
         */
        [[nodiscard]] bool handleConnect(
            const HeaderData &header, const uint8_t *raw, size_t total, const sockaddr_in *from) const;

      private:
        /**
         * @brief Validates the header of an incoming packet.
         * @param pkt The incoming IPacket to validate.
         * @param header The HeaderData extracted from the incoming packet.
         * @return True if the header is valid, false otherwise.
         */
        [[nodiscard]] static bool validateHeader(const IPacket &pkt, const HeaderData &header);

        /**
         * @brief Handler for player input packets.
         * @param sessionId The ID of the player.
         * @param payload Pointer to the payload data of the input packet.
         * @param payloadSize Size of the payload data.
         */
        void handleInput(int sessionId, const uint8_t *payload, size_t payloadSize) const;

        /**
         * @brief Handler for player ping packets.
         * @param sessionId The ID of the player.
         * @param payload Pointer to the payload data of the ping packet.
         * @param payloadSize Size of the payload data.
         */
        void handlePing(const int sessionId, const uint8_t *payload, const size_t payloadSize) const;

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
        [[nodiscard]] static bool isPacketValid(const std::shared_ptr<IPacket> &packet) noexcept;

        /**
         * @brief Extracts the header from the incoming packet.
         * @param packet The incoming IPacket to extract the header from.
         * @param outHeader Reference to the HeaderData to populate with extracted data.
         * @return True if the header was successfully extracted and validated, false otherwise.
         */
        [[nodiscard]] static bool extractHeader(const IPacket &packet, HeaderData &outHeader) noexcept;

        /**
         * @brief Dispatches the packet to the appropriate handler based on its type.
         * @param sessionId The ID of the player session.
         * @param header The HeaderData of the incoming packet.
         * @param payload Pointer to the payload data of the packet.
         * @param payloadSize Size of the payload data.
         */
        void dispatchPacket(int sessionId, const HeaderData &header, const uint8_t *payload, size_t payloadSize) const;

        std::shared_ptr<Server::ISessionManager>
            _sessions; ///> Pointer to the SessionManager for managing player sessions.
        std::shared_ptr<Engine::RoomManager> _roomManager; ///> Pointer to the RoomManager for managing game rooms.

        static constexpr uint8_t PROTOCOL_VERSION = 1; ///> Expected protocol version for incoming packets.
    };
} // namespace Net