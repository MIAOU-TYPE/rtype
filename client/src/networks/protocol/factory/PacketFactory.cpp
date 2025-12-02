/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"
#include <iostream>

namespace Network
{
    PacketFactory::PacketFactory(const std::shared_ptr<IClientPacket> &packet)
    {
        if (!packet)
            throw FactoryError("Invalid IClientPacket pointer");
        _packet = packet;
    }

    PacketHeader PacketFactory::makeHeader(uint8_t type, uint16_t size) noexcept
    {
        PacketHeader header;
        header.type = type;
        header.version = VERSION;
        header.size = htons(size);
        return header;
    }

    std::shared_ptr<IClientPacket> PacketFactory::makeConnect(uint32_t clientId) const noexcept
    {
        PacketConnect connectPacket;
        connectPacket.header = makeHeader(CONNECT, sizeof(PacketConnect));
        connectPacket.clientId = htonl(clientId);

        try {
            return makePacket<PacketConnect>(connectPacket);
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makeConnect} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IClientPacket> PacketFactory::makeDisconnect() const noexcept
    {
        PacketHeader disconnectPacket = makeHeader(DISCONNECT, sizeof(PacketHeader));

        try {
            return makePacket<PacketHeader>(disconnectPacket);
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makeDisconnect} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IClientPacket> PacketFactory::makeInput(
        uint32_t entity, float dx, float dy, uint8_t shooting) const noexcept
    {
        PacketInput inputPacket;
        inputPacket.header = makeHeader(INPUT, sizeof(PacketInput));
        inputPacket.entity = htonl(entity);
        inputPacket.dx = htonf(dx);
        inputPacket.dy = htonf(dy);
        inputPacket.shooting = shooting;

        try {
            return makePacket<PacketInput>(inputPacket);
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makeInput} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IClientPacket> PacketFactory::makePing() const noexcept
    {
        PacketHeader pingPacket = makeHeader(PING, sizeof(PacketHeader));

        try {
            return makePacket<PacketHeader>(pingPacket);
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makePing} " << e.what() << std::endl;
            return nullptr;
        }
    }

} // namespace Network
