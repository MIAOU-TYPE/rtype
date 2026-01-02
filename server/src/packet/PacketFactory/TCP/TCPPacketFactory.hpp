/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketFactory
*/

#pragma once

#include <memory>
#include <cstring>
#include <vector>
#include "IPacket.hpp"
#include "payload/TCPPayload.hpp"

namespace Net::Factory
{
    /**
     * @class TCPPacketFactory
     * @brief Factory for creating TCP packets
     */
    class TCPPacketFactory {
      public:
        /**
         * @brief Constructor for TCPPacketFactory
         * @param proto The prototype packet to use for creating new packets
         */
        explicit TCPPacketFactory(const std::shared_ptr<IPacket> &proto);

        /**
         * @brief Creates a new TCP packet
         * @param addr The address of the packet
         * @param payload The payload of the packet
         * @return A shared pointer to the newly created packet
         */
        std::shared_ptr<IPacket> make(const sockaddr_in &addr, const std::vector<uint8_t> &payload) const;

      private:
        std::shared_ptr<IPacket> _packet; ///> Prototype packet for creating new packets
    };
} // namespace Net::Factory
