/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** testTCProuter
*/

#include <gtest/gtest.h>

#ifndef _WIN32
    #include <arpa/inet.h>
#else
    #include <winsock2.h>
#endif
#include <chrono>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <string_view>
#include <unordered_map>

#include "TCPPacketFactory.hpp"
#include "TCPPacketRouter.hpp"

#include "IPacket.hpp"
#include "TCPPayload.hpp"
#include "TCPReader.hpp"
#include "TCPTypesData.hpp"
#include "TCPWriter.hpp"

#include "IServer.hpp"
#include "ISessionManager.hpp"

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

    static std::vector<uint8_t> buildErrorPayload(uint32_t req, uint16_t code, std::string_view msg)
    {
        Net::TCP::Writer b;
        b.u16(code);
        b.str16(msg);
        return Net::TCP::buildPayload(Net::Protocol::TCP::ERROR_MESSAGE, req, b.bytes());
    }

    static std::vector<uint8_t> bytesOf(const std::shared_ptr<Net::IPacket> &p)
    {
        if (!p)
            return {};
        const auto sz = static_cast<size_t>(p->size());
        const auto *buf = static_cast<const uint8_t *>(p->buffer());
        return std::vector<uint8_t>(buf, buf + sz);
    }

    class FakePacket final : public Net::IPacket {
      public:
        FakePacket(sockaddr_in addr, const std::vector<uint8_t> &bytes, size_t cap = 2048)
            : _addr(addr), _capacity(cap), _size(bytes.size()), _buffer(cap)
        {
            if (bytes.size() > cap)
                throw std::runtime_error("FakePacket: bytes > cap");
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
            pkt->setAddress(_addr);
            pkt->setSize(_size);
            if (_size > 0)
                std::memcpy(pkt->buffer(), _buffer.data(), _size);
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

    class FakeTcpServer final : public Net::Server::IServer {
      public:
        explicit FakeTcpServer(int port) : _port(port)
        {
        }

        void configure(const std::string &, int32_t port) override
        {
            _port = port;
        }

        void start() override
        {
            _running = true;
        }

        void stop() noexcept override
        {
            _running = false;
        }

        void setNonBlocking(bool) override
        {
        }

        [[nodiscard]] bool isRunning() const noexcept override
        {
            return _running;
        }

        void setRunning(bool r) noexcept override
        {
            _running = r;
        }

        [[nodiscard]] bool readPackets() noexcept override
        {
            return true;
        }

        [[nodiscard]] int getPort() const noexcept override
        {
            return _port;
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

        bool sendPacket(const Net::IPacket &p) noexcept override
        {
            sent.emplace_back(bytesOf(p.clone()));
            return true;
        }

        std::vector<std::vector<uint8_t>> sent;

      private:
        int _port = 0;
        bool _running = false;
    };

    class FakeSessions final : public Net::Server::ISessionManager {
      public:
        int getOrCreateSession(const sockaddr_in &addr) override
        {
            _sessions[_sessionId] = addr;
            _byAddrKey[addrKey(addr)] = _sessionId;
            return _sessionId;
        }

        int getSessionId(const sockaddr_in &addr) const override
        {
            const auto it = _byAddrKey.find(addrKey(addr));
            if (it == _byAddrKey.end())
                return -1;
            return it->second;
        }

        void removeSession(int sessionId) override
        {
            _udpTokens.erase(sessionId);
            _udpBinds.erase(sessionId);
            _identities.erase(sessionId);
            _lastScores.erase(sessionId);
            _sessions.erase(sessionId);
        }

        const sockaddr_in *getAddress(int sessionId) const override
        {
            const auto it = _sessions.find(sessionId);
            if (it == _sessions.end())
                return nullptr;
            return &it->second;
        }

        std::vector<std::pair<int, sockaddr_in>> getAllSessions() const override
        {
            std::vector<std::pair<int, sockaddr_in>> out;
            out.reserve(_sessions.size());
            for (const auto &kv : _sessions)
                out.push_back(kv);
            return out;
        }

        void forEachSession(const std::function<void(int, const sockaddr_in &)> &func) const override
        {
            for (const auto &kv : _sessions)
                func(kv.first, kv.second);
        }

        void setUdpToken(int sessionId, uint64_t token) override
        {
            _udpTokens[sessionId] = token;
        }

        uint64_t getUdpToken(int sessionId) const override
        {
            const auto it = _udpTokens.find(sessionId);
            if (it == _udpTokens.end())
                return 0;
            return it->second;
        }

        bool bindUdp(int sessionId, const sockaddr_in &udpAddr) override
        {
            _udpBinds[sessionId] = udpAddr;
            _byUdpAddrKey[addrKey(udpAddr)] = sessionId;
            return true;
        }

        const sockaddr_in *getUdpAddress(int sessionId) const override
        {
            const auto it = _udpBinds.find(sessionId);
            if (it == _udpBinds.end())
                return nullptr;
            return &it->second;
        }

        int getSessionIdFromUdp(const sockaddr_in &udpAddr) const override
        {
            const auto it = _byUdpAddrKey.find(addrKey(udpAddr));
            if (it == _byUdpAddrKey.end())
                return -1;
            return it->second;
        }

        bool isSequenceValid(int, uint32_t) const noexcept override
        {
            return true;
        }

        std::optional<Auth::Identity> getIdentity(int sessionId) const override
        {
            const auto it = _identities.find(sessionId);
            if (it == _identities.end())
                return std::nullopt;
            return it->second;
        }

        void setIdentity(int sessionId, Auth::Identity id, std::chrono::seconds) override
        {
            _identities[sessionId] = std::move(id);
        }

        void clearIdentity(int sessionId) override
        {
            _identities.erase(sessionId);
        }

        bool isAuthed(int sessionId) const override
        {
            return _identities.find(sessionId) != _identities.end();
        }

        bool consumeUdp(const sockaddr_in &) override
        {
            return true;
        }

        void setLastScore(int sessionId, uint32_t score) override
        {
            _lastScores[sessionId] = score;
        }

        std::optional<uint32_t> getLastScore(int sessionId) const override
        {
            const auto it = _lastScores.find(sessionId);
            if (it == _lastScores.end())
                return std::nullopt;
            return it->second;
        }

        std::string getUsername(int sessionId) const override
        {
            const auto it = _identities.find(sessionId);
            if (it == _identities.end())
                return {};
            return it->second.username;
        }

        void forceSessionId(int id)
        {
            _sessionId = id;
        }

        void forceAuthed(int id, std::string username = "u", uint32_t userId = 1)
        {
            _identities[id] = Auth::Identity{userId, std::move(username)};
        }

        void forceNotAuthed(int id)
        {
            _identities.erase(id);
        }

      private:
        static uint64_t addrKey(const sockaddr_in &a)
        {
            const uint64_t ip = static_cast<uint64_t>(a.sin_addr.s_addr);
            const uint64_t port = static_cast<uint64_t>(a.sin_port);
            const uint64_t fam = static_cast<uint64_t>(a.sin_family);
            return (ip << 32) ^ (port << 16) ^ fam;
        }

        int _sessionId = 10;

        std::unordered_map<int, sockaddr_in> _sessions;
        std::unordered_map<uint64_t, int> _byAddrKey;

        std::unordered_map<int, uint64_t> _udpTokens;
        std::unordered_map<int, sockaddr_in> _udpBinds;
        std::unordered_map<uint64_t, int> _byUdpAddrKey;

        std::unordered_map<int, Auth::Identity> _identities;
        std::unordered_map<int, uint32_t> _lastScores;
    };

} // namespace

TEST(TCPPacketRouter, HandleReturnsIfPacketHasNoAddress)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto tcp = std::make_shared<FakeTcpServer>(5000);

    auto proto = std::make_shared<FakePacket>(2048);
    auto factory = std::make_shared<Net::Factory::TCPPacketFactory>(proto);

    Net::TCPPacketRouter router(sessions, nullptr, tcp, factory, nullptr, nullptr);

    auto pkt = std::make_shared<FakePacket>(makeAddr(), std::vector<uint8_t>{1, 2, 3, 4, 5});
    pkt->setHasAddr(false);

    router.handle(pkt);
    EXPECT_TRUE(tcp->sent.empty());
}

TEST(TCPPacketRouter, TruncatedHeaderSendsErrorCode1Req0)
{
    auto sessions = std::make_shared<FakeSessions>();
    auto tcp = std::make_shared<FakeTcpServer>(5000);

    auto proto = std::make_shared<FakePacket>(2048);
    auto factory = std::make_shared<Net::Factory::TCPPacketFactory>(proto);

    Net::TCPPacketRouter router(sessions, nullptr, tcp, factory, nullptr, nullptr);

    const auto addr = makeAddr();
    const std::vector<uint8_t> tooSmall{0x01, 0x02, 0x03, 0x04};
    auto pkt = std::make_shared<FakePacket>(addr, tooSmall);

    router.handle(pkt);

    ASSERT_EQ(tcp->sent.size(), 1u);

    const std::string msg = "TCP header: truncated payload (need=5, got=4)";
    const auto expected = buildErrorPayload(0, 1, msg);
    EXPECT_EQ(tcp->sent[0], expected);
}

TEST(TCPPacketRouter, NotAuthedAndNotAuthFreeSends401AuthRequired)
{
    auto sessions = std::make_shared<FakeSessions>();
    sessions->forceSessionId(10);
    sessions->forceNotAuthed(10);

    auto tcp = std::make_shared<FakeTcpServer>(5000);

    auto proto = std::make_shared<FakePacket>(2048);
    auto factory = std::make_shared<Net::Factory::TCPPacketFactory>(proto);

    Net::TCPPacketRouter router(sessions, nullptr, tcp, factory, nullptr, nullptr);

    const auto addr = makeAddr();
    const uint32_t req = 42;

    const auto payload = Net::TCP::buildPayload(Net::Protocol::TCP::LIST_ROOMS, req, {});
    auto pkt = std::make_shared<FakePacket>(addr, payload);

    router.handle(pkt);

    ASSERT_EQ(tcp->sent.size(), 1u);
    const auto expected = buildErrorPayload(req, 401, "AUTH_REQUIRED");
    EXPECT_EQ(tcp->sent[0], expected);
}

TEST(TCPPacketRouter, HelloSendsWelcomeWithExistingTokenAndUdpPortIsTcpPlus1)
{
    auto sessions = std::make_shared<FakeSessions>();
    sessions->forceSessionId(10);
    sessions->forceNotAuthed(10);

    const uint64_t token = 0x1122334455667788ull;
    sessions->setUdpToken(10, token);

    auto tcp = std::make_shared<FakeTcpServer>(5000);

    auto proto = std::make_shared<FakePacket>(2048);
    auto factory = std::make_shared<Net::Factory::TCPPacketFactory>(proto);

    Net::TCPPacketRouter router(sessions, nullptr, tcp, factory, nullptr, nullptr);

    const auto addr = makeAddr();
    const uint32_t req = 7;
    const uint16_t ver = 3;

    Net::TCP::Writer body;
    body.u16(ver);

    const auto in = Net::TCP::buildPayload(Net::Protocol::TCP::HELLO, req, body.bytes());
    auto pkt = std::make_shared<FakePacket>(addr, in);

    router.handle(pkt);

    ASSERT_EQ(tcp->sent.size(), 1u);

    Net::TCP::Writer outBody;
    outBody.u16(ver);
    outBody.u32(static_cast<uint32_t>(10));
    outBody.u16(static_cast<uint16_t>(tcp->getPort() + 1));
    outBody.u32(static_cast<uint32_t>(token >> 32));
    outBody.u32(static_cast<uint32_t>(token & 0xFFFFFFFFu));

    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::WELCOME, req, outBody.bytes());
    EXPECT_EQ(tcp->sent[0], expected);

    EXPECT_EQ(sessions->getUdpToken(10), token);
}
