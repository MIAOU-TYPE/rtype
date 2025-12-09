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
#include "mockPacket.hpp"
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

static sockaddr_in makeAddr(uint32_t ip, uint16_t port)
{
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(ip);
    a.sin_port = htons(port);
    return a;
}

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
    pkt->buffer()[sizeof(h)] = 0b11111;
    pkt->setSize(sizeof(h) + 1);

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->inputCalled);
    EXPECT_TRUE(sink->lastInput.up);
    EXPECT_TRUE(sink->lastInput.down);
    EXPECT_TRUE(sink->lastInput.left);
    EXPECT_TRUE(sink->lastInput.right);
    EXPECT_TRUE(sink->lastInput.shoot);
}
