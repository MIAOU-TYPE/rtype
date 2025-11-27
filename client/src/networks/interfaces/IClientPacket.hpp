/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** IClientPacket
*/

#pragma once

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif

#include <cstdint>
#include <exception>
#include <stddef.h>
#include <string>
#include <utility>
#include <vector>

/**
 * @namespace Network
 * @brief Namespace for network-related classes and functions.
 */
namespace Network
{
    class PacketError : public std::exception {
      public:
        /**
         * @brief Constructs an PacketError with a given message.
         * @param message The error message.
         */
        explicit PacketError(std::string message) : _msg(std::move(message))
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _msg.c_str();
        }

      private:
        std::string _msg;
    };

    /**
     * @class IClientPacket
     * @brief Interface for a client packet.
     * @details This interface defines the essential methods for accessing and modifying
     * the properties of a client packet, including the buffer, size, and source address.
     * @note This interface should be implemented by any client packet class.
     */
    class IClientPacket {
      public:
        /**
         * @brief Virtual destructor for IClientPacket interface.
         * Ensures proper cleanup of derived classes.
         * @note Always declare destructors in interfaces as virtual.
         */
        virtual ~IClientPacket() = default;

        /**
         * @brief Retrieves the packet buffer.
         * @return A pointer to the packet buffer.
         */
        virtual uint8_t *buffer() = 0;

        /**
         * @brief Retrieves the packet buffer (const version).
         * @return A const pointer to the packet buffer.
         */
        virtual const uint8_t *buffer() const = 0;

        /**
         * @brief Retrieves the size of the packet.
         * @return The size of the packet in bytes.
         */
        virtual size_t size() const = 0;

        /**
         * @brief Sets the size of the packet.
         * @param s The size to set for the packet.
         */
        virtual void setSize(size_t s) = 0;

        /**
         * @brief Retrieves the source address of the packet.
         * @return A pointer to the sockaddr_in structure representing the source address.
         */
        virtual sockaddr_in *address() = 0;

        /**
         * @brief Retrieves the source address of the packet (const version).
         * @return A const pointer to the sockaddr_in structure representing the source address.
         */
        virtual const sockaddr_in *address() const = 0;
    };
} // namespace Network
