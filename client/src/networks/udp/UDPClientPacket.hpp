/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPClientPacket
*/

#pragma once
#include <cstring>
#include <memory>
#include <vector>
#include "IClientPacket.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
#endif

/**
 * @namespace Network
 * @brief Namespace for network-related classes and functions.
 */
namespace Network
{
    /**
     * @class UDPClientPacket
     * @brief Concrete base class for client packets.
     * @details This class provides common functionality for packet management,
     * including buffer, size, and address handling. Derived classes can use
     * this implementation directly or override specific methods as needed.
     * @note This class inherits from IClientPacket and provides default implementations.
     */
    class UDPClientPacket : public IClientPacket {
      public:
        /**
         * @brief Constructor for UDPClientPacket.
         * @details Initializes the packet buffer and size to zero.
         */
        UDPClientPacket() = default;

        /**
         * @brief Virtual destructor for UDPClientPacket.
         */
        ~UDPClientPacket() = default;

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
         * @brief Retrieves the capacity of the packet buffer.
         * @return The maximum capacity of the packet buffer in bytes.
         */
        size_t capacity() const override;

        /**
         * @brief Creates a clone of the packet.
         * @return A shared pointer to a new UDPClientPacket instance with the same buffer capacity.
         */
        std::shared_ptr<IClientPacket> clone() const override;

        /**
         * @brief Maximum size of the packet buffer.
         * @note Derived classes can override this value if needed.
         */
        static constexpr size_t MAX_SIZE = 2048;

      private:
        uint8_t _buffer[MAX_SIZE] = {0}; ///> Buffer to store packet data
        size_t _size = 0;                ///> Size of the packet
        sockaddr_in _addr = {};          ///> Source address of the packet
    };
} // namespace Network
