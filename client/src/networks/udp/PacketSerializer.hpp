/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketSerializer
*/

#pragma once

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
#endif
#include <cstdint>
#include <cstring>
#include <vector>
#include "NetworkException.hpp"
#include "PacketTypes.hpp"
#include "ProtocolDefinitions.hpp"

namespace Network
{
    /**
     * @brief Serializes network packets for client-to-server communication
     *
     * This class handles the serialization of various packet types into
     * byte buffers ready for network transmission. It ensures proper byte
     * ordering (network byte order) for multi-byte fields.
     */
    class PacketSerializer {
      public:
        /**
         * @brief Construct a new PacketSerializer object
         */
        PacketSerializer();

        /**
         * @brief Destroy the PacketSerializer object
         */
        ~PacketSerializer();

        /**
         * @brief Serialize a connection request packet
         *
         * @param clientId Unique identifier for the client
         * @return std::vector<uint8_t> Serialized packet data
         */
        std::vector<uint8_t> serializeConnectPacket(uint32_t clientId);

        /**
         * @brief Serialize a player input packet
         *
         * @param entity Entity ID to control
         * @param dx Horizontal movement axis (-1.0 to 1.0)
         * @param dy Vertical movement axis (-1.0 to 1.0)
         * @param shooting Shooting action flag (0 or 1)
         * @return std::vector<uint8_t> Serialized packet data
         */
        std::vector<uint8_t> serializeInputPacket(uint32_t entity, float dx, float dy, uint8_t shooting);

        /**
         * @brief Serialize a ping packet
         *
         * @return std::vector<uint8_t> Serialized packet data
         */
        std::vector<uint8_t> serializePingPacket();

        /**
         * @brief Serialize a disconnection packet
         *
         * @return std::vector<uint8_t> Serialized packet data
         */
        std::vector<uint8_t> serializeDisconnectPacket();
    };

} // namespace Network