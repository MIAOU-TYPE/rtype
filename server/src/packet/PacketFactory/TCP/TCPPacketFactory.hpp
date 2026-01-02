/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketFactory
*/

#pragma once

// TCPPacketFactory.hpp
#pragma once
#include <memory>
#include <vector>
#include "IPacket.hpp"
#include "payload/TCPPayload.hpp"

namespace Net::Factory
{
    class TCPPacketFactory {
      public:
        explicit TCPPacketFactory(const std::shared_ptr<IPacket> &proto) : _packet(proto)
        {
        }

        std::shared_ptr<IPacket> make(const sockaddr_in &addr, const std::vector<uint8_t> &payload) const
        {
            auto p = _packet->newPacket();
            p->setAddress(addr);
            std::memcpy(p->buffer(), payload.data(), payload.size());
            p->setSize(static_cast<uint32_t>(payload.size()));
            return p;
        }

      private:
        std::shared_ptr<IPacket> _packet;
    };
} // namespace Net::Factory
