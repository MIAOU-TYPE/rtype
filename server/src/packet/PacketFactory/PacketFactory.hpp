/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#pragma once
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include "DamagePacket.hpp"
#include "DefaultPacket.hpp"
#include "Endian.hpp"
#include "EntityCreatePacket.hpp"
#include "EntityDestroyPacket.hpp"
#include "IPacket.hpp"
#include "InputPacket.hpp"
#include "TypesPacket.hpp"

/**
 * @namespace Net
 * @brief Namespace for networking-related classes and functions.
 */
/**
 * @namespace Factory
 * @brief Namespace for factory classes related to networking.
 */
namespace Net::Factory
{
    class FactoryError : public std::exception {
      public:
        /**
         * @brief Constructs a new FactoryError object with the specified message.
         * @param message The error message.
         */
        explicit FactoryError(std::string message) : _message(std::move(message))
        {
        }

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message = ""; ///< The error message.
    };

    /**
     * @class PacketFactory
     * @brief Factory class for creating various types of network packets.
     * @details This class provides methods to create different types of packets
     * such as connect/disconnect, input, default, entity creation/destruction, and damage events.
     */
    class PacketFactory {
      public:
        /**
         * @brief Constructs a new PacketFactory object.
         * @param packet A shared pointer to an IPacket used as a template for creating packets.
         */
        explicit PacketFactory(const std::shared_ptr<Net::IPacket> &packet);

        /**
         * @brief Destructor for PacketFactory.
         */
        ~PacketFactory() = default;

        /**
         * @brief Creates a default packet with the specified flag.
         * @param flag The flag to set in the default packet.
         * @param addr The address to which the packet will be sent.
         * @return A shared pointer to the created IPacket.
         */
        std::shared_ptr<IPacket> makeDefault(const sockaddr_in &addr, uint8_t flag) const noexcept;

        /**
         * @brief Creates an EntityCreatePacket with the specified parameters.
         * @param addr The address to which the packet will be sent.
         * @param id The ID of the entity to create.
         * @param x The x-coordinate of the entity.
         * @param y The y-coordinate of the entity.
         * @param sprite The sprite ID of the entity.
         * @return A shared pointer to the created IPacket.
         */
        std::shared_ptr<IPacket> makeEntityCreate(
            const sockaddr_in &addr, size_t id, float x, float y, uint16_t sprite) const noexcept;

        /**
         * @brief Creates an EntityDestroyPacket for the specified entity ID.
         * @param addr The address to which the packet will be sent.
         * @param id The ID of the entity to destroy.
         * @return A shared pointer to the created IPacket.
         */
        std::shared_ptr<IPacket> makeEntityDestroy(const sockaddr_in &addr, size_t id) const noexcept;

        /**
         * @brief Creates a DamagePacket for the specified entity ID and damage amount.
         * @param addr The address to which the packet will be sent.
         * @param id The ID of the entity receiving damage.
         * @param amount The amount of damage to apply.
         * @return A shared pointer to the created IPacket.
         */
        std::shared_ptr<IPacket> makeDamage(const sockaddr_in &addr, uint32_t id, uint16_t amount) const noexcept;

      private:
        /**
         * @brief Creates a HeaderPacket with the specified parameters.
         * @param type The type of the packet.
         * @param version The version of the packet.
         * @param size The size of the packet.
         * @return The constructed HeaderPacket.
         */
        static HeaderPacket makeHeader(uint8_t type, uint8_t version, uint16_t size) noexcept;

        /**
         * @brief Creates a packet of the specified type with the given address and packet data.
         * @tparam Type The type of the packet data. (struct representing the packet)
         * @param addr The address to which the packet will be sent.
         * @param packetData The data to be included in the packet.
         * @return A shared pointer to the created IPacket.
         */
        template <typename Type>
        std::shared_ptr<IPacket> makePacket(const sockaddr_in &addr, const Type &packetData) const;

        std::shared_ptr<IPacket> _packet = nullptr; ///> Pointer to the template IPacket used for creating packets.

        static constexpr uint8_t VERSION = 1; ///> The version of the packet factory.
    };
} // namespace Net::Factory

#include "PacketFactory.tpp"