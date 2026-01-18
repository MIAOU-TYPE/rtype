/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AdminWebServer.cpp
*/

#include "AdminWebServer.hpp"

namespace
{
    using json = nlohmann::json;

    static std::string ipToString(uint32_t ipNbo)
    {
        char buf[64] = {};
        in_addr a{};
        a.s_addr = ipNbo;
#ifdef _WIN32
        InetNtopA(AF_INET, &a, buf, static_cast<DWORD>(sizeof(buf)));
#else
        inet_ntop(AF_INET, &a, buf, sizeof(buf));
#endif
        return std::string(buf);
    }

    static std::optional<int> parseSessionId(const std::string &s)
    {
        if (s.empty())
            return std::nullopt;
        if (!std::all_of(s.begin(), s.end(), [](unsigned char c) {
                return std::isdigit(c) != 0;
            }))
            return std::nullopt;
        try {
            return std::stoi(s);
        } catch (...) {
            return std::nullopt;
        }
    }

    static bool hasBearerToken(const httplib::Request &req, const std::string &token)
    {
        const auto it = req.headers.find("Authorization");
        if (it == req.headers.end())
            return false;

        const std::string &v = it->second;
        static constexpr auto kPrefix = "Bearer ";
        constexpr auto prefixLen = std::char_traits<char>::length(kPrefix);

        if (v.size() <= prefixLen)
            return false;
        if (v.rfind(kPrefix, 0) != 0)
            return false;
        return v.substr(prefixLen) == token;
    }

    static void setJson(httplib::Response &res, const json &j, int status = 200)
    {
        res.status = status;
        res.set_content(j.dump(2), "application/json; charset=utf-8");
    }

    static void setText(httplib::Response &res, int status, const char *msg)
    {
        res.status = status;
        res.set_content(msg, "text/plain; charset=utf-8");
    }

    static std::optional<json> parseJsonBody(const httplib::Request &req, httplib::Response &res)
    {
        try {
            return json::parse(req.body.empty() ? "{}" : req.body);
        } catch (...) {
            setText(res, 400, "Bad JSON");
            return std::nullopt;
        }
    }

    template <typename T>
    static bool requirePtr(const std::shared_ptr<T> &ptr, httplib::Response &res)
    {
        if (ptr)
            return true;
        setText(res, 500, "Service unavailable");
        return false;
    }

    static auto *kIndexHtml = R"HTML(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1" />
  <title>R-Type Admin</title>
  <link rel="stylesheet" href="/style.css" />
</head>
<body>
  <div class="wrap">
    <h1>R-Type Admin</h1>

    <div class="row">
      <label>Token</label>
      <input id="token" type="password" placeholder="admin" />
      <button onclick="refreshAll()">Refresh</button>
    </div>

    <h2>Status</h2>
    <pre id="status">{}</pre>

    <h2>Rooms</h2>
    <table><tbody id="rooms"></tbody></table>

    <h2>Sessions</h2>
    <table><tbody id="sessions"></tbody></table>

    <h2>Room moderation</h2>
    <div class="row">
      <label>RoomId</label>
      <input id="rm_roomId" placeholder="123" />
      <label>Username</label>
      <input id="rm_username" placeholder="player" />
      <button onclick="banRoomManual()">Ban</button>
      <button onclick="unbanRoomManual()">Unban</button>
      <button onclick="kickRoomManual()">Kick</button>
    </div>

    <div class="row">
      <button onclick="shutdownServer()">Shutdown server</button>
    </div>
  </div>

  <script src="/app.js"></script>
</body>
</html>
)HTML";

    static auto kStyleCss = R"CSS(
body { font-family: sans-serif; background:#0b0f14; color:#eaeef5; margin:0; }
.wrap { max-width: 1100px; margin: 24px auto; padding: 0 16px; }
h1 { margin: 8px 0 20px; }
h2 { margin-top: 24px; }
.row { display:flex; gap: 10px; align-items:center; margin: 10px 0 14px; flex-wrap: wrap; }
input { padding: 8px 10px; width: 260px; }
button { padding: 8px 12px; cursor:pointer; }
pre { background:#121a24; padding: 12px; border-radius: 8px; overflow:auto; }
table { width:100%; border-collapse: collapse; background:#121a24; border-radius: 8px; overflow:hidden; }
th, td { padding: 10px; border-bottom: 1px solid #233247; vertical-align: middle; }
th { text-align:left; color:#b9c4d6; }
tr:last-child td { border-bottom: none; }
td button { padding: 6px 10px; }
.small { width: 140px; }
)CSS";

    static auto kAppJs = R"JS(
function authHeaders() {
  const t = document.getElementById("token").value.trim();
  return { "Authorization": "Bearer " + t };
}

async function apiGet(path) {
  const r = await fetch(path, { headers: authHeaders() });
  if (!r.ok) throw new Error(await r.text());
  return r.json();
}

async function apiPost(path, body) {
  const r = await fetch(path, {
    method: "POST",
    headers: { ...authHeaders(), "Content-Type": "application/json" },
    body: JSON.stringify(body || {})
  });
  if (!r.ok) throw new Error(await r.text());
  return r.json();
}

function renderTable(el, cols, rows) {
  const t = document.getElementById(el);
  t.innerHTML = "";
  const thead = document.createElement("tr");
  cols.forEach(c => { const th = document.createElement("th"); th.textContent = c; thead.appendChild(th); });
  t.appendChild(thead);

  rows.forEach(r => {
    const tr = document.createElement("tr");
    cols.forEach(c => {
      const td = document.createElement("td");
      td.appendChild(r[c] instanceof Node ? r[c] : document.createTextNode(String(r[c] ?? "")));
      tr.appendChild(td);
    });
    t.appendChild(tr);
  });
}

function vRoomId() {
  const s = document.getElementById("rm_roomId").value.trim();
  const n = Number(s);
  if (!Number.isFinite(n) || n <= 0) return null;
  return n;
}

function vUsername() {
  return document.getElementById("rm_username").value.trim();
}

async function banRoomManual() {
  const roomId = vRoomId();
  const username = vUsername();
  if (!roomId || !username) return alert("roomId + username required");
  await apiPost("/api/banroom", { roomId, who: username });
  await refreshAll();
}

async function unbanRoomManual() {
  const roomId = vRoomId();
  const username = vUsername();
  if (!roomId || !username) return alert("roomId + username required");
  await apiPost("/api/unbanroom", { roomId, who: username });
  await refreshAll();
}

async function kickRoomManual() {
  const roomId = vRoomId();
  const username = vUsername();
  if (!roomId || !username) return alert("roomId + username required");
  await apiPost("/api/kickroom", { roomId, who: username });
  await refreshAll();
}

async function shutdownServer() {
  await apiPost("/api/shutdown", {});
}

async function refreshAll() {
  try {
    const status = await apiGet("/api/status");
    document.getElementById("status").textContent = JSON.stringify(status, null, 2);

    const rooms = await apiGet("/api/rooms");
    renderTable("rooms", ["roomId","name","players","maxPlayers","playerNames"], rooms);

    const sessions = await apiGet("/api/sessions");
    const rows = sessions.map(s => {
      const kick = document.createElement("button");
      kick.textContent = "Kick (server)";
      kick.onclick = async () => { await apiPost("/api/kick", { who: String(s.id) }); await refreshAll(); };

      const kickRoom = document.createElement("button");
      kickRoom.textContent = "KickRoom";
      kickRoom.disabled = (s.roomId === null || s.roomId === undefined);
      kickRoom.onclick = async () => { await apiPost("/api/kickroom", { roomId: s.roomId, who: String(s.id) }); await refreshAll(); };

      const banRoom = document.createElement("button");
      banRoom.textContent = "BanRoom";
      banRoom.disabled = (s.roomId === null || s.roomId === undefined);
      banRoom.onclick = async () => { await apiPost("/api/banroom", { roomId: s.roomId, who: String(s.id) }); await refreshAll(); };

      return {
        id: s.id,
        username: s.username,
        authed: s.authed,
        ip: s.ip,
        port: s.port,
        roomId: s.roomId,
        kick: kick,
        kickRoom: kickRoom,
        banRoom: banRoom
      };
    });
    renderTable("sessions", ["id","username","authed","ip","port","roomId","kickRoom","banRoom","kick"], rows);

  } catch (e) {
    alert("Error: " + e.message);
  }
}

window.addEventListener("load", () => {
  const token = document.getElementById("token");
  if (token && !token.value) token.value = "admin";
  refreshAll();
});
)JS";

    static std::optional<int> resolveWhoToSessionId(
        const std::shared_ptr<Net::Server::ISessionManager> &sessions, const std::string &who)
    {
        if (!sessions)
            return std::nullopt;
        if (const auto sid = parseSessionId(who); sid.has_value())
            return sid;
        return sessions->findSessionIdByUsername(who);
    }

    static std::optional<std::string> resolveWhoToUsername(
        const std::shared_ptr<Net::Server::ISessionManager> &sessions, const std::string &who)
    {
        if (!sessions)
            return std::nullopt;
        if (const auto sid = parseSessionId(who); sid.has_value()) {
            const std::string u = sessions->getUsername(*sid);
            if (u.empty())
                return std::nullopt;
            return u;
        }
        if (who.empty())
            return std::nullopt;
        return who;
    }

    static void handleStatus(const std::shared_ptr<Net::Server::ISessionManager> &sessions,
        const std::shared_ptr<Engine::RoomManager> &rooms, httplib::Response &res)
    {
        json out;
        out["ok"] = true;
        out["sessions"] = sessions ? sessions->getAllSessions().size() : 0u;
        out["rooms"] = rooms ? rooms->listRooms().size() : 0u;
        setJson(res, out, 200);
    }

    static void handleRooms(const std::shared_ptr<Engine::RoomManager> &rooms, httplib::Response &res)
    {
        if (!rooms) {
            setJson(res, json::array(), 200);
            return;
        }

        json arr = json::array();
        for (const auto &rd : rooms->listRooms()) {
            json r;
            r["roomId"] = rd.roomId;
            r["name"] = rd.roomName;
            if constexpr (requires { rd.currentPlayers; })
                r["players"] = rd.currentPlayers;
            else
                r["players"] = rd.playerNames.size();
            r["maxPlayers"] = rd.maxPlayers;
            r["playerNames"] = rd.playerNames;
            arr.push_back(std::move(r));
        }
        setJson(res, arr, 200);
    }

    static void handleSessions(const std::shared_ptr<Net::Server::ISessionManager> &sessions,
        const std::shared_ptr<Engine::RoomManager> &rooms, httplib::Response &res)
    {
        if (!sessions || !rooms) {
            setJson(res, json::array(), 200);
            return;
        }

        json arr = json::array();
        for (const auto &[sid, addr] : sessions->getAllSessions()) {
            json s;
            s["id"] = sid;
            s["username"] = sessions->getUsername(sid);
            s["authed"] = sessions->isAuthed(sid);

            s["ip"] = ipToString(addr.sin_addr.s_addr);
            s["port"] = ntohs(addr.sin_port);

            const auto roomId = rooms->getRoomIdOfPlayer(sid);
            s["roomId"] = roomId == 0 ? json(nullptr) : json(roomId);

            arr.push_back(std::move(s));
        }
        setJson(res, arr, 200);
    }

    static void handleKick(const std::shared_ptr<Net::Server::ISessionManager> &sessions,
        const std::shared_ptr<Engine::RoomManager> &rooms, const httplib::Request &req, httplib::Response &res)
    {
        const auto inOpt = parseJsonBody(req, res);
        if (!inOpt)
            return;
        const json &in = *inOpt;

        if (!requirePtr(sessions, res) || !requirePtr(rooms, res))
            return;

        const std::string who = in.value("who", "");
        const auto sidOpt = resolveWhoToSessionId(sessions, who);
        if (!sidOpt.has_value()) {
            setText(res, 404, "Unknown session/user");
            return;
        }

        const int sid = *sidOpt;
        (void) rooms->removePlayer(sid);
        sessions->removeSession(sid);

        setJson(res, json{{"ok", true}}, 200);
    }

    static void handleKickRoom(const std::shared_ptr<Net::Server::ISessionManager> &sessions,
        const std::shared_ptr<Engine::RoomManager> &rooms, const httplib::Request &req, httplib::Response &res)
    {
        const auto inOpt = parseJsonBody(req, res);
        if (!inOpt)
            return;
        const json &in = *inOpt;

        if (!requirePtr(sessions, res) || !requirePtr(rooms, res))
            return;

        const uint32_t roomId = in.value("roomId", 0u);
        const std::string who = in.value("who", "");
        if (roomId == 0 || who.empty()) {
            setText(res, 400, "Expected roomId + who");
            return;
        }

        const auto sidOpt = resolveWhoToSessionId(sessions, who);
        if (!sidOpt.has_value()) {
            setText(res, 404, "Unknown session/user");
            return;
        }

        const int sid = *sidOpt;
        if (rooms->getRoomIdOfPlayer(sid) != roomId) {
            setText(res, 404, "Player not in that room");
            return;
        }

        (void) rooms->removePlayer(sid);
        setJson(res, json{{"ok", true}}, 200);
    }

    static void handleBanRoom(const std::shared_ptr<Net::Server::ISessionManager> &sessions,
        const std::shared_ptr<Engine::RoomManager> &rooms, const httplib::Request &req, httplib::Response &res)
    {
        const auto inOpt = parseJsonBody(req, res);
        if (!inOpt)
            return;
        const json &in = *inOpt;
        if (!requirePtr(sessions, res) || !requirePtr(rooms, res))
            return;
        const uint32_t roomId = in.value("roomId", 0u);
        const std::string who = in.value("who", "");
        if (roomId == 0 || who.empty()) {
            setText(res, 400, "Expected roomId + who");
            return;
        }
        const auto room = rooms->getRoomById(roomId);
        if (!room) {
            setText(res, 404, "Room not found");
            return;
        }
        const auto usernameOpt = resolveWhoToUsername(sessions, who);
        if (!usernameOpt.has_value()) {
            setText(res, 404, "Unknown username");
            return;
        }
        const std::string &username = *usernameOpt;
        room->banUsername(username);
        if (const auto sidOpt = sessions->findSessionIdByUsername(username); sidOpt.has_value()) {
            const int sid = *sidOpt;
            if (rooms->getRoomIdOfPlayer(sid) == roomId)
                (void) rooms->removePlayer(sid);
        }

        setJson(res, json{{"ok", true}}, 200);
    }

    static void handleUnbanRoom(std::shared_ptr<Net::Server::ISessionManager> sessions,
        std::shared_ptr<Engine::RoomManager> rooms, const httplib::Request &req, httplib::Response &res)
    {
        const auto inOpt = parseJsonBody(req, res);
        if (!inOpt)
            return;
        const json &in = *inOpt;
        if (!requirePtr(sessions, res) || !requirePtr(rooms, res))
            return;
        const uint32_t roomId = in.value("roomId", 0u);
        const std::string who = in.value("who", "");
        if (roomId == 0 || who.empty()) {
            setText(res, 400, "Expected roomId + who");
            return;
        }
        const auto room = rooms->getRoomById(roomId);
        if (!room) {
            setText(res, 404, "Room not found");
            return;
        }
        const auto usernameOpt = resolveWhoToUsername(sessions, who);
        if (!usernameOpt.has_value()) {
            setText(res, 404, "Unknown username");
            return;
        }
        room->unbanUsername(*usernameOpt);
        setJson(res, json{{"ok", true}}, 200);
    }

    static void handleShutdown(std::function<void()> onShutdown, httplib::Response &res)
    {
        if (onShutdown)
            onShutdown();
        setJson(res, json{{"ok", true}}, 200);
    }

} // namespace

namespace Net::Admin
{
    AdminWebServer::AdminWebServer(std::shared_ptr<Net::Server::ISessionManager> sessions,
        std::shared_ptr<Engine::RoomManager> rooms, std::function<void()> onShutdown, int port, std::string token)
        : _sessions(std::move(sessions)), _rooms(std::move(rooms)), _onShutdown(std::move(onShutdown)), _port(port),
          _token(std::move(token))
    {
    }

    AdminWebServer::~AdminWebServer()
    {
        stop();
    }

    void AdminWebServer::start()
    {
        if (bool expected = false; !_running.compare_exchange_strong(expected, true))
            return;
        _thread = std::thread(&AdminWebServer::run, this);
    }

    void AdminWebServer::stop() noexcept
    {
        _running.store(false, std::memory_order_relaxed);
        if (_srv)
            _srv->stop();
        if (_thread.joinable() && std::this_thread::get_id() != _thread.get_id())
            _thread.join();
    }

    int AdminWebServer::port() const noexcept
    {
        return _port;
    }

    void AdminWebServer::run()
    {
        _srv = std::make_unique<httplib::Server>();
        auto &srv = *_srv;
        srv.Get("/", [](const httplib::Request &, httplib::Response &res) {
            res.set_content(kIndexHtml, "text/html; charset=utf-8");
        });
        srv.Get("/style.css", [](const httplib::Request &, httplib::Response &res) {
            res.set_content(kStyleCss, "text/css; charset=utf-8");
        });
        srv.Get("/app.js", [](const httplib::Request &, httplib::Response &res) {
            res.set_content(kAppJs, "application/javascript; charset=utf-8");
        });
        auto requireAuth = [&](const httplib::Request &req, httplib::Response &res) -> bool {
            if (!hasBearerToken(req, _token)) {
                res.status = 401;
                res.set_content("Unauthorized", "text/plain; charset=utf-8");
                return false;
            }
            return true;
        };

        srv.Get("/api/status", [&](const httplib::Request &req, httplib::Response &res) {
            if (!requireAuth(req, res))
                return;
            handleStatus(_sessions, _rooms, res);
        });

        srv.Get("/api/rooms", [&](const httplib::Request &req, httplib::Response &res) {
            if (!requireAuth(req, res))
                return;
            handleRooms(_rooms, res);
        });

        srv.Get("/api/sessions", [&](const httplib::Request &req, httplib::Response &res) {
            if (!requireAuth(req, res))
                return;
            handleSessions(_sessions, _rooms, res);
        });

        srv.Post("/api/kick", [&](const httplib::Request &req, httplib::Response &res) {
            if (!requireAuth(req, res))
                return;
            handleKick(_sessions, _rooms, req, res);
        });

        srv.Post("/api/kickroom", [&](const httplib::Request &req, httplib::Response &res) {
            if (!requireAuth(req, res))
                return;
            handleKickRoom(_sessions, _rooms, req, res);
        });

        srv.Post("/api/banroom", [&](const httplib::Request &req, httplib::Response &res) {
            if (!requireAuth(req, res))
                return;
            handleBanRoom(_sessions, _rooms, req, res);
        });

        srv.Post("/api/unbanroom", [&](const httplib::Request &req, httplib::Response &res) {
            if (!requireAuth(req, res))
                return;
            handleUnbanRoom(_sessions, _rooms, req, res);
        });

        srv.Post("/api/shutdown", [&](const httplib::Request &req, httplib::Response &res) {
            if (!requireAuth(req, res))
                return;
            handleShutdown(_onShutdown, res);
        });

        srv.listen("127.0.0.1", _port);
        _srv.reset();
    }
} // namespace Net::Admin
