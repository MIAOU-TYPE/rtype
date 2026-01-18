/*
** EPITECH PROJECT, 2026
** RType
** File description:
** AdminConsole.cpp
*/

#include "AdminConsole.hpp"

namespace
{
    [[nodiscard]] std::optional<Engine::RoomId> readRoomId(
        std::istringstream &iss, const std::function<std::optional<Engine::RoomId>(const std::string &)> &parseRoomId)
    {
        std::string s;
        if (!(iss >> s))
            return std::nullopt;
        return parseRoomId(s);
    }

    [[nodiscard]] std::string toLower(std::string s)
    {
        std::ranges::transform(s, s.begin(), [](const unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        return s;
    }

#ifndef _WIN32
    [[nodiscard]] bool tryReadLineWithTimeout(std::string &out, const int timeoutMs)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeval tv{};
        tv.tv_sec = timeoutMs / 1000;
        tv.tv_usec = (timeoutMs % 1000) * 1000;
        if (const int r = select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv); r <= 0)
            return false;
        if (!FD_ISSET(STDIN_FILENO, &readfds))
            return false;
        return static_cast<bool>(std::getline(std::cin, out));
    }
#else
    [[nodiscard]] bool tryReadLineWithTimeout(std::string &out, int /*timeoutMs*/)
    {
        return static_cast<bool>(std::getline(std::cin, out));
    }
#endif
} // namespace

namespace Net::Admin
{
    AdminConsole::AdminConsole(std::shared_ptr<Server::ISessionManager> sessions,
        std::shared_ptr<Engine::RoomManager> rooms, ShutdownFn shutdownFn)
        : _sessions(std::move(sessions)), _rooms(std::move(rooms)), _shutdown(std::move(shutdownFn))
    {
    }

    AdminConsole::~AdminConsole()
    {
        stop();
    }

    void AdminConsole::start()
    {
        if (!_sessions || !_rooms)
            return;
        if (_running.exchange(true, std::memory_order_relaxed))
            return;
        _thread = std::thread(&AdminConsole::run, this);
    }

    void AdminConsole::stop()
    {
        _running.store(false, std::memory_order_relaxed);
        if (_thread.joinable() && std::this_thread::get_id() != _thread.get_id())
            _thread.join();
    }

    bool AdminConsole::isAllDigits(const std::string &s)
    {
        if (s.empty())
            return false;
        return std::ranges::all_of(s, [](unsigned char c) {
            return std::isdigit(c) != 0;
        });
    }

    std::optional<int> AdminConsole::resolveSessionId(const std::string &who) const
    {
        if (who.empty())
            return std::nullopt;

        if (isAllDigits(who)) {
            try {
                return std::stoi(who);
            } catch (...) {
                return std::nullopt;
            }
        }
        return _sessions ? _sessions->findSessionIdByUsername(who) : std::nullopt;
    }

    std::optional<Engine::RoomId> AdminConsole::parseRoomId(const std::string &s) const
    {
        if (!isAllDigits(s))
            return std::nullopt;
        try {
            const auto v = std::stoul(s);
            if (v > 0xFFFFFFFFu)
                return std::nullopt;
            return static_cast<Engine::RoomId>(v);
        } catch (...) {
            return std::nullopt;
        }
    }

    std::optional<std::string> AdminConsole::resolveUsername(const std::string &who) const
    {
        if (who.empty())
            return std::nullopt;

        if (isAllDigits(who)) {
            int sid = -1;
            try {
                sid = std::stoi(who);
            } catch (...) {
                return std::nullopt;
            }
            if (sid < 0)
                return std::nullopt;

            const auto name = _sessions ? _sessions->getUsername(sid) : std::string();
            if (name.empty())
                return std::nullopt;
            return name;
        }

        return who;
    }

    void AdminConsole::printHelp()
    {
        std::cout << "Admin commands:\n"
                  << "  help\n"
                  << "  status\n"
                  << "  rooms\n"
                  << "  sessions\n"
                  << "  kickroom <roomId> <sessionId|username>\n"
                  << "  banroom <roomId> <sessionId|username>\n"
                  << "  unbanroom <roomId> <sessionId|username>\n"
                  << "  bans\n"
                  << "  shutdown\n";
    }

    std::string AdminConsole::ipToString(const uint32_t ipNbo)
    {
        in_addr addr{};
        addr.s_addr = ipNbo;
        char buf[INET_ADDRSTRLEN] = {0};
        const char *res = inet_ntop(AF_INET, &addr, buf, sizeof(buf));
        return res ? std::string(res) : std::string("?");
    }

    bool AdminConsole::parseIPv4(const std::string &s, uint32_t &outIpNbo)
    {
        in_addr addr{};
        if (inet_pton(AF_INET, s.c_str(), &addr) != 1)
            return false;
        outIpNbo = addr.s_addr;
        return true;
    }

    void AdminConsole::cmdStatus() const
    {
        const auto rooms = _rooms->listRooms();
        const auto sessions = _sessions->getAllSessions();
        std::cout << "rooms=" << rooms.size() << " sessions=" << sessions.size()
                  << " bans=" << _sessions->listBans().size() << "\n";
    }

    void AdminConsole::cmdRooms() const
    {
        const auto rooms = _rooms->listRooms();
        if (rooms.empty()) {
            std::cout << "no rooms\n";
            return;
        }
        for (const auto &r : rooms) {
            std::cout << "roomId=" << r.roomId << " name=\"" << r.roomName << "\" players=" << r.currentPlayers << "/"
                      << r.maxPlayers << "\n";
        }
    }

    void AdminConsole::cmdSessions() const
    {
        const auto sessions = _sessions->getAllSessions();
        if (sessions.empty()) {
            std::cout << "no sessions\n";
            return;
        }
        for (const auto &[id, tcp] : sessions) {
            const auto *udp = _sessions->getUdpAddress(id);
            const auto roomId = _rooms->getRoomIdOfPlayer(id);
            const auto username = _sessions->getUsername(id);
            std::cout << "sid=" << id << " tcp=" << ipToString(tcp.sin_addr.s_addr) << ":" << ntohs(tcp.sin_port);
            if (udp)
                std::cout << " udp=" << ipToString(udp->sin_addr.s_addr) << ":" << ntohs(udp->sin_port);
            else
                std::cout << " udp=-";
            std::cout << " authed=" << (_sessions->isAuthed(id) ? "1" : "0") << " user=\"" << username << "\""
                      << " roomId=" << roomId << "\n";
        }
    }

    bool AdminConsole::cmdKickRoom(const Engine::RoomId roomId, const std::string &who) const
    {
        const auto room = _rooms->getRoomById(roomId);
        if (!room) {
            std::cout << "not found\n";
            return false;
        }
        const auto sidOpt = resolveSessionId(who);
        if (!sidOpt.has_value()) {
            std::cout << "not found\n";
            return false;
        }
        const int sessionId = *sidOpt;
        if (_rooms->getRoomIdOfPlayer(sessionId) != roomId) {
            std::cout << "not found\n";
            return false;
        }
        (void) _rooms->removePlayer(sessionId);
        std::cout << "room-kicked sid=" << sessionId << " roomId=" << roomId << "\n";
        return true;
    }

    bool AdminConsole::cmdBanRoom(const Engine::RoomId roomId, const std::string &who) const
    {
        const auto room = _rooms->getRoomById(roomId);
        if (!room) {
            std::cout << "not found\n";
            return false;
        }
        const auto usernameOpt = resolveUsername(who);
        if (!usernameOpt.has_value()) {
            std::cout << "not found\n";
            return false;
        }
        const auto &username = *usernameOpt;
        room->banUsername(username);
        if (const auto sidOpt = _sessions->findSessionIdByUsername(username); sidOpt.has_value()) {
            const int sid = *sidOpt;
            if (_rooms->getRoomIdOfPlayer(sid) == roomId)
                (void) _rooms->removePlayer(sid);
        }
        std::cout << "room-banned user=\"" << username << "\" roomId=" << roomId << "\n";
        return true;
    }

    bool AdminConsole::cmdUnbanRoom(const Engine::RoomId roomId, const std::string &who) const
    {
        const auto room = _rooms->getRoomById(roomId);
        if (!room) {
            std::cout << "not found\n";
            return false;
        }
        const auto usernameOpt = resolveUsername(who);
        if (!usernameOpt.has_value()) {
            std::cout << "not found\n";
            return false;
        }
        const auto &username = *usernameOpt;
        room->unbanUsername(username);
        std::cout << "room-unbanned user=\"" << username << "\" roomId=" << roomId << "\n";
        return true;
    }

    void AdminConsole::run()
    {
        printHelp();
        const auto usage = [](const char *u) {
            std::cout << u << "\n";
        };

        const std::unordered_map<std::string, Handler> handlers = {
            {"help",
                [&](std::istringstream &) {
                    printHelp();
                }},
            {"status",
                [&](std::istringstream &) {
                    cmdStatus();
                }},
            {"rooms",
                [&](std::istringstream &) {
                    cmdRooms();
                }},
            {"sessions",
                [&](std::istringstream &) {
                    cmdSessions();
                }},
            {"shutdown",
                [&](std::istringstream &) {
                    if (_shutdown)
                        _shutdown();
                }},
            {"kickroom",
                [&](std::istringstream &iss) {
                    const auto rid = readRoomId(iss, [&](const std::string &s) {
                        return parseRoomId(s);
                    });
                    std::string who;
                    if (!rid || !(iss >> who))
                        return usage("usage: kickroom <roomId> <sessionId|username>");
                    (void) cmdKickRoom(*rid, who);
                }},
            {"banroom",
                [&](std::istringstream &iss) {
                    const auto rid = readRoomId(iss, [&](const std::string &s) {
                        return parseRoomId(s);
                    });
                    std::string who;
                    if (!rid || !(iss >> who))
                        return usage("usage: banroom <roomId> <sessionId|username>");
                    (void) cmdBanRoom(*rid, who);
                }},
            {"unbanroom",
                [&](std::istringstream &iss) {
                    const auto rid = readRoomId(iss, [&](const std::string &s) {
                        return parseRoomId(s);
                    });
                    std::string who;
                    if (!rid || !(iss >> who))
                        return usage("usage: unbanroom <roomId> <sessionId|username>");
                    (void) cmdUnbanRoom(*rid, who);
                }},
        };

        while (_running.load(std::memory_order_relaxed)) {
            std::string line;
            if (!tryReadLineWithTimeout(line, 50))
                continue;
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
                line.pop_back();
            if (line.empty())
                continue;
            std::istringstream iss(line);
            std::string cmd;
            if (!(iss >> cmd))
                continue;
            cmd = toLower(cmd);
            if (const auto it = handlers.find(cmd); it != handlers.end())
                it->second(iss);
            else
                std::cout << "unknown command\n";
        }
    }
} // namespace Net::Admin