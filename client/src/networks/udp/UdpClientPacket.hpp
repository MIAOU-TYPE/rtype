/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UdpClientPacket
*/

#pragma once
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>
#include "AClientPacket.hpp"

/**
 * @namespace Network
 * @brief Namespace for network-related classes and functions.
 */
namespace Network
{
    /**
     * @class UdpClientPacket
     * @brief Represents a received UDP network packet.
     * @details This class encapsulates the data and metadata of a received packet,
     * including the buffer, size, and source address. It inherits from AClientPacket
     * which provides the default implementation for common packet operations.
     */
    class UdpClientPacket : public AClientPacket {
      public:
        /**
         * @brief Constructs a new UdpClientPacket object.
         * @details This constructor initializes the packet buffer and size through AClientPacket.
         */
        UdpClientPacket();

        /**
         * @brief Destroys the UdpClientPacket object.
         * @details This destructor cleans up any resources associated with the packet.
         */
        ~UdpClientPacket() = default;

        // All methods are inherited from AClientPacket
        // No need to redeclare them unless we want to override specific behavior
    };
} // namespace Network

/**
 * @brief Overloads the output stream operator for UdpClientPacket.
 * @param os The output stream.
 * @param pkt The UdpClientPacket to be printed.
 * @return The output stream.
 */
inline std::ostream &operator<<(std::ostream &os, const Network::IClientPacket &pkt)
{
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(pkt.address()->sin_addr), ip, INET_ADDRSTRLEN);
    
    os << "[Packet from " << ip << ":" << ntohs(pkt.address()->sin_port) 
       << " | Size: " << pkt.size() << " bytes] ";
    
    for (size_t i = 0; i < pkt.size(); ++i) {
        uint8_t c = pkt.buffer()[i];
        if (std::isprint(c))
            os << static_cast<char>(c);
        else
            os << ".";
    }
    
    return os;
}

/**
 * @brief Overloads the output stream operator for shared pointers to IClientPacket.
 * @param os The output stream.
 * @param pkt The shared pointer to the IClientPacket to be printed.
 * @return The output stream.
 */
inline std::ostream &operator<<(std::ostream &os, std::shared_ptr<Network::IClientPacket> pkt)
{
    return os << *pkt;
}
