/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketRouter
*/

#include "TCPPacketRouter.hpp"

namespace
{
    std::string malformedTcp(const char *stage, const size_t need, const size_t got)
    {
        std::string s = "TCP ";
        s += stage;
        s += ": truncated payload (need=" + std::to_string(need) + ", got=" + std::to_string(got) + ")";
        return s;
    }

    bool readRoomId(Net::TCP::Reader &r, uint32_t &roomId) noexcept
    {
        try {
            roomId = r.u32();
            return true;
        } catch (...) {
            return false;
        }
    }
} // namespace

namespace Network
{
    TCPPacketRouter::TCPPacketRouter()
    {
        _sink = std::make_shared<TCPMessageSink>();
    }

    void TCPPacketRouter::protocolError(const uint32_t req, const std::string_view msg) const
    {
        if (_sink)
            _sink->onProtocolError(req, msg);
    }

    void TCPPacketRouter::handle(const std::shared_ptr<Net::IPacket> &pkt) const
    {
        if (!_sink || !pkt)
            return;

        if (const auto addr = pkt->address(); !addr)
            return;

        const size_t n = pkt->size();
        const auto *payload = pkt->buffer();

        if (n < 5)
            return protocolError(0, malformedTcp("header", 5, n));

        Net::TCP::Header h{};
        Net::TCP::Reader r(payload, n);

        try {
            h = Net::TCP::parseHeader(payload, n);
            r = Net::TCP::bodyReader(payload, n);
        } catch (const std::exception &e) {
            return protocolError(0, std::string("TCP header: parse failed: ") + e.what());
        } catch (...) {
            return protocolError(0, "TCP header: parse failed: unknown error");
        }

        try {
            switch (h.type) {
                case Net::Protocol::TCP::WELCOME: onWelcome(h.requestId, r); break;
                case Net::Protocol::TCP::ERROR_MESSAGE: onError(h.requestId, r); break;
                case Net::Protocol::TCP::AUTH_OK: onAuthOk(h.requestId, r); break;
                case Net::Protocol::TCP::ROOMS_LIST: onRoomsList(h.requestId, r); break;
                case Net::Protocol::TCP::ROOM_CREATED: onRoomCreated(h.requestId, r); break;
                case Net::Protocol::TCP::ROOM_JOINED: onRoomJoined(h.requestId, r); break;
                case Net::Protocol::TCP::ROOM_LEFT: onRoomLeft(h.requestId, r); break;
                case Net::Protocol::TCP::GAME_START: onGameStart(h.requestId, r); break;
                case Net::Protocol::TCP::SCOREBOARD_LIST: onScoreboardList(h.requestId, r); break;
                case Net::Protocol::TCP::ROOM_UPDATE: onRoomUpdated(h.requestId, r); break;
                default: protocolError(h.requestId, "Unsupported TCP packet type (client)"); break;
            }
        } catch (const std::exception &e) {
            protocolError(h.requestId, std::string("TCP body: parse failed: ") + e.what());
        } catch (...) {
            protocolError(h.requestId, "TCP body: parse failed: unknown error");
        }
    }

    std::shared_ptr<IMessageHandler> TCPPacketRouter::sink() const noexcept
    {
        return _sink;
    }

    void TCPPacketRouter::onWelcome(const uint32_t req, Net::TCP::Reader &r) const
    {
        if (r.remaining() < (2u + 4u + 2u + 4u + 4u))
            return protocolError(req, malformedTcp("WELCOME", 16, r.remaining()));

        uint16_t ver = 0;
        uint32_t sessionId = 0;
        uint16_t udpPort = 0;
        uint32_t tokenHi = 0;
        uint32_t tokenLo = 0;

        try {
            ver = r.u16();
            sessionId = r.u32();
            udpPort = r.u16();
            tokenHi = r.u32();
            tokenLo = r.u32();
        } catch (...) {
            return protocolError(req,
                "WELCOME: malformed payload (expected ver(u16)+sessionId(u32)+udpPort(u16)+tokenHi(u32)+tokenLo(u32))");
        }

        if (r.remaining() != 0)
            return protocolError(req, "WELCOME: unexpected trailing bytes");

        const uint64_t token = (static_cast<uint64_t>(tokenHi) << 32) | static_cast<uint64_t>(tokenLo);

        _sink->onWelcome(req, ver, sessionId, udpPort, token);
    }

    void TCPPacketRouter::onError(const uint32_t req, Net::TCP::Reader &r) const
    {
        if (r.remaining() < 2u)
            return protocolError(req, malformedTcp("ERROR_MESSAGE code(u16)", 2, r.remaining()));

        uint16_t code = 0;
        std::string msg;

        try {
            code = r.u16();
            msg = r.str16();
        } catch (...) {
            return protocolError(req, "ERROR_MESSAGE: malformed payload (expected code(u16)+msg(str16))");
        }

        if (r.remaining() != 0)
            return protocolError(req, "ERROR_MESSAGE: unexpected trailing bytes");

        _sink->onError(req, code, msg);
    }

    void TCPPacketRouter::onRoomsList(const uint32_t req, Net::TCP::Reader &r) const
    {
        if (r.remaining() < 2u)
            return protocolError(req, malformedTcp("ROOMS_LIST count(u16)", 2, r.remaining()));

        uint16_t count = 0;
        try {
            count = r.u16();
        } catch (...) {
            return protocolError(req, "ROOMS_LIST: malformed payload (expected count(u16))");
        }

        std::vector<RoomData> rooms;
        rooms.reserve(count);

        for (uint16_t i = 0; i < count; ++i) {
            RoomData info{};
            try {
                info.roomId = r.u32();
                info.roomName = r.str16();
                info.currentPlayers = r.u16();
                info.maxPlayers = r.u16();
                info.gameConfig.difficulty = static_cast<Engine::Difficulty>(r.u8());
                info.gameConfig.levelId = r.str16();
                info.gameConfig.worldMusic = r.str16();
            } catch (...) {
                return protocolError(req,
                    "ROOMS_LIST: malformed room entry (expected id(u32)+name(str16)+current(u16)+max(u16)+gameConfig)");
            }
            rooms.push_back(std::move(info));
        }

        if (r.remaining() != 0)
            return protocolError(req, "ROOMS_LIST: unexpected trailing bytes");

        _sink->onRoomsList(req, rooms);
    }

    void TCPPacketRouter::onRoomCreated(const uint32_t req, Net::TCP::Reader &r) const
    {
        uint32_t roomId = 0;
        if (!readRoomId(r, roomId))
            return protocolError(req, "ROOM_CREATED: malformed payload (expected roomId(u32))");
        if (r.remaining() != 0)
            return protocolError(req, "ROOM_CREATED: unexpected trailing bytes");
        _sink->onRoomCreated(req, roomId);
    }

    void TCPPacketRouter::onRoomJoined(const uint32_t req, Net::TCP::Reader &r) const
    {
        uint32_t roomId = 0;
        if (!readRoomId(r, roomId))
            return protocolError(req, "ROOM_JOINED: malformed payload (expected roomId(u32))");
        if (r.remaining() != 0)
            return protocolError(req, "ROOM_JOINED: unexpected trailing bytes");
        _sink->onRoomJoined(req, roomId);
    }

    void TCPPacketRouter::onRoomLeft(const uint32_t req, Net::TCP::Reader &r) const
    {
        uint32_t roomId = 0;
        if (!readRoomId(r, roomId))
            return protocolError(req, "ROOM_LEFT: malformed payload (expected roomId(u32))");
        if (r.remaining() != 0)
            return protocolError(req, "ROOM_LEFT: unexpected trailing bytes");
        _sink->onRoomLeft(req, roomId);
    }

    void TCPPacketRouter::onGameStart(const uint32_t req, Net::TCP::Reader &r) const
    {
        uint32_t roomId = 0;
        if (!readRoomId(r, roomId))
            return protocolError(req, "GAME_START: malformed payload (expected roomId(u32))");
        if (r.remaining() != 0)
            return protocolError(req, "GAME_START: unexpected trailing bytes");
        _sink->onGameStart(req, roomId);
    }

    void TCPPacketRouter::onAuthOk(const uint32_t req, Net::TCP::Reader &r) const
    {
        uint32_t userId = 0;
        std::string username;
        uint32_t tokenHi = 0, tokenLo = 0, ttlSec = 0;

        try {
            userId = r.u32();
            username = r.str16();
            tokenHi = r.u32();
            tokenLo = r.u32();
            ttlSec = r.u32();
        } catch (...) {
            return protocolError(req, "AUTH_OK: malformed payload");
        }
        if (r.remaining() != 0)
            return protocolError(req, "AUTH_OK: unexpected trailing bytes");
        const uint64_t token = static_cast<uint64_t>(tokenHi) << 32 | tokenLo;
        _sink->onAuthOk(req, userId, username, token, ttlSec);
    }

    void TCPPacketRouter::onScoreboardList(const uint32_t req, Net::TCP::Reader &r) const
    {
        if (r.remaining() < 2u)
            return protocolError(req, malformedTcp("SCOREBOARD_LIST count(u16)", 2, r.remaining()));

        uint16_t count = 0;
        try {
            count = r.u16();
        } catch (...) {
            return protocolError(req, "SCOREBOARD_LIST: malformed payload (expected count(u16))");
        }
        std::vector<ScoreEntry> scores;
        scores.reserve(count);
        constexpr uint32_t intMaxU32 = (std::numeric_limits<int>::max)();

        for (uint16_t i = 0; i < count; ++i) {
            std::string username;
            uint32_t s = 0;
            try {
                username = r.str16();
                s = r.u32();
            } catch (...) {
                return protocolError(req, "SCOREBOARD_LIST: malformed entry (expected username(str16)+score(u32))");
            }
            ScoreEntry e{};
            e.username = std::move(username);
            if (s > intMaxU32)
                e.score = (std::numeric_limits<int>::max)();
            else
                e.score = static_cast<int>(s);
            scores.push_back(std::move(e));
        }
        if (r.remaining() != 0)
            return protocolError(req, "SCOREBOARD_LIST: unexpected trailing bytes");
        _sink->onScoreboardList(req, scores);
    }

    void TCPPacketRouter::onRoomUpdated(const uint32_t req, Net::TCP::Reader &r) const
    {
        RoomData room{};
        try {
            room.roomName = r.str16();
            room.maxPlayers = r.u8();
            const uint16_t playerCount = r.u16();
            room.playerNames.clear();
            room.playerNames.reserve(playerCount);
            for (uint16_t i = 0; i < playerCount; ++i)
                room.playerNames.push_back(r.str16());
            room.currentPlayers = playerCount;
            room.gameConfig.difficulty = static_cast<Engine::Difficulty>(r.u8());
            room.gameConfig.levelId = r.str16();
            room.gameConfig.worldMusic = r.str16();
        } catch (...) {
            return protocolError(req, "ROOM_UPDATED: malformed payload");
        }

        if (r.remaining() != 0)
            return protocolError(req, "ROOM_UPDATED: unexpected trailing bytes");

        _sink->onRoomUpdated(req, room);
    }

} // namespace Network
