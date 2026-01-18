/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketRouter
*/

#include "TCPPacketRouter.hpp"

namespace
{
    [[nodiscard]] std::string malformedTcp(const char *stage, const size_t need, const size_t got)
    {
        std::string s = "TCP ";
        s += stage;
        s += ": truncated payload (need=" + std::to_string(need) + ", got=" + std::to_string(got) + ")";
        return s;
    }

    [[nodiscard]] uint64_t ensureUdpToken(Net::Server::ISessionManager &sessions, const int sessionId)
    {
        auto token = sessions.getUdpToken(sessionId);
        if (token != 0)
            return token;

        std::random_device rd;
        token =
            (static_cast<uint64_t>(rd()) << 32) ^ static_cast<uint64_t>(rd()) ^ (static_cast<uint64_t>(sessionId) << 1);
        sessions.setUdpToken(sessionId, token);
        return token;
    }
} // namespace

namespace Net
{
    TCPPacketRouter::TCPPacketRouter(std::shared_ptr<Server::ISessionManager> sessions,
        std::shared_ptr<Engine::RoomManager> rooms, std::shared_ptr<Server::IServer> tcpServer,
        std::shared_ptr<Factory::TCPPacketFactory> packetFactory, std::shared_ptr<Auth::AuthService> authService,
        std::shared_ptr<Engine::ScoreService> scoreService)
        : _sessions(std::move(sessions)), _rooms(std::move(rooms)), _tcp(std::move(tcpServer)),
          _packetFactory(std::move(packetFactory)), _auth(std::move(authService)), _scores(std::move(scoreService)),
          _serverUdpPort(_tcp->getPort() + 1)
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

        TCP::Header h;
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

        const bool authFree = h.type == Protocol::TCP::HELLO || h.type == Protocol::TCP::AUTH_REGISTER
            || h.type == Protocol::TCP::AUTH_LOGIN;

        if (!authFree && !_sessions->isAuthed(sessionId))
            return sendError(*addr, h.requestId, 401, "AUTH_REQUIRED");

        switch (h.type) {
            case Protocol::TCP::HELLO: onHello(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::AUTH_REGISTER: onAuthRegister(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::AUTH_LOGIN: onAuthLogin(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::SCOREBOARD_GET: onScoreboardGet(*addr, h.requestId, r); break;
            case Protocol::TCP::LIST_ROOMS: onListRooms(*addr, h.requestId); break;
            case Protocol::TCP::CREATE_ROOM: onCreateRoom(*addr, h.requestId, r); break;
            case Protocol::TCP::JOIN_ROOM: onJoinRoom(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::LEAVE_ROOM: onLeaveRoom(*addr, sessionId, h.requestId); break;
            case Protocol::TCP::START_GAME: onStartGame(*addr, sessionId, h.requestId); break;
            case Protocol::TCP::ROOM_INFO: onRoomInfo(*addr, sessionId, h.requestId, r); break;
            case Protocol::TCP::ROOM_MESSAGE: onMessageRoom(*addr, sessionId, h.requestId, r); break;
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

        (void) _tcp->sendPacket(*out);
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

        auto token = _sessions->getUdpToken(sessionId);
        if (token == 0) {
            std::random_device rd;
            token = (static_cast<uint64_t>(rd()) << 32) ^ static_cast<uint64_t>(rd())
                ^ (static_cast<uint64_t>(sessionId) << 1);
        }
        _sessions->setUdpToken(sessionId, token);

        if (!_packetFactory)
            return;

        const auto out = _packetFactory->makeWelcome(
            addr, req, ver, static_cast<uint32_t>(sessionId), static_cast<uint16_t>(_serverUdpPort), token);
        if (!out)
            return;

        for (size_t i = 0; i < 3; i++)
            (void) _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onAuthRegister(
        const sockaddr_in &addr, const int sessionId, const uint32_t req, TCP::Reader &r) const
    {
        if (!_auth || !_packetFactory)
            return sendError(addr, req, 500, "AUTH_REGISTER: service unavailable");

        std::string username, password;
        try {
            username = r.str16();
            password = r.str16();
        } catch (...) {
            return sendError(addr, req, 400, "AUTH_REGISTER: malformed payload (username str16 + password str16)");
        }
        if (r.remaining() != 0)
            return sendError(addr, req, 400, "AUTH_REGISTER: unexpected trailing bytes");

        Auth::AuthOk ok{};
        try {
            ok = _auth->registerUser(username, password);
        } catch (const Auth::AuthServiceError &e) {
            if (e.kind() == "invalid_input")
                return sendError(addr, req, 400, e.message());
            if (e.kind() == "username_taken")
                return sendError(addr, req, 409, e.message());
            if (e.kind() == "db_error")
                return sendError(addr, req, 500, "Server error (database)");
            if (e.kind() == "crypto_error")
                return sendError(addr, req, 500, "Server error (crypto)");
            return sendError(addr, req, 500, "Server error");
        }

        constexpr uint32_t ttlSec = 24u * 60u * 60u;
        _sessions->setIdentity(sessionId, Auth::Identity{ok.userId, ok.username}, std::chrono::seconds(ttlSec));
        const uint64_t udpToken = ensureUdpToken(*_sessions, sessionId);
        if (const auto out = _packetFactory->makeAuthOk(addr, req, ok.userId, ok.username, udpToken, ttlSec))
            (void) _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onAuthLogin(
        const sockaddr_in &addr, const int sessionId, const uint32_t req, TCP::Reader &r) const
    {
        if (!_auth || !_packetFactory)
            return sendError(addr, req, 500, "AUTH_LOGIN: service unavailable");

        std::string username, password;
        try {
            username = r.str16();
            password = r.str16();
        } catch (...) {
            return sendError(addr, req, 400, "AUTH_LOGIN: malformed payload (username str16 + password str16)");
        }
        if (r.remaining() != 0)
            return sendError(addr, req, 400, "AUTH_LOGIN: unexpected trailing bytes");

        Auth::AuthOk ok{};
        try {
            ok = _auth->login(username, password);
        } catch (const Auth::AuthServiceError &e) {
            if (e.kind() == "invalid_input")
                return sendError(addr, req, 400, e.message());
            if (e.kind() == "invalid_credentials")
                return sendError(addr, req, 401, "Invalid username or password");
            if (e.kind() == "db_error")
                return sendError(addr, req, 500, "Server error (database)");
            if (e.kind() == "crypto_error")
                return sendError(addr, req, 500, "Server error (crypto)");
            return sendError(addr, req, 500, "Server error");
        }

        constexpr uint32_t ttlSec = 24u * 60u * 60u;
        _sessions->setIdentity(sessionId, Auth::Identity{ok.userId, ok.username}, std::chrono::seconds(ttlSec));
        const uint64_t udpToken = ensureUdpToken(*_sessions, sessionId);
        if (const auto out = _packetFactory->makeAuthOk(addr, req, ok.userId, ok.username, udpToken, ttlSec))
            (void) _tcp->sendPacket(*out);
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

        for (const auto &[id, name, currentPlayers, maxPlayers, gameConfig, names] : rooms) {
            RoomData ri{};
            ri.roomId = static_cast<uint32_t>(id);
            ri.roomName = name;
            ri.currentPlayers = static_cast<size_t>(currentPlayers);
            ri.maxPlayers = static_cast<size_t>(maxPlayers);
            ri.gameConfig = gameConfig;
            outRooms.push_back(ri);
        }

        const auto out = _packetFactory->makeRoomsList(addr, req, outRooms);
        if (!out)
            return;

        (void) _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onCreateRoom(const sockaddr_in &addr, const uint32_t req, TCP::Reader &r) const
    {
        if (const auto currentRoom = _rooms->getRoomIdOfPlayer(_sessions->getOrCreateSession(addr)); currentRoom != 0)
            (void) _rooms->removePlayer(_sessions->getOrCreateSession(addr));
        std::string roomName;
        uint8_t maxPlayers = 0;
        uint8_t difficultyRaw = 0;
        uint8_t gameModeRaw = 0;
        std::string levelPath;

        try {
            roomName = r.str16();
            maxPlayers = r.u8();
            difficultyRaw = r.u8();
            gameModeRaw = r.u8();
            levelPath = r.str16();
        } catch (...) {
            return sendError(addr, req, 4,
                "CREATE_ROOM: malformed payload (expected name(str16) + maxPlayers(u8) + difficulty(u8) + gameMode(u8) "
                "+ "
                "levelPath(str16))");
        }

        if (roomName.empty() || roomName.size() > 32)
            return sendError(addr, req, 6, "CREATE_ROOM: roomName must be 1..32 characters");

        if (maxPlayers < 1 || maxPlayers > 4)
            return sendError(addr, req, 5, "CREATE_ROOM: maxPlayers must be in range 1..4");

        Engine::Difficulty difficulty;
        switch (difficultyRaw) {
            case 0: difficulty = Engine::Difficulty::Easy; break;
            case 1: difficulty = Engine::Difficulty::Medium; break;
            case 2: difficulty = Engine::Difficulty::Hard; break;
            default: return sendError(addr, req, 10, "CREATE_ROOM: invalid difficulty value");
        }

        Engine::GameMode gameMode;
        switch (gameModeRaw) {
            case 0: gameMode = Engine::GameMode::Standard; break;
            case 1: gameMode = Engine::GameMode::Coop; break;
            case 2: gameMode = Engine::GameMode::FriendlyFire; break;
            case 3: gameMode = Engine::GameMode::Survival; break;
            default: return sendError(addr, req, 11, "CREATE_ROOM: invalid game mode value");
        }

        if (r.remaining() != 0)
            return sendError(addr, req, 7, "CREATE_ROOM: unexpected trailing bytes");

        Engine::ModeParameters modeParams{};
        switch (gameMode) {
            case Engine::GameMode::FriendlyFire: modeParams.teamDamage = true; break;
            case Engine::GameMode::Coop: modeParams.sharedHealth = true; break;
            case Engine::GameMode::Survival: break;
            case Engine::GameMode::Standard:
            default: break;
        }

        uint32_t roomId = 0;
        try {
            const Engine::GameConfig config{difficulty, gameMode, modeParams, levelPath};
            roomId = _rooms->createRoom(config, roomName, maxPlayers);
        } catch (const std::exception &e) {
            return sendError(addr, req, 8, e.what());
        }

        if (roomId == 0)
            return sendError(addr, req, 9, "CREATE_ROOM: failed to create room");

        (void) _rooms->addPlayerToRoom(roomId, _sessions->getOrCreateSession(addr));
        if (!_packetFactory)
            return;

        const auto out = _packetFactory->makeRoomCreated(addr, req, roomId);
        if (!out)
            return;

        (void) _tcp->sendPacket(*out);

        const auto updatePkt = _packetFactory->makeRoomUpdated(addr, req, _rooms->getRoomById(roomId)->getRoomData());
        if (!updatePkt)
            return;
        (void) _tcp->sendPacket(*updatePkt);
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
            if (const bool success = _rooms->addPlayerToRoom(roomId, sessionId); !success)
                return sendError(addr, req, 12, "JOIN_ROOM: cannot join room (full/invalid)");
        } catch (const std::exception &e) {
            return sendError(addr, req, 12, e.what());
        }

        if (!_packetFactory)
            return;

        const auto out = _packetFactory->makeRoomJoined(addr, req, roomId);
        if (!out)
            return;

        (void) _tcp->sendPacket(*out);
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

        (void) _tcp->sendPacket(*out);
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

        for (size_t i = 0; i < 3; i++) {
            for (const auto &session : room->sessions()) {
                if (const auto memberAddr = _sessions->getAddress(session)) {
                    if (const auto out = _packetFactory->makeGameStart(*memberAddr, 0, roomId))
                        (void) _tcp->sendPacket(*out);
                }
            }
        }
    }

    void TCPPacketRouter::onScoreboardGet(const sockaddr_in &addr, const uint32_t req, TCP::Reader &r) const
    {
        if (!_scores || !_packetFactory)
            return sendError(addr, req, 500, "SCOREBOARD_GET: service unavailable");

        uint16_t limit = 10;
        try {
            if (r.remaining() == 2)
                limit = r.u16();
            else if (r.remaining() != 0)
                return sendError(addr, req, 400, "SCOREBOARD_GET: malformed payload (optional limit u16)");
        } catch (...) {
            return sendError(addr, req, 400, "SCOREBOARD_GET: malformed payload");
        }

        if (limit == 0)
            limit = 10;
        if (limit > 100)
            limit = 100;

        std::vector<ScoreEntry> top;
        try {
            top = _scores->getTopScores(limit);
        } catch (const std::exception &e) {
            return sendError(addr, req, 500, std::string("SCOREBOARD_GET: db error: ") + e.what());
        }
        const auto out = _packetFactory->makeScoreboardList(addr, req, top);
        if (!out)
            return sendError(addr, req, 500, "SCOREBOARD_GET: build response failed");

        (void) _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onRoomInfo(
        const sockaddr_in &addr, const int sessionId, const uint32_t req, const TCP::Reader &r) const
    {
        uint32_t roomId = 0;

        try {
            roomId = _rooms->getRoomIdOfPlayer(sessionId);
        } catch (...) {
            return sendError(addr, req, 20, "ROOM_INFO: malformed payload (expected roomId(u32))");
        }

        if (r.remaining() != 0)
            return sendError(addr, req, 21, "ROOM_INFO: unexpected trailing bytes");

        const auto room = _rooms->getRoomById(roomId);
        if (!room)
            return sendError(addr, req, 22, "ROOM_INFO: room not found");

        if (!_packetFactory)
            return sendError(addr, req, 500, "ROOM_INFO: service unavailable");

        const auto out = _packetFactory->makeRoomUpdated(addr, req, room->getRoomData());
        if (!out)
            return sendError(addr, req, 22, "ROOM_INFO: room not updated");

        (void) _tcp->sendPacket(*out);
    }

    void TCPPacketRouter::onMessageRoom(
        const sockaddr_in &addr, const int sessionId, const uint32_t req, TCP::Reader &r) const
    {
        uint32_t roomId = 0;
        std::string message = "";

        try {
            roomId = _rooms->getRoomIdOfPlayer(sessionId);
            message = r.str16();
        } catch (...) {
            return sendError(addr, req, 30, "MESSAGE_ROOM: malformed payload (expected roomId(u32))");
        }
        if (r.remaining() != 0)
            return sendError(addr, req, 31, "MESSAGE_ROOM: unexpected trailing bytes");
        const auto room = _rooms->getRoomById(roomId);
        if (!room)
            return sendError(addr, req, 32, "MESSAGE_ROOM: room not found");

        for (const auto session : room->sessions()) {
            if (const auto memberAddr = _sessions->getAddress(session)) {
                auto username = _sessions->getUsername(sessionId);
                std::string fullMessage = username + ": " + message;
                if (const auto out = _packetFactory->makeRoomMessage(*memberAddr, req, fullMessage))
                    (void) _tcp->sendPacket(*out);
            }
        }
    }
} // namespace Net
