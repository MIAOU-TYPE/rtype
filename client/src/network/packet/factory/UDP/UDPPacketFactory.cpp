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
        static uint32_t sequenceCounter = 1;
        HeaderData header{};

        std::memcpy(header.magic, kPacketMagic, sizeof(header.magic));
        header.type = type;
        header.version = VERSION;
        header.size = htons(size);
        header.sequence = htonl(sequenceCounter);
        sequenceCounter++;
        return header;
    }

    std::shared_ptr<Net::IPacket> UDPPacketFactory::makeBase(const uint8_t flag) const noexcept
    {
        DefaultData basePacket;
        basePacket.header = makeHeader(flag, sizeof(DefaultData));

        try {
            return makePacket<DefaultData>(basePacket);
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::makeBase} " << e.what() << std::endl;
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
        if (input.powerShoot)
            packet.flags |= 0x20;

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

    std::shared_ptr<Net::IPacket> UDPPacketFactory::makePing() const noexcept
    {
        PongData packet{};
        packet.header = makeHeader(Net::Protocol::UDP::PING, sizeof(PongData));
        const auto now =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                .count();
        packet.pongTimestamp = htonll(static_cast<uint64_t>(now));

        try {
            return makePacket<PongData>(packet);
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::makePing} " << e.what() << std::endl;
            return nullptr;
        }
    }
} // namespace Network
