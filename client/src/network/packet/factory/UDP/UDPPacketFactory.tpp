/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacketFactory
*/

#pragma once

namespace Network
{
    template <typename Type>
    std::shared_ptr<Net::IPacket> UDPPacketFactory::makePacket(const Type &packetData) const
    {
        if (!_packet)
            throw FactoryError("{UDPPacketFactory::makePacket}: Template packet is null");
        if (sizeof(Type) > _packet->capacity())
            throw FactoryError("{UDPPacketFactory::makePacket}: Packet type is too large to be serialized");
        auto buffer = _packet->newPacket();
        if (!buffer)
            throw FactoryError("{UDPPacketFactory::makePacket}: Failed to create new packet buffer");
        std::memcpy(buffer->buffer(), &packetData, sizeof(Type));
        buffer->setSize(sizeof(Type));
        return buffer;
    }

} // namespace Network
