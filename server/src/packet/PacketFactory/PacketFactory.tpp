/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#pragma once
namespace Net::Factory
{
    template <typename Type>
    std::shared_ptr<IPacket> PacketFactory::makePacket(
        const sockaddr_in &addr, const Type &packetData) const
    {
        auto buffer = _packet->newPacket();
        if (!buffer)
            throw FactoryError("{Net::Factory::PacketFactory::makePacket} Failed to create new packet buffer");

        if (sizeof(Type) > buffer->capacity())
            throw FactoryError("{Net::Factory::PacketFactory::makePacket} Packet type is too large to be serialized");

        buffer->setAddress(addr);
        std::memcpy(buffer->buffer(), &packetData, sizeof(Type));
        buffer->setSize(sizeof(Type));
        return buffer;
    }
}