/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** testUDPPacketRouter
*/

#include <gtest/gtest.h>

#include <arpa/inet.h>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <string_view>
#include <unordered_map>

#include "UDPPacketRouter.hpp"

#include "ConnectData.hpp"
#include "IPacket.hpp"
#include "IServer.hpp"
#include "ISessionManager.hpp"
#include "InputData.hpp"
#include "RoomManager.hpp"
#include "UDPPacketFactory.hpp"
#include "UDPTypesData.hpp"

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

    template <class T>
    static std::vector<uint8_t> toBytes(const T &obj)
    {
        std::vector<uint8_t> out(sizeof(T));
        std::memcpy(out.data(), &obj, sizeof(T));
        return out;
    }

    static void fillHeader(HeaderData &h, uint8_t type, uint16_t totalSize, uint32_t seq)
    {
        std::memcpy(h.magic, kPacketMagic, 4);
        h.version = 1;
        h.type = type;
        h.size = htons(totalSize);
        h.sequence = htonl(seq);
    }

    class FakePacket final : public Net::IPacket {
      public:
        FakePacket(sockaddr_in addr, const std::vector<uint8_t> &bytes, size_t cap = 2048)
            : _addr(addr), _capacity(cap), _size(bytes.size()), _buffer(cap)
        {
            if (bytes.size() > cap)
                throw std::runtime_error("FakePacket: bytes > cap");
            if (!bytes.empty())
                std::memcpy(_buffer.data(), bytes.data(), bytes.size());
        }

        explicit FakePacket(size_t cap = 2048) : _capacity(cap), _size(0), _buffer(cap)
        {
        }

        std::shared_ptr<Net::IPacket> newPacket() const override
        {
            return std::make_shared<FakePacket>(_capacity);
        }

        std::shared_ptr<Net::IPacket> clone() const override
        {
            auto pkt = std::make_shared<FakePacket>(_capacity);
            if (_hasAddr)
                pkt->setAddress(_addr);
            pkt->setSize(_size);
            if (_size > 0)
                std::memcpy(pkt->buffer(), _buffer.data(), _size);
            pkt->setHasAddr(_hasAddr);
            return pkt;
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

        void setHasAddr(bool v)
        {
            _hasAddr = v;
        }

      private:
        sockaddr_in _addr{};
        bool _hasAddr = true;
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
            return 4242;
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
        void setConsumeUdp(bool v)
        {
            _consumeUdp = v;
        }

        void setSessionFromUdp(int v)
        {
            _sessionFromUdp = v;
        }

        void setSeqValid(bool v)
        {
            _seqValid = v;
        }

        void setToken(int sid, uint64_t token)
        {
            _udpTokens[sid] = token;
        }

        int getOrCreateSession(const sockaddr_in &) override
        {
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
            _udpTokens[sessionId] = token;
        }

        uint64_t getUdpToken(int sessionId) const override
        {
            const auto it = _udpTokens.find(sessionId);
            return (it == _udpTokens.end()) ? 0 : it->second;
        }

        bool bindUdp(int sessionId, const sockaddr_in &udpAddr) override
        {
            bindCalls++;
            lastBindSession = sessionId;
            lastBindAddr = udpAddr;
            return _bindUdpOk;
        }

        const sockaddr_in *getUdpAddress(int) const override
        {
            return nullptr;
        }

        int getSessionIdFromUdp(const sockaddr_in &) const override
        {
            return _sessionFromUdp;
        }

        bool isSequenceValid(int, uint32_t) const noexcept override
        {
            return _seqValid;
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
            return false;
        }

        bool consumeUdp(const sockaddr_in &) override
        {
            consumeCalls++;
            return _consumeUdp;
        }

        void setLastScore(int, uint32_t) override
        {
        }

        std::optional<uint32_t> getLastScore(int) const override
        {
            return std::nullopt;
        }

        std::string getUsername(int) const override
        {
            return {};
        }

        int consumeCalls = 0;
        int bindCalls = 0;
        int lastBindSession = -1;
        sockaddr_in lastBindAddr{};

        bool _bindUdpOk = true;

      private:
        bool _consumeUdp = true;
        int _sessionFromUdp = 10;
        bool _seqValid = true;
        std::unordered_map<int, uint64_t> _udpTokens;
    };

    static std::shared_ptr<Engine::RoomManager> makeRealRoomManager(std::shared_ptr<FakeSessions> sessions)
    {
        auto udpServer = std::make_shared<FakeUdpServer>();

        auto proto = std::make_shared<FakePacket>(2048);
        auto udpFactory = std::make_shared<Net::Factory::UDPPacketFactory>(proto);

        std::shared_ptr<Engine::ScoreService> scores = nullptr;

        return std::make_shared<Engine::RoomManager>(
            sessions, udpServer, udpFactory, scores, "levels/world1/levels.json");
    }

} // namespace

TEST(UDPPacketRouter, NullPacketReturns)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto rooms = makeRealRoomManager(sessions);
    Net::UDPPacketRouter router(sessions, rooms);

    router.handlePacket(nullptr);
    EXPECT_EQ(sessions->consumeCalls, 0);
}

TEST(UDPPacketRouter, NoAddressReturnsBeforeConsume)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto rooms = makeRealRoomManager(sessions);
    Net::UDPPacketRouter router(sessions, rooms);

    DefaultData d{};
    fillHeader(d.header, Net::Protocol::UDP::PING, sizeof(DefaultData), 1);

    auto pkt = std::make_shared<FakePacket>(makeAddr(), toBytes(d));
    pkt->setHasAddr(false);

    router.handlePacket(pkt);
    EXPECT_EQ(sessions->consumeCalls, 0);
}

TEST(UDPPacketRouter, ConsumeUdpFalseDropsPacket)
{
    auto sessions = std::make_shared<FakeSessions>();
    sessions->setConsumeUdp(false);

    auto rooms = makeRealRoomManager(sessions);
    Net::UDPPacketRouter router(sessions, rooms);

    DefaultData d{};
    fillHeader(d.header, Net::Protocol::UDP::PING, sizeof(DefaultData), 1);

    auto pkt = std::make_shared<FakePacket>(makeAddr(), toBytes(d));
    router.handlePacket(pkt);

    EXPECT_EQ(sessions->consumeCalls, 1);
}

TEST(UDPPacketRouter, TooSmallOrTooBigSizeDrops)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto rooms = makeRealRoomManager(sessions);
    Net::UDPPacketRouter router(sessions, rooms);

    auto pktSmall = std::make_shared<FakePacket>(makeAddr(), std::vector<uint8_t>{1, 2, 3});
    router.handlePacket(pktSmall);

    std::vector<uint8_t> big(1501, 0);
    auto pktBig = std::make_shared<FakePacket>(makeAddr(), big);
    router.handlePacket(pktBig);

    EXPECT_EQ(sessions->consumeCalls, 2);
}

TEST(UDPPacketRouter, BadMagicDrops)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto rooms = makeRealRoomManager(sessions);
    Net::UDPPacketRouter router(sessions, rooms);

    DefaultData d{};
    fillHeader(d.header, Net::Protocol::UDP::PING, sizeof(DefaultData), 1);
    d.header.magic[0] ^= 0xFF;

    auto pkt = std::make_shared<FakePacket>(makeAddr(), toBytes(d));
    router.handlePacket(pkt);

    EXPECT_EQ(sessions->consumeCalls, 1);
}

TEST(UDPPacketRouter, ConnectBadTokenDoesNotBind)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto rooms = makeRealRoomManager(sessions);
    Net::UDPPacketRouter router(sessions, rooms);

    const uint32_t sid = 10;
    const uint64_t goodToken = 0x1122334455667788ULL;
    sessions->setToken(static_cast<int>(sid), goodToken);

    const uint64_t badToken = 0xAABBCCDDEEFF0011ULL;

    ConnectData cd{};
    fillHeader(cd.header, Net::Protocol::UDP::CONNECT, sizeof(ConnectData), 1);
    cd.sessionId = htonl(sid);
    cd.tokenHi = htonl(static_cast<uint32_t>(badToken >> 32));
    cd.tokenLo = htonl(static_cast<uint32_t>(badToken & 0xFFFFFFFFu));

    auto pkt = std::make_shared<FakePacket>(makeAddr(), toBytes(cd));
    router.handlePacket(pkt);

    EXPECT_EQ(sessions->bindCalls, 0);
}

TEST(UDPPacketRouter, ConnectGoodTokenBindsUdp)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto rooms = makeRealRoomManager(sessions);
    Net::UDPPacketRouter router(sessions, rooms);

    const uint32_t sid = 10;
    const uint64_t token = 0x1122334455667788ULL;
    sessions->setToken(static_cast<int>(sid), token);

    ConnectData cd{};
    fillHeader(cd.header, Net::Protocol::UDP::CONNECT, sizeof(ConnectData), 1);
    cd.sessionId = htonl(sid);
    cd.tokenHi = htonl(static_cast<uint32_t>(token >> 32));
    cd.tokenLo = htonl(static_cast<uint32_t>(token & 0xFFFFFFFFu));

    const auto from = makeAddr(9999);
    auto pkt = std::make_shared<FakePacket>(from, toBytes(cd));
    router.handlePacket(pkt);

    ASSERT_EQ(sessions->bindCalls, 1);
    EXPECT_EQ(sessions->lastBindSession, static_cast<int>(sid));
    EXPECT_EQ(sessions->lastBindAddr.sin_port, from.sin_port);
    EXPECT_EQ(sessions->lastBindAddr.sin_addr.s_addr, from.sin_addr.s_addr);
}
