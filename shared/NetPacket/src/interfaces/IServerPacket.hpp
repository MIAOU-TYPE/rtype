/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** IPacket
*/

#pragma once
#include <cstdint>
#include <memory>
#include <stddef.h>
#ifndef _WIN32
    #include <netinet/in.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

/**
 * @namespace Net
 * @brief Namespace for network-related classes and functions.
 */
namespace Net
{
    /**
     * @class IServerPacket
     * @brief Interface for a server packet.
     * @details This interface defines the essential methods for accessing and modifying
     * the properties of a server packet, including the buffer, size, and source address.
     * @note This interface should be implemented by any server packet class.
     */
    class IServerPacket {
      public:
        /**
         * @brief Virtual destructor for IServerPacket interface.
         * Ensures proper cleanup of derived classes.
         * @note Always declare destructors in interfaces as virtual.
         */
        virtual ~IServerPacket() = default;

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
         * @brief Retrieves the source address of the packet.
         * @return A pointer to the sockaddr_in structure representing the source address.
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
        virtual const sockaddr_in *address() const = 0;

        /**
         * @brief Sets the source address of the packet.
         * @param addr The sockaddr_in structure representing the source address to set.
         */
        virtual void setAddress(const sockaddr_in &addr) = 0;

        /**
         * @brief Creates a clone of the current packet.
         * @return A shared pointer to the cloned IServerPacket.
         */
        virtual std::shared_ptr<IServerPacket> clone() const = 0;

        /**
         * @brief Retrieves the capacity of the packet buffer.
         * @return The capacity of the packet buffer.
         */
        virtual size_t capacity() const noexcept = 0;
    };
} // namespace Net
