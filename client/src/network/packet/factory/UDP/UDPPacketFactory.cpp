/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacketFactory
*/

#include "UDPPacketFactory.hpp"

namespace Network
{
    UDPPacketFactory::UDPPacketFactory(const std::shared_ptr<Net::IPacket> &packet)
    {
        if (!packet)
            throw FactoryError("{UDPPacketFactory::UDPPacketFactory}: invalid IPacket pointer");
        _packet = packet;
    }

    HeaderData UDPPacketFactory::makeHeader(const uint8_t type, const uint16_t size) noexcept
    {
        HeaderData header;
        header.type = type;
        header.version = VERSION;
        header.size = htons(size);
        return header;
    }

    std::shared_ptr<Net::IPacket> UDPPacketFactory::makeBase(const uint8_t flag) const noexcept
    {
        DefaultData basePacket;
        basePacket.header = makeHeader(flag, sizeof(DefaultData));

        try {
            return makePacket<DefaultData>(basePacket);
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::makePing} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> UDPPacketFactory::makeInput(const PlayerInput &input) const noexcept
    {
        PlayerInputData packet{};
        packet.header = makeHeader(Net::Protocol::UDP::INPUT, sizeof(PlayerInputData));

        packet.flags = 0;
        if (input.up)
            packet.flags |= 0x01;
        if (input.down)
            packet.flags |= 0x02;
        if (input.left)
            packet.flags |= 0x04;
        if (input.right)
            packet.flags |= 0x08;
        if (input.shoot)
            packet.flags |= 0x10;

        try {
            return makePacket<PlayerInputData>(packet);
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::makePlayerInput} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> UDPPacketFactory::makeConnect(const ConnectInfo &connect) const noexcept
    {
        ConnectData packet{};
        packet.header = makeHeader(Net::Protocol::UDP::CONNECT, sizeof(ConnectData));
        packet.sessionId = htonl(connect.sessionId);
        packet.tokenHi = htonl(static_cast<uint32_t>(connect.token >> 32));
        packet.tokenLo = htonl(static_cast<uint32_t>(connect.token));

        try {
            return makePacket<ConnectData>(packet);
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::makeConnect} " << e.what() << std::endl;
            return nullptr;
        }
    }
} // namespace Network
