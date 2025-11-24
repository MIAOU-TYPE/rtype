/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketSerializer
*/

#include "PacketSerializer.hpp"

namespace Network
{

    PacketSerializer::PacketSerializer()
    {
    }

    PacketSerializer::~PacketSerializer()
    {
    }

    std::vector<uint8_t> PacketSerializer::serializeConnectPacket(uint32_t clientId)
    {
        PacketConnect packet{};
        packet.header.type = static_cast<uint8_t>(PacketType::CONNECT);
        packet.header.version = 1;
        packet.header.size = htons(sizeof(PacketConnect));
        packet.clientId = htonl(clientId);

        std::vector<uint8_t> buffer(sizeof(PacketConnect));

        if (buffer.size() < sizeof(PacketConnect)) {
            throw Client::Exception::BufferException("Buffer size is insufficient for PacketConnect serialization");
        }

        std::memcpy(buffer.data(), &packet, sizeof(PacketConnect));
        return buffer;
    }

    std::vector<uint8_t> PacketSerializer::serializeInputPacket(uint32_t entity, float dx, float dy, uint8_t shooting)
    {
        if (entity == 0) {
            throw Client::Exception::InvalidPacketException("Entity ID cannot be zero for PacketInput serialization");
        }

        PacketInput packet{};
        packet.header.type = static_cast<uint8_t>(PacketType::INPUT);
        packet.header.version = 1;
        packet.header.size = htons(sizeof(PacketInput));
        packet.entity = htonl(entity);
        packet.dx = dx;
        packet.dy = dy;
        packet.shooting = shooting;

        std::vector<uint8_t> buffer(sizeof(PacketInput));

        if (buffer.size() < sizeof(PacketInput)) {
            throw Client::Exception::BufferException("Buffer size is insufficient for PacketInput serialization");
        }

        std::memcpy(buffer.data(), &packet, sizeof(PacketInput));
        return buffer;
    }

    std::vector<uint8_t> PacketSerializer::serializePingPacket()
    {
        PacketPing packet{};
        packet.header.type = static_cast<uint8_t>(PacketType::PING);
        packet.header.version = 1;
        packet.header.size = htons(sizeof(PacketPing));

        std::vector<uint8_t> buffer(sizeof(PacketPing));

        if (buffer.size() < sizeof(PacketPing)) {
            throw Client::Exception::BufferException("Buffer size is insufficient for PacketPing serialization");
        }

        std::memcpy(buffer.data(), &packet, sizeof(PacketPing));
        return buffer;
    }

    std::vector<uint8_t> PacketSerializer::serializeDisconnectPacket()
    {
        PacketDisconnect packet{};
        packet.header.type = static_cast<uint8_t>(PacketType::DISCONNECT);
        packet.header.version = 1;
        packet.header.size = htons(sizeof(PacketDisconnect));

        std::vector<uint8_t> buffer(sizeof(PacketDisconnect));

        if (buffer.size() < sizeof(PacketDisconnect)) {
            throw Client::Exception::BufferException("Buffer size is insufficient for PacketDisconnect serialization");
        }
        
        std::memcpy(buffer.data(), &packet, sizeof(PacketDisconnect));
        return buffer;
    }

} // namespace Network