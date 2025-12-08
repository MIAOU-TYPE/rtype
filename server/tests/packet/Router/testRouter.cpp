#include <cstring>
#include <gtest/gtest.h>
#include <memory>
#include <netinet/in.h>

#include "../../../src/network/SessionManager/Manager/SessionManager.hpp"
#include "IPacket.hpp"
#include "PacketFactory.hpp"
#include "PacketRouter.hpp"
#include "TypesData.hpp"

// =========================================================
//                     MOCKS POUR LES TESTS
// =========================================================

// ---- Mock SessionManager
class MockSessionManager : public Net::Server::SessionManager {
  public:
    MockSessionManager() : SessionManager()
    {
    }

    int getOrCreateSession(const sockaddr_in &addr)
    {
        lastAddr = addr;
        return forcedId;
    }

    int forcedId = 1;
    sockaddr_in lastAddr{};
};

// ---- Mock Message Sink
class MockSink : public Net::IMessageSink {
  public:
    void onPlayerConnect(int sessionId) override
    {
        connectCalled = true;
        connectId = sessionId;
    }

    void onPlayerDisconnect(int sessionId) override
    {
        disconnectCalled = true;
        disconnectId = sessionId;
    }

    void onPing(int sessionId) override
    {
        pingCalled = true;
        pingId = sessionId;
    }

    void onPlayerInput(int sessionId, const Game::InputComponent &input) override
    {
        inputCalled = true;
        inputId = sessionId;
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

// ---- Mock IPacket conforme à l'interface
class MockPacket : public Net::IPacket {
  public:
    explicit MockPacket(size_t cap = 256) : _capacity(cap), _size(0)
    {
        _buffer.resize(cap);
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

    void setAddress(const sockaddr_in &addr) override
    {
        _addr = addr;
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
    a.sin_addr.s_addr = ip;
    a.sin_port = port;
    return a;
}

// =========================================================
//                       TESTS PacketRouter
// =========================================================

TEST(PacketRouter, RejectsNullPacket)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sessions, sink);

    EXPECT_FALSE(pr.isPacketValid(nullptr));
}

TEST(PacketRouter, RejectsTooSmallPacket)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setSize(1); // < sizeof(HeaderData)

    EXPECT_FALSE(pr.isPacketValid(pkt));
}

TEST(PacketRouter, RejectsWrongVersion)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();

    HeaderData header{};
    header.type = Net::Protocol::CONNECT;
    header.version = 99; // Mauvaise version
    header.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &header, sizeof(header));
    pkt->setSize(sizeof(HeaderData));

    HeaderData extracted{};
    EXPECT_FALSE(pr.extractHeader(*pkt, extracted));
}

TEST(PacketRouter, RejectsSizeMismatch)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();

    HeaderData header{};
    header.type = Net::Protocol::CONNECT;
    header.version = PROTOCOL_VERSION;
    header.size = htons(999); // mauvais

    std::memcpy(pkt->buffer(), &header, sizeof(header));
    pkt->setSize(sizeof(HeaderData));

    HeaderData extracted{};
    EXPECT_FALSE(pr.extractHeader(*pkt, extracted));
}

TEST(PacketRouter, AcceptsValidHeader)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();

    HeaderData header{};
    header.type = Net::Protocol::CONNECT;
    header.version = PROTOCOL_VERSION;
    header.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &header, sizeof(header));
    pkt->setSize(sizeof(HeaderData));

    HeaderData extracted{};
    EXPECT_TRUE(pr.extractHeader(*pkt, extracted));
}

TEST(PacketRouter, DispatchConnect)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();

    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0x11111111, 1000));

    HeaderData h{};
    h.type = Net::Protocol::CONNECT;
    h.version = 1;
    h.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->setSize(sizeof(HeaderData));

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->connectCalled);
    EXPECT_EQ(sink->connectId, sessions->forcedId);
}

TEST(PacketRouter, DispatchDisconnect)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();

    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0x11111111, 2222));

    HeaderData h{};
    h.type = Net::Protocol::DISCONNECT;
    h.version = PROTOCOL_VERSION;
    h.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->setSize(sizeof(HeaderData));

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->disconnectCalled);
}

TEST(PacketRouter, DispatchPing)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();

    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0x99999999, 3000));

    HeaderData h{};
    h.type = Net::Protocol::PING;
    h.version = PROTOCOL_VERSION;
    h.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->setSize(sizeof(HeaderData));

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->pingCalled);
}

TEST(PacketRouter, DispatchInput)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0x88888888, 4444));

    HeaderData h{};
    h.type = Net::Protocol::INPUT;
    h.version = PROTOCOL_VERSION;
    h.size = htons(sizeof(HeaderData) + 1);

    uint8_t payload = 0x1D; // 00011101 → up, down, left, right + shoot

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->buffer()[sizeof(h)] = payload;
    pkt->setSize(sizeof(h) + 1);

    pr.handlePacket(pkt);

    EXPECT_TRUE(sink->inputCalled);
    EXPECT_TRUE(sink->lastInput.up);
    EXPECT_TRUE(sink->lastInput.down);
    EXPECT_TRUE(sink->lastInput.left);
    EXPECT_TRUE(sink->lastInput.right);
    EXPECT_TRUE(sink->lastInput.shoot);
    EXPECT_EQ(sink->inputId, sessions->forcedId);
}

TEST(PacketRouter, InputMissingPayloadIsRejected)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto sink = std::make_shared<MockSink>();
    Net::PacketRouter pr(sessions, sink);

    auto pkt = std::make_shared<MockPacket>();
    pkt->setAddress(makeAddr(0xEEEEEEEE, 5000));

    HeaderData h{};
    h.type = Net::Protocol::INPUT;
    h.version = PROTOCOL_VERSION;
    h.size = htons(sizeof(HeaderData));

    std::memcpy(pkt->buffer(), &h, sizeof(h));
    pkt->setSize(sizeof(h)); // payload manquant

    pr.handlePacket(pkt);

    EXPECT_FALSE(sink->inputCalled);
}
