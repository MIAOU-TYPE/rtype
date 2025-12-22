/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketRouter
*/

#pragma once

#include <cstring>
#include <iostream>
#include <memory>

#include "DefaultData.hpp"
#include "Endian.hpp"
#include "HeaderData.hpp"
#include "IClientMessageSink.hpp"
#include "IPacket.hpp"
#include "SnapEntityData.hpp"
#include "TypesData.hpp"

namespace Ecs
{
    class PacketRouterError : public std::exception {
      public:
        /**
         * @brief Constructor for PacketRouterError.
         * @param message The error message to be associated with the exception.
         */
        explicit PacketRouterError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief Routes incoming packets to appropriate handlers based on packet type.
     */
    class PacketRouter {
      public:
        /**
         * @brief Constructs a PacketRouter with the given IClientMessageSink.
         * @param sink Shared pointer to the IClientMessageSink for handling routed messages.
         */
        explicit PacketRouter(const std::shared_ptr<IClientMessageSink> &sink);

        /**
         * @brief Handles an incoming packet by routing it to the appropriate handler.
         * @param packet Shared pointer to the incoming IPacket to be processed.
         */
        void handlePacket(const std::shared_ptr<Net::IPacket> &packet) const;

      private:
        /**
         * @brief Validates the header of an incoming packet.
         * @param packet The incoming IPacket to validate.
         * @param header The HeaderData extracted from the incoming packet.
         * @return True if the header is valid, false otherwise.
         */
        [[nodiscard]] static bool isHeaderValid(const Net::IPacket &packet, const HeaderData &header);

        /**
         * @brief Validates the incoming packet.
         * @param packet Shared pointer to the incoming IPacket to validate.
         * @return True if the packet is valid, false otherwise.
         */
        [[nodiscard]] static bool isPacketValid(const std::shared_ptr<Net::IPacket> &packet) noexcept;

        /**
         * @brief Handler for ACCEPT packets.
         */
        void handleAccept() const;

        /**
         * @brief Handler for REJECT packets.
         */
        void handleReject() const;

        /**
         * @brief Handler for PONG packets.
         */
        void handlePong() const;

        /**
         * @brief Handler for GAME_OVER packets.
         */
        void handleGameOver() const;

        /**
         * @brief Handler for SNAP_ENTITY packets.
         */
        void handleSnapEntity(const uint8_t *payload, size_t size) const;

        /**
         * @brief Extracts the header from the incoming packet.
         * @param packet The incoming IPacket to extract the header from.
         * @param outHeader Reference to the HeaderData to populate with extracted data
         * @return True if the header was successfully extracted and validated, false otherwise.
         */
        [[nodiscard]] static bool extractHeader(const Net::IPacket &packet, HeaderData &outHeader) noexcept;

        /**
         * @brief Dispatches the packet to the appropriate handler based on its type.
         * @param header The HeaderData of the incoming packet.
         * @param payload Pointer to the payload data of the packet
         * @param payloadSize Size of the payload data.
         */
        void dispatchPacket(const HeaderData &header, const std::uint8_t *payload, std::size_t payloadSize) const;

        static constexpr std::uint8_t PROTOCOL_VERSION = 1; ///> Expected protocol version for incoming packets.

        std::shared_ptr<IClientMessageSink> _sink; ///> Pointer to the IClientMessageSink for handling routed messages.
    };
} // namespace Ecs