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

    std::shared_ptr<Net::IPacket> TCPPacketFactory::make(const std::vector<uint8_t> &payload) const
    {
        if (!_packet)
            return nullptr;

        auto p = _packet->newPacket();

        if (!p || payload.size() > p->capacity())
            return nullptr;

        if (!payload.empty())
            std::memcpy(p->buffer(), payload.data(), payload.size());
        p->setSize(static_cast<uint32_t>(payload.size()));
        return p;
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeHello(const uint32_t req, const uint16_t ver) const
    {
        try {
            Net::TCP::Writer b;
            b.u16(ver);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::HELLO, req, b.bytes());
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeHello} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeListRooms(const uint32_t req) const
    {
        try {
            static const std::vector<uint8_t> empty{};
            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::LIST_ROOMS, req, empty);
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeListRooms} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeCreateRoom(const uint32_t req, const std::string_view roomName,
        const uint8_t maxPlayers, const Engine::Difficulty difficulty, const std::string_view levelPath) const
    {
        try {
            Net::TCP::Writer b;
            b.str16(roomName);
            b.u8(maxPlayers);
            b.u8(static_cast<uint8_t>(difficulty));
            b.str16(levelPath);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::CREATE_ROOM, req, b.bytes());
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeCreateRoom} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeJoinRoom(const uint32_t req, const uint32_t roomId) const
    {
        try {
            Net::TCP::Writer b;
            b.u32(roomId);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::JOIN_ROOM, req, b.bytes());
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeJoinRoom} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeLeaveRoom(const uint32_t req) const
    {
        try {
            static const std::vector<uint8_t> empty{};
            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::LEAVE_ROOM, req, empty);
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeLeaveRoom} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeStartGame(const uint32_t req) const
    {
        try {
            static const std::vector<uint8_t> empty{};
            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::START_GAME, req, empty);
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeStartGame} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeAuthRegister(
        const uint32_t req, const std::string_view username, const std::string_view password) const
    {
        try {
            Net::TCP::Writer b;
            b.str16(username);
            b.str16(password);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::AUTH_REGISTER, req, b.bytes());
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeAuthRegister} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeAuthLogin(
        const uint32_t req, const std::string_view username, const std::string_view password) const
    {
        try {
            Net::TCP::Writer b;
            b.str16(username);
            b.str16(password);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::AUTH_LOGIN, req, b.bytes());
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeAuthLogin} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeScoreboardGet(const uint32_t req, const uint16_t limit) const
    {
        try {
            Net::TCP::Writer b;
            b.u16(limit);
            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::SCOREBOARD_GET, req, b.bytes());
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeScoreboardGet} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeRoomInfo(const uint32_t req) const
    {
        try {
            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::ROOM_INFO, req, {});
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeRoomInfo} error creating packet" << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<Net::IPacket> TCPPacketFactory::makeRoomMessage(
        const uint32_t req, const std::string_view message) const
    {
        try {
            if (message.empty())
                return nullptr;
            Net::TCP::Writer b;
            b.str16(message);

            const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::ROOM_MESSAGE, req, b.bytes());
            return make(payload);
        } catch (...) {
            std::cerr << "{TCPPacketFactory::makeRoomMessage} error creating packet" << std::endl;
            return nullptr;
        }
    }
} // namespace Network
