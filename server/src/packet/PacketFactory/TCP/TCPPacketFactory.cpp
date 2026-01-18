/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketFactory
*/

#include "TCPPacketFactory.hpp"

namespace Net::Factory
{
    TCPPacketFactory::TCPPacketFactory(const std::shared_ptr<IPacket> &proto) : _packet(proto)
    {
    }

    std::shared_ptr<IPacket> TCPPacketFactory::make(const sockaddr_in &addr, const std::vector<uint8_t> &payload) const
    {
        if (!_packet)
            return nullptr;

        auto p = _packet->newPacket();
        if (!p || payload.size() > p->capacity())
            return nullptr;

        p->setAddress(addr);
        std::memcpy(p->buffer(), payload.data(), payload.size());
        p->setSize(static_cast<uint32_t>(payload.size()));
        return p;
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeError(
        const sockaddr_in &addr, const ReqId req, const uint16_t code, const std::string_view msg) const
    {
        TCP::Writer b;
        b.u16(code);
        b.str16(msg);

        const auto payload = TCP::buildPayload(Protocol::TCP::ERROR_MESSAGE, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeWelcome(const sockaddr_in &addr, const ReqId req, const uint16_t ver,
        const SessionId sessionId, const uint16_t udpPort, const uint64_t token) const
    {
        TCP::Writer b;
        b.u16(ver);
        b.u32(sessionId);
        b.u16(udpPort);
        b.u32(static_cast<uint32_t>(token >> 32));
        b.u32(static_cast<uint32_t>(token & 0xFFFFFFFFu));

        const auto payload = TCP::buildPayload(Protocol::TCP::WELCOME, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeRoomsList(
        const sockaddr_in &addr, const ReqId req, const std::vector<RoomData> &rooms) const
    {
        if (rooms.size() > 0xFFFFu)
            return makeError(addr, req, 16, "LIST_ROOMS: too many rooms to fit in u16");

        TCP::Writer b;
        b.u16(static_cast<uint16_t>(rooms.size()));

        for (const auto &[roomId, roomName, currentPlayers, maxPlayers, gameConfig, names] : rooms) {
            b.u32(roomId);
            b.str16(roomName);
            b.u16(static_cast<uint16_t>(currentPlayers));
            b.u16(static_cast<uint16_t>(maxPlayers));
            b.u8(static_cast<uint8_t>(gameConfig.difficulty));
            b.str16(gameConfig.levelId);
            b.str16(gameConfig.worldMusic);
        }

        const auto payload = TCP::buildPayload(Protocol::TCP::ROOMS_LIST, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeRoomCreated(
        const sockaddr_in &addr, const ReqId req, const RoomId roomId) const
    {
        TCP::Writer b;
        b.u32(roomId);

        const auto payload = TCP::buildPayload(Protocol::TCP::ROOM_CREATED, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeRoomJoined(
        const sockaddr_in &addr, const ReqId req, const RoomId roomId) const
    {
        TCP::Writer b;
        b.u32(roomId);

        const auto payload = TCP::buildPayload(Protocol::TCP::ROOM_JOINED, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeRoomLeft(
        const sockaddr_in &addr, const ReqId req, const RoomId roomId) const
    {
        TCP::Writer b;
        b.u32(roomId);

        const auto payload = TCP::buildPayload(Protocol::TCP::ROOM_LEFT, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeGameStart(
        const sockaddr_in &addr, const ReqId req, const RoomId roomId) const
    {
        TCP::Writer b;
        b.u32(roomId);

        const auto payload = TCP::buildPayload(Protocol::TCP::GAME_START, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeAuthOk(const sockaddr_in &addr, const ReqId req,
        const uint32_t userId, const std::string_view username, const uint64_t token, const uint32_t ttlSec) const
    {
        TCP::Writer b;
        b.u32(userId);
        b.str16(username);
        b.u32(static_cast<uint32_t>(token >> 32));
        b.u32(static_cast<uint32_t>(token & 0xFFFFFFFFu));
        b.u32(ttlSec);

        const auto payload = TCP::buildPayload(Protocol::TCP::AUTH_OK, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeScoreboardList(
        const sockaddr_in &addr, const ReqId req, const std::vector<ScoreEntry> &scores) const
    {
        if (scores.size() > 0xFFFFu)
            return makeError(addr, req, 16, "SCOREBOARD_GET: too many rows to fit in u16");

        TCP::Writer b;
        b.u16(static_cast<uint16_t>(scores.size()));
        for (const auto &[username, score] : scores) {
            b.str16(username);
            b.u32(static_cast<uint32_t>(score));
        }
        const auto payload = TCP::buildPayload(Protocol::TCP::SCOREBOARD_LIST, req, b.bytes());
        return make(addr, payload);
    }

    std::shared_ptr<IPacket> TCPPacketFactory::makeRoomUpdated(
        const sockaddr_in &addr, const ReqId req, const RoomData &room) const
    {
        if (room.playerNames.size() > 0xFFFFu)
            return makeError(addr, req, 16, "ROOM_UPDATED: too many players to fit in u16");
        TCP::Writer b;
        b.str16(room.roomName);
        b.u8(static_cast<uint8_t>(room.maxPlayers));
        b.u16(static_cast<uint16_t>(room.playerNames.size()));
        for (const auto &name : room.playerNames)
            b.str16(name);
        b.u8(static_cast<uint8_t>(room.gameConfig.difficulty));
        b.str16(room.gameConfig.levelId);
        b.str16(room.gameConfig.worldMusic);
        const auto payload = TCP::buildPayload(Protocol::TCP::ROOM_UPDATE, req, b.bytes());
        return make(addr, payload);
    }
} // namespace Net::Factory
