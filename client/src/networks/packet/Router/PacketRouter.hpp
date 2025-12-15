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
#include "EntityCreateData.hpp"
#include "EntityDestroyData.hpp"
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

    class PacketRouter {
      public:
        PacketRouter(const std::shared_ptr<IClientMessageSink> &sink);

        void handlePacket(const std::shared_ptr<Net::IPacket> &packet) const;

      private:
        static bool isHeaderValid(const Net::IPacket &packet, const HeaderData &header);

        static bool isPacketValid(const std::shared_ptr<Net::IPacket> &packet) noexcept;

        void handleAccept() const;

        void handleReject() const;

        void handlePong() const;

        void handleGameOver() const;

        void handleEntityCreate() const;

        void handleEntityDestroy() const;

        void handleSnapEntity() const;

        static bool extractHeader(const Net::IPacket &packet, HeaderData &outHeader) noexcept;

        void dispatchPacket(const HeaderData &header, const std::uint8_t *payload, std::size_t payloadSize) const;

        static constexpr std::uint8_t PROTOCOL_VERSION = 1; ///> Expected protocol version for incoming packets.

        std::shared_ptr<IClientMessageSink> _sink; ///> Pointer to the IClientMessageSink for handling routed messages.
    };
} // namespace Ecs