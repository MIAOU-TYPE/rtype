/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntitiesFactory
*/

#pragma once

#include <cstring>
#include <iostream>
#include <memory>

#include "DefaultData.hpp"
#include "EntityCreateData.hpp"
#include "EntityDestroyData.hpp"
#include "HeaderData.hpp"
#include "IPacket.hpp"
#include "SnapEntityData.hpp"
#include "TypesData.hpp"

namespace Ecs
{
    class EntitiesFactoryError : public std::exception {
      public:
        /**
         * @brief Constructor for EntitiesFactoryError.
         * @param message The error message to be associated with the exception.
         */
        explicit EntitiesFactoryError(const std::string &message) : _message("\n\t" + message)
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
     * @class EntitiesFactory
     * @brief Concrete implementation of IEntitiesFactory for creating and managing game entities.
     */
    class EntitiesFactory {
      public:
        /**
         * @brief Default constructor for EntitiesFactory.
         */
        EntitiesFactory() = default;

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
        static bool isHeaderValid(const Net::IPacket &packet, const HeaderData &header);

        /**
         * @brief Validates the incoming packet.
         * @param packet Shared pointer to the incoming IPacket to validate.
         * @return True if the packet is valid, false otherwise.
         */
        bool isPacketValid(const std::shared_ptr<Net::IPacket> &packet) const noexcept;

        /**
         * @brief Handler for default packets (ACCEPT, REJECT).
         * @param packet The incoming IPacket to process.
         */
        void handleAccept() const;

        /**
         * @brief Handler for default packets (ACCEPT, REJECT).
         * @param packet The incoming IPacket to process.
         */
        void handleReject() const;

        /**
         * @brief Handler for default packets (ACCEPT, REJECT).
         * @param packet The incoming IPacket to process.
         */
        void handlePong() const;

        /**
         * @brief Handler for GAME_OVER packets.
         * @param packet The incoming IPacket to process.
         */
        void handleGameOver() const;

        /**
         * @brief Handler for ENTITY_CREATE packets.
         * @param packet The incoming IPacket to process.
         */
        void handleEntityCreate(const std::uint8_t *payload, const std::size_t payloadSize) const;

        /**
         * @brief Handler for ENTITY_DESTROY packets.
         * @param packet The incoming IPacket to process.
         */
        void handleEntityDestroy(const std::uint8_t *payload, const std::size_t payloadSize) const;

        /**
         * @brief Handler for SNAPSHOT packets.
         * @param packet The incoming IPacket to process.
         */
        void handleSnapEntity(const std::uint8_t *payload, const std::size_t payloadSize) const;

        /**
         * @brief Extracts the header from an incoming packet.
         * @param packet The incoming IPacket to extract the header from.
         * @param outHeader Reference to a HeaderData structure where the extracted header will be stored.
         * @return True if the header was successfully extracted and is valid, false otherwise.
         */
        bool extractHeader(const Net::IPacket &packet, HeaderData &outHeader) const noexcept;

        /**
         * @brief Dispatches the packet to the appropriate handler based on its type.
         * @param header The HeaderData of the incoming packet.
         * @param payload Pointer to the payload data of the packet.
         * @param payloadSize Size of the payload data.
         */
        void dispatchPacket(const HeaderData &header, const std::uint8_t *payload, std::size_t payloadSize) const;

        static constexpr std::uint8_t PROTOCOL_VERSION = 1; ///> Expected protocol version for incoming packets.
    };
} // namespace Ecs