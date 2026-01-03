/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPClientPacketFactory
*/

#pragma once

namespace Network
{
    template <typename Type>
    std::shared_ptr<Net::IPacket> UDPClientPacketFactory::makePacket(const Type &packetData) const
    {
        if (!_packet)
            throw FactoryError("{UDPClientPacketFactory::makePacket}: Template packet is null");
        if (sizeof(Type) > _packet->capacity())
            throw FactoryError("{UDPClientPacketFactory::makePacket}: Packet type is too large to be serialized");
        auto buffer = _packet->newPacket();
        if (!buffer)
            throw FactoryError("{UDPClientPacketFactory::makePacket}: Failed to create new packet buffer");
        std::memcpy(buffer->buffer(), &packetData, sizeof(Type));
        buffer->setSize(sizeof(Type));
        return buffer;
    }

} // namespace Network
