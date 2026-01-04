/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketRouter
*/

#include "TCPPacketRouter.hpp"

namespace
{
    std::string malformedTcp(const char *stage, const std::size_t need, const std::size_t got)
    {
        std::string s = "TCP ";
        s += stage;
        s += ": truncated payload (need=" + std::to_string(need) + ", got=" + std::to_string(got) + ")";
        return s;
    }
} // namespace

namespace Net
{
    TCPPacketRouter::TCPPacketRouter(std::shared_ptr<Server::ISessionManager> sessions,
        std::shared_ptr<Engine::RoomManager> rooms, std::shared_ptr<Server::IServer> tcpServer,
        std::shared_ptr<Factory::TCPPacketFactory> packetFactory)
        : _sessions(std::move(sessions)), _rooms(std::move(rooms)), _tcp(std::move(tcpServer)),
          _packetFactory(std::move(packetFactory)), _serverUdpPort(8081)
    {
    }

    void TCPPacketRouter::handle(const std::shared_ptr<IPacket> &pkt) const
    {
        const auto addr = pkt->address();
        if (!addr)
            return;

        const size_t n = pkt->size();
        const auto *payload = pkt->buffer();

        if (n < 5)
            return sendError(*addr, 0, 1, malformedTcp("header", 5, n));

        TCP::Header h{};
        TCP::Reader r(payload, n);
        try {
            h = TCP::parseHeader(payload, n);
            r = TCP::bodyReader(payload, n);
        } catch (const std::exception &e) {
            return sendError(*addr, 0, 1, std::string("TCP header: parse failed: ") + e.what());
        } catch (...) {
            return sendError(*addr, 0, 1, "TCP header: parse failed: unknown error");
        }

        const int sessionId = _sessions->getOrCreateSession(*addr);

        switch (h.type) {
            case Protocol::TCP::HELLO: onHello(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::LIST_ROOMS: onListRooms(*addr, h.requestId); break;
            case Protocol::TCP::CREATE_ROOM: onCreateRoom(*addr, h.requestId, r); break;
            case Protocol::TCP::JOIN_ROOM: onJoinRoom(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::LEAVE_ROOM: onLeaveRoom(*addr, sessionId, h.requestId); break;
            case Protocol::TCP::START_GAME: onStartGame(*addr, sessionId, h.requestId); break;
            default: sendError(*addr, h.requestId, 2, "Unsupported TCP packet type"); break;
        }
    }

    void TCPPacketRouter::sendError(
        const sockaddr_in &addr, const uint32_t req, const uint16_t code, const std::string_view msg) const
    {
        if (!_packetFactory)
            return;

        const auto out = _packetFactory->makeError(addr, req, code, msg);
        if (!out)
            return;

        _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onHello(
        const sockaddr_in &addr, const int sessionId, const uint32_t req, TCP::Reader &r) const
    {
        if (r.remaining() < 2)
            return sendError(addr, req, 3, malformedTcp("HELLO ver(u16)", 2, r.remaining()));

        uint16_t ver = 0;
        try {
            ver = r.u16();
        } catch (...) {
            return sendError(addr, req, 3, malformedTcp("HELLO ver(u16)", 2, r.remaining()));
        }

        std::random_device rd;
        const uint64_t token =
            (static_cast<uint64_t>(rd()) << 32) ^ static_cast<uint64_t>(rd()) ^ (static_cast<uint64_t>(sessionId) << 1);

        _sessions->setUdpToken(sessionId, token);

        if (!_packetFactory)
            return;

        const auto out =
            _packetFactory->makeWelcome(addr, req, ver, static_cast<uint32_t>(sessionId), _serverUdpPort, token);
        if (!out)
            return;

        _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onListRooms(const sockaddr_in &addr, const uint32_t req) const
    {
        if (!_packetFactory)
            return;

        const auto rooms = _rooms->listRooms();
        if (rooms.size() > 0xFFFFu)
            return sendError(addr, req, 16, "LIST_ROOMS: too many rooms to fit in u16");

        std::vector<RoomData> outRooms;
        outRooms.reserve(rooms.size());

        for (const auto &[id, name, currentPlayers, maxPlayers] : rooms) {
            RoomData ri{};
            ri.roomId = static_cast<uint32_t>(id);
            ri.roomName = name;
            ri.currentPlayers = static_cast<size_t>(currentPlayers);
            ri.maxPlayers = static_cast<size_t>(maxPlayers);
            outRooms.push_back(ri);
        }

        const auto out = _packetFactory->makeRoomsList(addr, req, outRooms);
        if (!out)
            return;

        _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onCreateRoom(const sockaddr_in &addr, const uint32_t req, TCP::Reader &r) const
    {
        std::string roomName;
        uint8_t maxPlayers = 0;

        try {
            roomName = r.str16();
            maxPlayers = r.u8();
        } catch (...) {
            return sendError(addr, req, 4, "CREATE_ROOM: malformed payload (expected name(str16) + maxPlayers(u8))");
        }

        if (roomName.empty() || roomName.size() > 32)
            return sendError(addr, req, 6, "CREATE_ROOM: roomName must be 1..32 characters");

        if (maxPlayers < 1 || maxPlayers > 4)
            return sendError(addr, req, 5, "CREATE_ROOM: maxPlayers must be in range 1..4");

        if (r.remaining() != 0)
            return sendError(addr, req, 7, "CREATE_ROOM: unexpected trailing bytes");

        uint32_t roomId = 0;
        try {
            roomId = _rooms->createRoom(roomName, maxPlayers);
        } catch (const std::exception &e) {
            return sendError(addr, req, 8, e.what());
        }

        if (roomId == 0)
            return sendError(addr, req, 9, "CREATE_ROOM: failed to create room");

        if (!_packetFactory)
            return;

        const auto out = _packetFactory->makeRoomCreated(addr, req, roomId);
        if (!out)
            return;

        _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onJoinRoom(
        const sockaddr_in &addr, const int sessionId, const uint32_t req, TCP::Reader &r) const
    {
        uint32_t roomId = 0;

        try {
            roomId = r.u32();
        } catch (...) {
            return sendError(addr, req, 10, "JOIN_ROOM: malformed payload (expected roomId(u32))");
        }

        if (r.remaining() != 0)
            return sendError(addr, req, 11, "JOIN_ROOM: unexpected trailing bytes");

        try {
            _rooms->addPlayerToRoom(roomId, sessionId);
        } catch (const std::exception &e) {
            return sendError(addr, req, 12, e.what());
        }

        if (!_packetFactory)
            return;

        const auto out = _packetFactory->makeRoomJoined(addr, req, roomId);
        if (!out)
            return;

        _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onLeaveRoom(const sockaddr_in &addr, int sessionId, uint32_t req) const
    {
        uint32_t roomId = 0;

        try {
            roomId = _rooms->removePlayer(sessionId);
            if (roomId == 0)
                return sendError(addr, req, 13, "LEAVE_ROOM: player is not in a room");
        } catch (const std::exception &e) {
            return sendError(addr, req, 13, e.what());
        }

        if (!_packetFactory)
            return;

        const auto out = _packetFactory->makeRoomLeft(addr, req, roomId);
        if (!out)
            return;

        _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onStartGame(const sockaddr_in &addr, const int sessionId, const uint32_t req) const
    {
        const uint32_t roomId = _rooms->getRoomIdOfPlayer(sessionId);
        if (roomId == 0)
            return sendError(addr, req, 14, "START_GAME: player is not in a room");

        if (!_rooms->start(roomId))
            return sendError(addr, req, 15, "START_GAME: room cannot be started (state/players)");

        const auto &room = _rooms->getRoomById(roomId);
        if (!room || !_packetFactory)
            return;

        for (const auto &session : room->sessions()) {
            if (const auto memberAddr = _sessions->getAddress(session)) {
                if (const auto out = _packetFactory->makeGameStart(*memberAddr, 0, roomId))
                    _tcp->sendPacket(*out);
            }
        }
    }
} // namespace Net
