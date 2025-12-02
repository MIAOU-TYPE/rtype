/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"

namespace Network
{
    template <typename Type>
    std::shared_ptr<IClientPacket> PacketFactory::makePacket(const Type &packetData) const
    {
        if (!_packet)
            throw FactoryError("Template packet is null");
        if (sizeof(Type) > _packet->capacity())
            throw FactoryError("Packet type is too large to be serialized");
        auto buffer = _packet->clone();
        if (!buffer)
            throw FactoryError("Failed to clone packet");
        std::memcpy(buffer->buffer(), &packetData, sizeof(Type));
        buffer->setSize(sizeof(Type));
        return buffer;
    }

} // namespace Network
