/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketFactory
*/

#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <string_view>

#include "IPacket.hpp"
#include "TCPPayload.hpp"
#include "TCPTypesData.hpp"
#include "TCPWriter.hpp"

namespace Network
{
    /**
     * @class TCPPacketFactory
     * @brief Factory class for creating TCP packets.
     * @details This class provides methods to create various types of TCP packets
     * such as Hello, ListRooms, CreateRoom, JoinRoom, LeaveRoom, and StartGame.
     */
    class TCPPacketFactory {
      public:
        /**
         * @brief Constructor for TCPPacketFactory.
         * @param proto A shared pointer to a prototype IPacket used for creating new packets.
         */
        explicit TCPPacketFactory(std::shared_ptr<Net::IPacket> proto);

        /**
         * @brief Create a TCP packet with the given payload.
         * @param payload The payload data for the packet.
         * @return A shared pointer to the created TCP packet.
         */
        [[nodiscard]] std::shared_ptr<Net::IPacket> make(const std::vector<uint8_t> &payload) const;

        /**
         * @brief Create a Hello packet.
         * @param req The request ID for the Hello packet.
         * @param ver The version number for the Hello packet.
         * @return A shared pointer to the created Hello packet.
         */
        [[nodiscard]] std::shared_ptr<Net::IPacket> makeHello(uint32_t req, uint16_t ver) const;

        /**
         * @brief Create a ListRooms packet.
         * @param req The request ID for the ListRooms packet.
         * @return A shared pointer to the created ListRooms packet.
         */
        [[nodiscard]] std::shared_ptr<Net::IPacket> makeListRooms(uint32_t req) const;

        /**
         * @brief Create a CreateRoom packet.
         * @param req The request ID for the CreateRoom packet.
         * @param roomName The name of the room to be created.
         * @param maxPlayers The maximum number of players allowed in the room.
         * @return A shared pointer to the created CreateRoom packet.
         */
        [[nodiscard]] std::shared_ptr<Net::IPacket> makeCreateRoom(
            uint32_t req, std::string_view roomName, uint8_t maxPlayers) const;

        /**
         * @brief Create a JoinRoom packet.
         * @param req The request ID for the JoinRoom packet.
         * @param roomId The ID of the room to join.
         * @return A shared pointer to the created JoinRoom packet.
         */
        [[nodiscard]] std::shared_ptr<Net::IPacket> makeJoinRoom(uint32_t req, uint32_t roomId) const;

        /**
         * @brief Create a LeaveRoom packet.
         * @param req The request ID for the LeaveRoom packet.
         * @return A shared pointer to the created LeaveRoom packet.
         */
        [[nodiscard]] std::shared_ptr<Net::IPacket> makeLeaveRoom(uint32_t req) const;

        /**
         * @brief Create a StartGame packet.
         * @param req The request ID for the StartGame packet.
         * @return A shared pointer to the created StartGame packet.
         */
        [[nodiscard]] std::shared_ptr<Net::IPacket> makeStartGame(uint32_t req) const;

      private:
        std::shared_ptr<Net::IPacket> _packet = nullptr; ///> Prototype packet for creating new packets
    };
} // namespace Network
