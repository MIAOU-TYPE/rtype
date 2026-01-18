/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** testRoom
*/

#include <gtest/gtest.h>

#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <string_view>
#include <unordered_map>

#include "Room.hpp"

#include "IPacket.hpp"
#include "IServer.hpp"
#include "ISessionManager.hpp"
#include "UDPPacketFactory.hpp"

#include "GameConstants.hpp"

namespace
{
    static sockaddr_in makeAddr(uint16_t port = 4242)
    {
        sockaddr_in a{};
        a.sin_family = AF_INET;
        a.sin_addr.s_addr = htonl(0x7F000001u);
        a.sin_port = htons(port);
        return a;
    }

    class FakePacket final : public Net::IPacket {
      public:
        explicit FakePacket(size_t cap = 2048) : _capacity(cap), _size(0), _buffer(cap)
        {
        }

        std::shared_ptr<Net::IPacket> newPacket() const override
        {
            return std::make_shared<FakePacket>(_capacity);
        }

        std::shared_ptr<Net::IPacket> clone() const override
        {
            auto p = std::make_shared<FakePacket>(_capacity);
            if (_hasAddr)
                p->setAddress(_addr);
            p->setSize(_size);
            if (_size)
                std::memcpy(p->buffer(), _buffer.data(), _size);
            return p;
        }

        uint8_t *buffer() override
        {
            return _buffer.data();
        }

        const uint8_t *buffer() const override
        {
            return _buffer.data();
        }

        size_t size() const override
        {
            return _size;
        }

        void setSize(size_t s) override
        {
            _size = s;
        }

        const sockaddr_in *address() const override
        {
            return _hasAddr ? &_addr : nullptr;
        }

        void setAddress(const sockaddr_in &addr) override
        {
            _addr = addr;
            _hasAddr = true;
        }

        size_t capacity() const noexcept override
        {
            return _capacity;
        }

      private:
        sockaddr_in _addr{};
        bool _hasAddr = false;
        size_t _capacity = 0;
        size_t _size = 0;
        std::vector<uint8_t> _buffer;
    };

    class FakeUdpServer final : public Net::Server::IServer {
      public:
        void configure(const std::string &, int32_t) override
        {
        }

        void start() override
        {
        }

        void stop() noexcept override
        {
        }

        void setNonBlocking(bool) override
        {
        }

        [[nodiscard]] bool isRunning() const noexcept override
        {
            return true;
        }

        void setRunning(bool) noexcept override
        {
        }

        [[nodiscard]] bool readPackets() noexcept override
        {
            return true;
        }

        [[nodiscard]] bool isStoredIpCorrect() const noexcept override
        {
            return true;
        }

        [[nodiscard]] bool isStoredPortCorrect() const noexcept override
        {
            return true;
        }

        [[nodiscard]] bool popPacket(std::shared_ptr<Net::IPacket> &) noexcept override
        {
            return false;
        }

        int getPort() const noexcept override
        {
            return 5000;
        }

        bool sendPacket(const Net::IPacket &) noexcept override
        {
            sendCalls++;
            return true;
        }

        int sendCalls = 0;
    };

    class FakeSessions final : public Net::Server::ISessionManager {
      public:
        int getOrCreateSession(const sockaddr_in &addr) override
        {
            lastAddr = addr;
            return 0;
        }

        int getSessionId(const sockaddr_in &) const override
        {
            return -1;
        }

        void removeSession(int) override
        {
        }

        const sockaddr_in *getAddress(int) const override
        {
            return nullptr;
        }

        std::vector<std::pair<int, sockaddr_in>> getAllSessions() const override
        {
            return {};
        }

        void forEachSession(const std::function<void(int, const sockaddr_in &)> &) const override
        {
        }

        void setUdpToken(int sessionId, uint64_t token) override
        {
            udpTokens[sessionId] = token;
        }

        uint64_t getUdpToken(int sessionId) const override
        {
            const auto it = udpTokens.find(sessionId);
            return (it == udpTokens.end()) ? 0 : it->second;
        }

        bool bindUdp(int, const sockaddr_in &) override
        {
            return true;
        }

        const sockaddr_in *getUdpAddress(int sessionId) const override
        {
            const auto it = udpAddrs.find(sessionId);
            if (it == udpAddrs.end())
                return nullptr;
            return &it->second;
        }

        int getSessionIdFromUdp(const sockaddr_in &) const override
        {
            return -1;
        }

        bool isSequenceValid(int, uint32_t) const noexcept override
        {
            return true;
        }

        std::optional<Auth::Identity> getIdentity(int) const override
        {
            return std::nullopt;
        }

        void setIdentity(int, Auth::Identity, std::chrono::seconds) override
        {
        }

        void clearIdentity(int) override
        {
        }

        bool isAuthed(int) const override
        {
            return true;
        }

        bool consumeUdp(const sockaddr_in &) override
        {
            return true;
        }

        void setLastScore(int sessionId, uint32_t score) override
        {
            lastScores[sessionId] = score;
        }

        std::optional<uint32_t> getLastScore(int sessionId) const override
        {
            const auto it = lastScores.find(sessionId);
            if (it == lastScores.end())
                return std::nullopt;
            return it->second;
        }

        std::string getUsername(int sessionId) const override
        {
            const auto it = usernames.find(sessionId);
            return (it == usernames.end()) ? std::string{} : it->second;
        }

        sockaddr_in lastAddr{};
        std::unordered_map<int, uint64_t> udpTokens;
        std::unordered_map<int, sockaddr_in> udpAddrs;
        std::unordered_map<int, uint32_t> lastScores;
        std::unordered_map<int, std::string> usernames;
    };

    static Engine::GameConfig makeConfig()
    {
        Engine::GameConfig cfg{};
        cfg.difficulty = Engine::Difficulty::Easy;
        cfg.mode = Engine::GameMode::Standard;
        cfg.parameters = Engine::ModeParameters{};
        cfg.levelId = "levels/world1/levels.json";
        return cfg;
    }

    static std::shared_ptr<Engine::Room> makeRoom(std::shared_ptr<FakeSessions> sessions)
    {
        auto udpServer = std::make_shared<FakeUdpServer>();
        auto proto = std::make_shared<FakePacket>(2048);
        auto udpFactory = std::make_shared<Net::Factory::UDPPacketFactory>(proto);

        const std::string levelPath = "levels/world1/levels.json";

        auto cfg = makeConfig();
        return std::make_shared<Engine::Room>(sessions, udpServer, udpFactory, levelPath, cfg, "testRoom", 4);
    }

} // namespace

TEST(Room, StartsEmpty)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto room = makeRoom(sessions);

    EXPECT_TRUE(room->empty());
    EXPECT_EQ(room->getCurrentPlayers(), 0u);

    const auto data = room->getRoomData();
    EXPECT_EQ(data.currentPlayers, 0u);
    EXPECT_EQ(data.maxPlayers, 4u);
    EXPECT_EQ(data.roomName, "testRoom");
}

TEST(Room, JoinAddsSessionAndUsername)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto room = makeRoom(sessions);

    room->join(10, "alice");

    EXPECT_FALSE(room->empty());
    EXPECT_EQ(room->getCurrentPlayers(), 1u);
    EXPECT_TRUE(room->sessions().contains(10));

    const auto data = room->getRoomData();
    EXPECT_EQ(data.currentPlayers, 1u);
    ASSERT_EQ(data.playerNames.size(), 1u);
    EXPECT_EQ(data.playerNames[0], "alice");
}

TEST(Room, JoinSameSessionThrows)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto room = makeRoom(sessions);

    room->join(10, "alice");
    EXPECT_THROW(room->join(10, "alice"), Engine::RoomError);
}

TEST(Room, LeaveRemovesSessionAndUsername)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto room = makeRoom(sessions);

    room->join(10, "alice");
    room->join(11, "bob");

    room->leave(10, "alice");

    EXPECT_FALSE(room->sessions().contains(10));
    EXPECT_TRUE(room->sessions().contains(11));
    EXPECT_EQ(room->getCurrentPlayers(), 1u);

    const auto data = room->getRoomData();
    EXPECT_EQ(data.currentPlayers, 1u);
    ASSERT_EQ(data.playerNames.size(), 1u);
    EXPECT_EQ(data.playerNames[0], "bob");
}

TEST(Room, LeaveNonExistingSessionIsNoopOnSetAndNames)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto room = makeRoom(sessions);

    room->join(10, "alice");

    room->leave(999, "nobody");

    EXPECT_TRUE(room->sessions().contains(10));
    const auto data = room->getRoomData();
    ASSERT_EQ(data.playerNames.size(), 1u);
    EXPECT_EQ(data.playerNames[0], "alice");
}

TEST(Room, StartStopAreIdempotent)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto room = makeRoom(sessions);

    room->start();
    room->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(30));

    room->stop();
    room->stop();

    SUCCEED();
}
