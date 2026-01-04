/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketFactory
*/

#include "TCPPacketFactory.hpp"

namespace Network
{
    TCPPacketFactory::TCPPacketFactory(std::shared_ptr<Net::IPacket> proto) : _packet(std::move(proto))
    {
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::make(const std::vector<std::uint8_t> &payload) const
    {
        if (!_packet)
            return nullptr;

        auto p = _packet->newPacket();

        if (!p || payload.size() > p->capacity())
            return nullptr;

        std::memcpy(p->buffer(), payload.data(), payload.size());
        p->setSize(static_cast<std::uint32_t>(payload.size()));
        return p;
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeHello(const std::uint32_t req, const std::uint16_t ver) const
    {
        try {
            Net::TCP::Writer b;
            b.u16(ver);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::HELLO, req, b.bytes());
            return make(payload);
        } catch (...) {
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeListRooms(const std::uint32_t req) const
    {
        try {
            constexpr std::vector<std::uint8_t> empty{};
            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::LIST_ROOMS, req, empty);
            return make(payload);
        } catch (...) {
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeCreateRoom(
        const std::uint32_t req, const std::string_view roomName, const std::uint8_t maxPlayers) const
    {
        try {
            Net::TCP::Writer b;
            b.str16(roomName);
            b.u8(maxPlayers);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::CREATE_ROOM, req, b.bytes());
            return make(payload);
        } catch (...) {
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeJoinRoom(
        const std::uint32_t req, const std::uint32_t roomId) const
    {
        try {
            Net::TCP::Writer b;
            b.u32(roomId);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::JOIN_ROOM, req, b.bytes());
            return make(payload);
        } catch (...) {
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeLeaveRoom(const std::uint32_t req) const
    {
        try {
            constexpr std::vector<std::uint8_t> empty{};
            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::LEAVE_ROOM, req, empty);
            return make(payload);
        } catch (...) {
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeStartGame(const std::uint32_t req) const
    {
        try {
            constexpr std::vector<std::uint8_t> empty{};
            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::START_GAME, req, empty);
            return make(payload);
        } catch (...) {
            return nullptr;
        }
    }
} // namespace Network
