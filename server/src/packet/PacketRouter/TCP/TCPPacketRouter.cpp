/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketRouter
*/

#include "TCPPacketRouter.hpp"

namespace Net
{
    TCPPacketRouter::TCPPacketRouter(std::shared_ptr<Server::ISessionManager> sessions,
        std::shared_ptr<Engine::RoomManager> rooms, std::shared_ptr<Server::IServer> tcpServer,
        std::shared_ptr<Factory::TCPPacketFactory> packetFactory)
        : _sessions(std::move(sessions)), _rooms(std::move(rooms)), _tcp(std::move(tcpServer)),
          _packetFactory(std::move(packetFactory))
    {
    }

    void TCPPacketRouter::handle(const std::shared_ptr<IPacket> &pkt) const
    {
        const auto addr = pkt->address();
        if (!addr || pkt->size() < 5)
            return;

        const auto *payload = pkt->buffer();
        const size_t n = pkt->size();

        TCP::Header h;
        TCP::Reader r(payload, n);
        try {
            h = TCP::parseHeader(payload, n);
            r = TCP::bodyReader(payload, n);
        } catch (...) {
            sendError(*addr, 0, 1, "malformed payload");
            return;
        }

        const int sessionId = _sessions->getOrCreateSession(*addr);

        switch (h.type) {
            case Protocol::TCP::HELLO: onHello(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::LIST_ROOMS: onListRooms(*addr, h.requestId); break;
            case Protocol::TCP::CREATE_ROOM: onCreateRoom(*addr, h.requestId, r); break;
            case Protocol::TCP::JOIN_ROOM: onJoinRoom(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::LEAVE_ROOM: onLeaveRoom(*addr, sessionId, h.requestId); break;
            case Protocol::TCP::START_GAME: onStartGame(*addr, sessionId); break;
            default: sendError(*addr, h.requestId, 2, "unknown type"); break;
        }
    }

    void TCPPacketRouter::sendError(
        const sockaddr_in &addr, const uint32_t req, const uint16_t code, const std::string_view msg) const
    {
        TCP::Writer b;
        b.u16(code);
        b.str16(msg);
        const auto payload = TCP::buildPayload(Protocol::TCP::ERROR_MESSAGE, req, b.bytes());
        _tcp->sendPacket(*_packetFactory->make(addr, payload));
    }

    void TCPPacketRouter::onHello(
        const sockaddr_in &addr, const int sessionId, const uint32_t req, TCP::Reader &r) const
    {
        uint16_t ver = 0;
        constexpr uint16_t _serverUdpPort = 8081;

        try {
            ver = r.u16();
        } catch (...) {
            return sendError(addr, req, 3, "bad HELLO");
        }

        std::random_device rd;
        const uint64_t token =
            (static_cast<uint64_t>(rd()) << 32) ^ static_cast<uint64_t>(rd()) ^ (static_cast<uint64_t>(sessionId) << 1);

        _sessions->setUdpToken(sessionId, token);

        TCP::Writer b;
        b.u16(ver);
        b.u32(static_cast<uint32_t>(sessionId));
        b.u16(_serverUdpPort);
        b.u32(static_cast<uint32_t>(token >> 32));
        b.u32(static_cast<uint32_t>(token & 0xFFFFFFFFu));

        const auto payload = TCP::buildPayload(Protocol::TCP::WELCOME, req, b.bytes());
        _tcp->sendPacket(*_packetFactory->make(addr, payload));
    }

    void TCPPacketRouter::onListRooms(const sockaddr_in &addr, const uint32_t req) const
    {
        TCP::Writer b;
        const auto rooms = _rooms->listRooms();

        if (rooms.size() > 0xFFFFu)
            return;

        b.u16(static_cast<uint16_t>(rooms.size()));
        for (const auto &[id, name, currentPlayers, maxPlayers] : rooms) {
            b.u32(id);
            b.str16(name);
            b.u8(static_cast<uint8_t>(currentPlayers));
            b.u8(static_cast<uint8_t>(maxPlayers));
        }

        const auto payload = TCP::buildPayload(Protocol::TCP::ROOMS_LIST, req, b.bytes());
        _tcp->sendPacket(*_packetFactory->make(addr, payload));
    }

    void TCPPacketRouter::onCreateRoom(const sockaddr_in &addr, const uint32_t req, TCP::Reader &r) const
    {
        std::string roomName;
        uint8_t maxPlayers = 0;

        try {
            roomName = r.str16();
            maxPlayers = r.u8();
        } catch (...) {
            return sendError(addr, req, 4, "bad CREATE_ROOM");
        }

        if (roomName.empty() || roomName.size() > 32)
            return sendError(addr, req, 6, "invalid roomName");

        if (maxPlayers < 1 || maxPlayers > 4)
            return sendError(addr, req, 5, "invalid maxPlayers");

        if (r.remaining() != 0)
            return sendError(addr, req, 7, "extra bytes");

        uint32_t roomId = 0;
        try {
            roomId = _rooms->createRoom(roomName, maxPlayers);
        } catch (const std::exception &e) {
            return sendError(addr, req, 8, e.what());
        }

        if (roomId == 0)
            return sendError(addr, req, 9, "createRoom failed");

        TCP::Writer b;
        b.u32(roomId);
        const auto payload = TCP::buildPayload(Protocol::TCP::ROOM_CREATED, req, b.bytes());
        _tcp->sendPacket(*_packetFactory->make(addr, payload));
    }

    void TCPPacketRouter::onJoinRoom(
        const sockaddr_in &addr, const int sessionId, const uint32_t req, TCP::Reader &r) const
    {
        uint32_t roomId = 0;

        try {
            roomId = r.u32();
        } catch (...) {
            return sendError(addr, req, 10, "bad JOIN_ROOM");
        }

        if (r.remaining() != 0)
            return sendError(addr, req, 11, "extra bytes");

        try {
            _rooms->addPlayerToRoom(roomId, sessionId);
        } catch (const std::exception &e) {
            return sendError(addr, req, 12, e.what());
        }

        TCP::Writer b;
        b.u32(roomId);
        const auto payload = TCP::buildPayload(Protocol::TCP::ROOM_JOINED, req, b.bytes());
        _tcp->sendPacket(*_packetFactory->make(addr, payload));
    }

    void TCPPacketRouter::onLeaveRoom(const sockaddr_in &addr, int sessionId, uint32_t req) const
    {
        uint32_t roomId = 0;

        try {
            roomId = _rooms->removePlayer(sessionId);
            if (roomId == 0)
                return sendError(addr, req, 13, "not in a room");
        } catch (const std::exception &e) {
            return sendError(addr, req, 13, e.what());
        }

        TCP::Writer b;
        b.u32(roomId);

        const auto payload = TCP::buildPayload(Protocol::TCP::ROOM_LEFT, req, b.bytes());
        _tcp->sendPacket(*_packetFactory->make(addr, payload));
    }

    void TCPPacketRouter::onStartGame(const sockaddr_in &addr, const int sessionId) const
    {
        const uint32_t roomId = _rooms->getRoomIdOfPlayer(sessionId);
        if (roomId == 0)
            return sendError(addr, 0, 14, "not in a room");

        if (!_rooms->start(roomId))
            return sendError(addr, 0, 15, "cannot start game");

        TCP::Writer b;
        b.u32(roomId);

        const auto payload = TCP::buildPayload(Protocol::TCP::GAME_START, 0, b.bytes());

        const auto &room = _rooms->getRoomById(roomId);
        if (!room)
            return;
        for (const auto &session : room->sessions()) {
            if (const auto memberAddr = _sessions->getAddress(session))
                _tcp->sendPacket(*_packetFactory->make(*memberAddr, payload));
        }
    }
} // namespace Net
