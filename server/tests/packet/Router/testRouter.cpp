/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testRouter
*/

#include <cstring>
#include <gtest/gtest.h>
#include <memory>
#include <netinet/in.h>

#include "IMessageSink.hpp"
#include "IPacket.hpp"
#include "ISessionManager.hpp"
#include "NetMessages.hpp"
#include "PacketRouter.hpp"

// =========================================================
//                     MOCKS POUR LES TESTS
// =========================================================

// ---- MockSessionManager ----
class MockSessionManager : public Net::Server::ISessionManager {
  public:
    int forcedSessionId = 1;
    Net::Server::AddressIn lastAddress{"0.0.0.0", 0};

    int getOrCreateSession(const Net::Server::AddressIn &addr) override
    {
        lastAddress = addr;
        return forcedSessionId;
    }

    int getSessionId(const Net::Server::AddressIn &addr) const override
    {
        return forcedSessionId;
    }

    void removeSession(int sessionId) override
    {
        removedSession = sessionId;
    }

    const Net::Server::AddressIn *getAddress(int sessionId) const override
    {
        return &lastAddress;
    }

    int removedSession = -1;
};

// ---- Mock Message Sink ----
class MockSink : public Net::IMessageSink {
  public:
    void onPlayerConnect(int sid) override
    {
        connectCalled = true;
        connectId = sid;
    }

    void onPlayerDisconnect(int sid) override
    {
        disconnectCalled = true;
        disconnectId = sid;
    }

    void onPing(int sid) override
    {
        pingCalled = true;
        pingId = sid;
    }

    void onPlayerInput(int sid, const Game::InputComponent &input) override
    {
        inputCalled = true;
        inputId = sid;
        lastInput = input;
    }

    bool connectCalled = false;
    bool disconnectCalled = false;
    bool pingCalled = false;
    bool inputCalled = false;

    int connectId = -1;
    int disconnectId = -1;
    int pingId = -1;
    int inputId = -1;

    Game::InputComponent lastInput{};
};

// ---- MockPacket conforme IPacket ----
class MockPacket : public Net::IPacket {
  public:
    MockPacket(size_t cap = 256) : _capacity(cap)
    {
        _buffer.resize(cap);
        _size = 0;
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
        return &_addr;
    }

    void setAddress(const sockaddr_in &a) override
    {
        _addr = a;
    }

    std::shared_ptr<Net::IPacket> clone() const override
    {
        return std::make_shared<MockPacket>(_capacity);
    }

    size_t capacity() const noexcept override
    {
        return _capacity;
    }

  private:
    std::vector<uint8_t> _buffer;
    sockaddr_in _addr{};
    size_t _size;
    size_t _capacity;
};

// Helper
static sockaddr_in makeAddr(uint32_t ip, uint16_t port)
{
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(ip);
    a.sin_port = htons(port);
    return a;
}

// =========================================================
//                    TESTS PacketRouter
// =========================================================

TEST(PacketRouter, RejectsNullPacket)
{
    auto sess = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();

    Net::PacketRouter pr(sess, sink);

    EXPECT_NO_THROW(pr.handlePacket(nullptr));
}

TEST(PacketRouter, RejectsTooSmallPacket)
{
    auto sess = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sess, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setSize(1);

    pr.handlePacket(pkt);

    EXPECT_FALSE(sink->connectCalled);
}

TEST(PacketRouter, RejectsWrongVersion)
{
    auto sess = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sess, sink);

    auto pkt = std::make_shared<MockPacket>();

    HeaderData h{};
    h.type = Net::Protocol::CONNECT;
    h.version = 99; // invalid
    h.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->setSize(sizeof(HeaderData));

    pr.handlePacket(pkt);

    EXPECT_FALSE(sink->connectCalled);
}

TEST(PacketRouter, DispatchConnect)
{
    auto sess = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sess, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0x01020304, 5000));

    HeaderData h{};
    h.type = Net::Protocol::CONNECT;
    h.version = 1;
    h.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->setSize(sizeof(HeaderData));

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->connectCalled);
    EXPECT_EQ(sink->connectId, sess->forcedSessionId);
}

TEST(PacketRouter, DispatchDisconnect)
{
    auto sess = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sess, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0x01020304, 6000));

    HeaderData h{};
    h.type = Net::Protocol::DISCONNECT;
    h.version = 1;
    h.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->setSize(sizeof(HeaderData));

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->disconnectCalled);
    EXPECT_EQ(sess->removedSession, sess->forcedSessionId);
}

TEST(PacketRouter, DispatchPing)
{
    auto sess = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sess, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0xDEADBEEF, 9000));

    HeaderData h{};
    h.type = Net::Protocol::PING;
    h.version = 1;
    h.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->setSize(sizeof(HeaderData));

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->pingCalled);
}

TEST(PacketRouter, DispatchInput)
{
    auto sess = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sess, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0xCAFEBABE, 7000));

    HeaderData h{};
    h.type = Net::Protocol::INPUT;
    h.version = 1;
    h.size = htons(sizeof(HeaderData) + 1);

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->buffer()[sizeof(h)] = 0b11111; // up down left right shoot
    pkt->setSize(sizeof(h) + 1);

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->inputCalled);
    EXPECT_TRUE(sink->lastInput.up);
    EXPECT_TRUE(sink->lastInput.down);
    EXPECT_TRUE(sink->lastInput.left);
    EXPECT_TRUE(sink->lastInput.right);
    EXPECT_TRUE(sink->lastInput.shoot);
}
