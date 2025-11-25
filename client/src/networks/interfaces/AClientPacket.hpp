/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AClientPacket
*/

#pragma once
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <vector>
#include "IClientPacket.hpp"

/**
 * @namespace Network
 * @brief Namespace for network-related classes and functions.
 */
namespace Network
{
    /**
     * @class AClientPacket
     * @brief Abstract base class for client packets.
     * @details This class provides common functionality for packet management,
     * including buffer, size, and address handling. Derived classes can use
     * this implementation or override specific methods as needed.
     * @note This class inherits from IClientPacket and provides default implementations.
     */
    class AClientPacket : public IClientPacket {
      public:
        /**
         * @brief Virtual destructor for AClientPacket.
         */
        virtual ~AClientPacket() = default;

        /**
         * @brief Retrieves the packet buffer.
         * @return A pointer to the packet buffer.
         */
        uint8_t *buffer() override;

        /**
         * @brief Retrieves the packet buffer (const version).
         * @return A const pointer to the packet buffer.
         */
        const uint8_t *buffer() const override;

        /**
         * @brief Retrieves the source address of the packet.
         * @return A pointer to the sockaddr_in structure representing the source address.
         */
        sockaddr_in *address() override;

        /**
         * @brief Retrieves the source address of the packet (const version).
         * @return A const pointer to the sockaddr_in structure representing the source address.
         */
        const sockaddr_in *address() const override;

        /**
         * @brief Retrieves the size of the packet.
         * @return The size of the packet in bytes.
         */
        size_t size() const override;

        /**
         * @brief Sets the size of the packet.
         * @param s The size to set for the packet.
         */
        void setSize(size_t s) override;

        /**
         * @brief Converts the packet data to a vector.
         * @return A vector containing the packet data.
         */
        std::vector<uint8_t> toVector() const override;

        /**
         * @brief Maximum size of the packet buffer.
         * @note Derived classes can override this value if needed.
         */
        static constexpr size_t MAX_SIZE = 2048;

      protected:
        /**
         * @brief Constructor for AClientPacket.
         * @details Initializes the packet buffer and size to zero.
         */
        AClientPacket();

        uint8_t _buffer[MAX_SIZE] = {0}; ///< Buffer to store packet data
        size_t _size = 0;                ///< Size of the packet
        sockaddr_in _addr = {};          ///< Source address of the packet
    };
} // namespace Network
