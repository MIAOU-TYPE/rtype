/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacket
*/

#pragma once
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include "IServerPacket.hpp"

/**
 * @namespace Net
 * @brief Namespace for network-related classes and functions.
 */
namespace Net
{
    /**
     * @class UDPPacket
     * @brief Represents a received network packet.
     * @details This class encapsulates the data and metadata of a received packet,
     * including the buffer, size, and source address.
     */
    class UDPPacket : public IServerPacket {
      public:
        /**
         * @brief Maximum size of the packet buffer.
         * @note This constant defines the maximum number of bytes that can be stored in the packet buffer.
         */
        static constexpr size_t MAX_SIZE = 2048;

        /**
         * @brief Constructs a new UDPPacket object.
         * @details This constructor initializes the packet buffer and size.
         */
        UDPPacket();

        /**
         * @brief Destroys the UDPPacket object.
         * @details This destructor cleans up any resources associated with the packet.
         */
        ~UDPPacket() override = default;

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
         * @brief Retrieves the source address of the packet (const version).
         * @return A const pointer to the sockaddr_in structure representing the source address.
         */
        const sockaddr_in *address() const override;

        /**
         * @brief Sets the source address of the packet.
         * @param addr The sockaddr_in structure representing the source address to set.
         */
        void setAddress(const sockaddr_in &addr) override;

        /**
         * @brief Creates a clone of the current packet.
         * @return A shared pointer to the cloned IServerPacket.
         */
        std::shared_ptr<IServerPacket> clone() const override;

      private:
        uint8_t _buffer[MAX_SIZE] = {0}; //> Buffer to store packet data
        size_t _size = 0;                //> Size of the packet
        sockaddr_in _addr = {};          //> Source address of the packet
    };
} // namespace Net

/**
 * @brief Overloads the output stream operator for UDPPacket.
 * @param os The output stream.
 * @param pkt The UDPPacket to be printed.
 * @return The output stream.
 */
inline std::ostream &operator<<(std::ostream &os, const Net::IServerPacket &pkt)
{
    std::ostream &out = os;

    for (size_t i = 0; i < pkt.size(); ++i) {
        uint8_t c = pkt.buffer()[i];
        if (std::isprint(c))
            os << static_cast<char>(c);
        else
            os << ".";
    }

    os << std::endl;
    return out;
}

/**
 * @brief Overloads the output stream operator for shared pointers to IServerPacket.
 * @param os The output stream.
 * @param pkt The shared pointer to the IServerPacket to be printed.
 * @return The output stream.
 */
inline std::ostream &operator<<(std::ostream &os, std::shared_ptr<Net::IServerPacket> pkt)
{
    return os << *pkt;
}
