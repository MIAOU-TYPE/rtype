/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#pragma once
#include <cstring>
#include <map>
#include <memory>
#include "DefaultPacket.hpp"
#include "HeaderPacket.hpp"
#include "IServerPacket.hpp"

#define ACCEPT         0x10
#define REJECT         0x11
#define SNAPSHOT       0x12
#define ENTITY_CREATE  0x13
#define ENTITY_DESTROY 0x14
#define PONG           0x15
#define DAMAGE_EVENT   0x16
#define GAME_OVER      0x17

/**
 * @namespace Net
 * @brief Namespace for network-related classes and functions.
 */
namespace Net
{
    /**
     * @namespace Factory
     * @brief Namespace for factory classes related to packet creation.
     */
    namespace Factory
    {
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
             * @param packet A shared pointer to an IServerPacket used as a template for creating packets.
             */
            PacketFactory(std::shared_ptr<Net::IServerPacket> packet);

            /**
             * @brief Destructor for PacketFactory.
             */
            ~PacketFactory() = default;

            /**
             * @brief Creates a default packet with the specified flag.
             * @param flag The flag to set in the default packet.
             * @return A reference to the created IServerPacket.
             */
            std::shared_ptr<IServerPacket> makeDefault(const sockaddr_in &addr, uint8_t flag) noexcept;

            std::shared_ptr<IServerPacket> makeEntityCreate(
                const sockaddr_in &addr, size_t id, float x, float y, uint16_t sprite) noexcept;
            std::shared_ptr<IServerPacket> makeEntityDestroy(const sockaddr_in &addr, size_t id) noexcept;
            std::shared_ptr<IServerPacket> makeDamage(const sockaddr_in &addr, size_t id, uint16_t amount) noexcept;

          private:
            /**
             * @brief Creates a HeaderPacket with the specified parameters.
             * @param type The type of the packet.
             * @param version The version of the packet.
             * @param size The size of the packet.
             * @return The constructed HeaderPacket.
             */
            HeaderPacket makeHeader(uint8_t type, uint8_t version, uint16_t size) noexcept;

            std::shared_ptr<IServerPacket> _packet =
                nullptr; //> Pointer to the template IServerPacket used for creating packets.
        };

    } // namespace Factory
} // namespace Net
