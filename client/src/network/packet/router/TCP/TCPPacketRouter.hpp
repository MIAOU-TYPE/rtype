/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** TCPPacketRouter
*/

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <string_view>

#include "IMessageHandler.hpp"
#include "IPacket.hpp"
#include "RoomData.hpp"
#include "TCPPayload.hpp"
#include "TCPReader.hpp"
#include "TCPTypesData.hpp"

namespace Network
{
    /**
     * @class TCPPacketRouter
     * @brief Routes incoming TCP packets to the appropriate message handler methods.
     */
    class TCPPacketRouter {
      public:
        /**
         * @brief Constructor for TCPPacketRouter.
         * @param handler Shared pointer to the message handler that will process the routed messages.
         */
        explicit TCPPacketRouter(std::shared_ptr<IMessageHandler> handler);

        /**
         * @brief Handles an incoming TCP packet by routing it to the appropriate message handler method.
         * @param pkt Shared pointer to the incoming TCP packet.
         */
        void handle(const std::shared_ptr<Net::IPacket> &pkt) const;

      private:
        /**
         * @brief Reports a protocol error to the message handler.
         * @param req The request ID associated with the error.
         * @param msg The error message.
         */
        void protocolError(std::uint32_t req, std::string_view msg) const;

        /**
         * @brief Handler for Welcome messages.
         * @param req The request ID.
         * @param r Reader for parsing the message payload.
         */
        void onWelcome(std::uint32_t req, Net::TCP::Reader &r) const;

        /**
         * @brief Handler for Error messages.
         * @param req The request ID.
         * @param r Reader for parsing the message payload.
         */
        void onError(std::uint32_t req, Net::TCP::Reader &r) const;

        /**
         * @brief Handler for Rooms List messages.
         * @param req The request ID.
         * @param r Reader for parsing the message payload.
         */
        void onRoomsList(std::uint32_t req, Net::TCP::Reader &r) const;

        /**
         * @brief Handler for Room Created messages.
         * @param req The request ID.
         * @param r Reader for parsing the message payload.
         */
        void onRoomCreated(std::uint32_t req, Net::TCP::Reader &r) const;

        /**
         * @brief Handler for Room Joined messages.
         * @param req The request ID.
         * @param r Reader for parsing the message payload.
         */
        void onRoomJoined(std::uint32_t req, Net::TCP::Reader &r) const;

        /**
         * @brief Handler for Room Left messages.
         * @param req The request ID.
         * @param r Reader for parsing the message payload.
         */
        void onRoomLeft(std::uint32_t req, Net::TCP::Reader &r) const;

        /**
         * @brief Handler for Game Start messages.
         * @param req The request ID.
         * @param r Reader for parsing the message payload.
         */
        void onGameStart(std::uint32_t req, Net::TCP::Reader &r) const;

        std::shared_ptr<IMessageHandler> _sink;
    };
} // namespace Network
