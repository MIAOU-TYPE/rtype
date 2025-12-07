/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#pragma once

#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include "DefaultPacket.hpp"
#include "Endian.hpp"
#include "HeaderPacket.hpp"
#include "IPacket.hpp"
#include "InputPacket.hpp"
#include "TypesPacket.hpp"

namespace Network
{
    /**
     * @class FactoryError
     * @brief Exception class for packet factory errors.
     */
    class FactoryError : public std::exception {
      public:
        /**
         * @brief Constructor with error message.
         * @param message The error message.
         */
        explicit FactoryError(const std::string &message) : _msg(message)
        {
        }

        /**
         * @brief Get the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _msg.c_str();
        }

      private:
        std::string _msg; ///> The error message
    };

    /**
     * @class PacketFactory
     * @brief Factory class for creating and serializing client packets.
     * @details This class provides methods to create different types of packets
     * (connect, disconnect, input, ping) with proper endianness conversion and serialization.
     */
    class PacketFactory {
      public:
        /**
         * @brief Construct a new PacketFactory object
         * @param packet A shared pointer to an IPacket used as a template for creating packets.
         */
        explicit PacketFactory(const std::shared_ptr<Net::IPacket> &packet);

        /**
         * @brief Destroy the PacketFactory object
         */
        ~PacketFactory() = default;

        /**
         * @brief Creates a base packet
         * @param flag The flag indicating the type of base packet
         * @return A shared pointer to the created packet
         */
        std::shared_ptr<Net::IPacket> makeBase(uint8_t flag) const noexcept;

        /**
         * @brief Creates an input packet
         * @param entity The entity ID to control
         * @param dx Horizontal movement axis (-1.0 to 1.0)
         * @param dy Vertical movement axis (-1.0 to 1.0)
         * @param shooting Shooting action flag
         * @return A shared pointer to the created packet
         */
        std::shared_ptr<Net::IPacket> makeInput(uint32_t entity, float dx, float dy, uint8_t shooting) const noexcept;

      private:
        /**
         * @brief Creates a packet header with the specified parameters
         * @param type The type of the packet
         * @param size The size of the packet
         * @return The constructed PacketHeader
         */
        static HeaderPacket makeHeader(uint8_t type, uint16_t size) noexcept;

        /**
         * @brief Creates a packet of the specified type with the given packet data
         * @tparam Type The type of the packet data (struct representing the packet)
         * @param packetData The data to be included in the packet
         * @return A shared pointer to the created IPacket
         */
        template <typename Type>
        std::shared_ptr<Net::IPacket> makePacket(const Type &packetData) const;

        std::shared_ptr<Net::IPacket> _packet = nullptr; ///> Template packet for cloning
        static constexpr uint8_t VERSION = 1;            ///> Protocol version
    };

} // namespace Network

#include "PacketFactory.tpp"
