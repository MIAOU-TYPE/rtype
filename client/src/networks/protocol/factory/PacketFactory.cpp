/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"

namespace Network
{
    PacketFactory::PacketFactory(const std::shared_ptr<Net::IPacket> &packet)
    {
        if (!packet)
            throw FactoryError("Invalid IPacket pointer");
        _packet = packet;
    }

    HeaderData PacketFactory::makeHeader(uint8_t type, uint16_t size) noexcept
    {
        HeaderData header;
        header.type = type;
        header.version = VERSION;
        header.size = htons(size);
        return header;
    }

    std::shared_ptr<Net::IPacket> PacketFactory::makeBase(uint8_t flag) const noexcept
    {
        DefaultData basePacket;
        basePacket.header = makeHeader(flag, sizeof(DefaultData));

        try {
            return makePacket<DefaultData>(basePacket);
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makePing} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> PacketFactory::makeInput(
        uint32_t entity, const int8_t dx, const int8_t dy, uint8_t shooting) const noexcept
    {
        InputData inputPacket;
        inputPacket.header = makeHeader(Net::Protocol::INPUT, sizeof(InputData));
        inputPacket.entity = htonl(entity);
        inputPacket.dx = dx;
        inputPacket.dy = dy;
        inputPacket.shooting = shooting;

        try {
            return makePacket<InputData>(inputPacket);
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makeInput} " << e.what() << std::endl;
            return nullptr;
        }
    }

} // namespace Network
