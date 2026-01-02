/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketFactory
*/

#include "TCPPacketFactory.hpp"

namespace Net::Factory
{
    TCPPacketFactory::TCPPacketFactory(const std::shared_ptr<IPacket> &proto) : _packet(proto)
    {
    }

    std::shared_ptr<IPacket> TCPPacketFactory::make(const sockaddr_in &addr, const std::vector<uint8_t> &payload) const
    {
        auto p = _packet->newPacket();
        p->setAddress(addr);
        std::memcpy(p->buffer(), payload.data(), payload.size());
        p->setSize(static_cast<uint32_t>(payload.size()));
        return p;
    }
} // namespace Net::Factory