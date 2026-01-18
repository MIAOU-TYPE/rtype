/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** testTCPPacketFactory
*/

#include <gtest/gtest.h>

#ifndef _WIN32
    #include <arpa/inet.h>
#else
    #include <winsock2.h>
#endif
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include <string_view>

#include "TCPPacketFactory.hpp"

#include "RoomData.hpp"
#include "ScoreEntry.hpp"
#include "TCPPayload.hpp"
#include "TCPTypesData.hpp"
#include "TCPWriter.hpp"

namespace
{
    using ReqId = Net::Factory::TCPPacketFactory::ReqId;
    using RoomId = Net::Factory::TCPPacketFactory::RoomId;
    using SessionId = Net::Factory::TCPPacketFactory::SessionId;

    static sockaddr_in makeAddr()
    {
        sockaddr_in a{};
        a.sin_family = AF_INET;
        a.sin_addr.s_addr = htonl(0x7F000001u);
        a.sin_port = htons(8080);
        return a;
    }

    static std::vector<uint8_t> bytesOf(const std::shared_ptr<Net::IPacket> &p)
    {
        if (!p)
            return {};
        const auto sz = static_cast<size_t>(p->size());
        const auto *buf = p->buffer();
        return std::vector<uint8_t>(buf, buf + sz);
    }

    class FakePacket final : public Net::IPacket {
      public:
        explicit FakePacket(size_t cap = 512) : _capacity(cap)
        {
            _buffer.resize(cap);
        }

        std::shared_ptr<Net::IPacket> newPacket() const override
        {
            return std::make_shared<FakePacket>(_capacity);
        }

        std::shared_ptr<Net::IPacket> clone() const override
        {
            auto pkt = std::make_shared<FakePacket>(_capacity);
            pkt->_size = _size;
            pkt->_addr = _addr;
            if (_size > 0)
                std::memcpy(pkt->_buffer.data(), _buffer.data(), _size);
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
            return &_addr;
        }

        void setAddress(const sockaddr_in &addr) override
        {
            _addr = addr;
        }

        size_t capacity() const noexcept override
        {
            return _capacity;
        }

      private:
        std::vector<uint8_t> _buffer;
        size_t _capacity = 0;
        size_t _size = 0;
        sockaddr_in _addr{};
    };

    class FakeProtoNullNewPacket final : public Net::IPacket {
      public:
        std::shared_ptr<Net::IPacket> newPacket() const override
        {
            return nullptr;
        }

        std::shared_ptr<Net::IPacket> clone() const override
        {
            return std::make_shared<FakeProtoNullNewPacket>();
        }

        uint8_t *buffer() override
        {
            return nullptr;
        }

        const uint8_t *buffer() const override
        {
            return nullptr;
        }

        size_t size() const override
        {
            return 0;
        }

        void setSize(size_t) override
        {
        }

        const sockaddr_in *address() const override
        {
            return nullptr;
        }

        void setAddress(const sockaddr_in &) override
        {
        }

        size_t capacity() const noexcept override
        {
            return 0;
        }
    };

    static std::shared_ptr<Net::IPacket> makeProtoWithCapacity(size_t cap)
    {
        return std::make_shared<FakePacket>(cap);
    }

} // namespace

TEST(TCPPacketFactory, AnyBuilderReturnsNullWhenProtoIsNull)
{
    Net::Factory::TCPPacketFactory f(nullptr);

    const auto p = f.makeRoomCreated(makeAddr(), 1, 42);
    EXPECT_EQ(p, nullptr);
}

TEST(TCPPacketFactory, AnyBuilderReturnsNullWhenProtoNewPacketReturnsNull)
{
    auto proto = std::make_shared<FakeProtoNullNewPacket>();
    Net::Factory::TCPPacketFactory f(proto);

    const auto p = f.makeRoomCreated(makeAddr(), 1, 42);
    EXPECT_EQ(p, nullptr);
}

TEST(TCPPacketFactory, BuilderReturnsNullWhenPayloadTooLargeForCapacity)
{
    auto proto = makeProtoWithCapacity(4);
    Net::Factory::TCPPacketFactory f(proto);

    const auto p = f.makeAuthOk(makeAddr(), 1, 1, "bob", 0x1122334455667788ull, 60);
    EXPECT_EQ(p, nullptr);
}

TEST(TCPPacketFactory, MakeErrorBuildsExpectedPayloadAndSetsAddress)
{
    auto proto = makeProtoWithCapacity(512);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 123;
    const uint16_t code = 42;
    const std::string_view msg = "nope";

    Net::TCP::Writer b;
    b.u16(code);
    b.str16(msg);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::ERROR_MESSAGE, req, b.bytes());

    const auto p = f.makeError(addr, req, code, msg);
    ASSERT_NE(p, nullptr);

    EXPECT_EQ(bytesOf(p), expected);

    ASSERT_NE(p->address(), nullptr);
    EXPECT_EQ(p->address()->sin_family, addr.sin_family);
    EXPECT_EQ(p->address()->sin_port, addr.sin_port);
    EXPECT_EQ(p->address()->sin_addr.s_addr, addr.sin_addr.s_addr);
}

TEST(TCPPacketFactory, MakeWelcomeBuildsExpectedPayload)
{
    auto proto = makeProtoWithCapacity(512);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 7;
    const uint16_t ver = 3;
    const SessionId sessionId = 0xAABBCCDDu;
    const uint16_t udpPort = 4242;
    const uint64_t token = 0x1122334455667788ull;

    Net::TCP::Writer b;
    b.u16(ver);
    b.u32(sessionId);
    b.u16(udpPort);
    b.u32(static_cast<uint32_t>(token >> 32));
    b.u32(static_cast<uint32_t>(token & 0xFFFFFFFFu));
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::WELCOME, req, b.bytes());

    const auto p = f.makeWelcome(addr, req, ver, sessionId, udpPort, token);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}

TEST(TCPPacketFactory, MakeRoomCreatedBuildsExpectedPayload)
{
    auto proto = makeProtoWithCapacity(256);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 9;
    const RoomId roomId = 0xDEADBEEFu;

    Net::TCP::Writer b;
    b.u32(roomId);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::ROOM_CREATED, req, b.bytes());

    const auto p = f.makeRoomCreated(addr, req, roomId);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}

TEST(TCPPacketFactory, MakeRoomJoinedBuildsExpectedPayload)
{
    auto proto = makeProtoWithCapacity(256);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 10;
    const RoomId roomId = 777;

    Net::TCP::Writer b;
    b.u32(roomId);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::ROOM_JOINED, req, b.bytes());

    const auto p = f.makeRoomJoined(addr, req, roomId);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}

TEST(TCPPacketFactory, MakeRoomLeftBuildsExpectedPayload)
{
    auto proto = makeProtoWithCapacity(256);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 11;
    const RoomId roomId = 888;

    Net::TCP::Writer b;
    b.u32(roomId);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::ROOM_LEFT, req, b.bytes());

    const auto p = f.makeRoomLeft(addr, req, roomId);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}

TEST(TCPPacketFactory, MakeGameStartBuildsExpectedPayload)
{
    auto proto = makeProtoWithCapacity(256);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 12;
    const RoomId roomId = 999;

    Net::TCP::Writer b;
    b.u32(roomId);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::GAME_START, req, b.bytes());

    const auto p = f.makeGameStart(addr, req, roomId);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}

TEST(TCPPacketFactory, MakeAuthOkBuildsExpectedPayload)
{
    auto proto = makeProtoWithCapacity(512);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 13;
    const uint32_t userId = 123456u;
    const std::string_view username = "bob";
    const uint64_t token = 0x0123456789ABCDEFull;
    const uint32_t ttl = 3600;

    Net::TCP::Writer b;
    b.u32(userId);
    b.str16(username);
    b.u32(static_cast<uint32_t>(token >> 32));
    b.u32(static_cast<uint32_t>(token & 0xFFFFFFFFu));
    b.u32(ttl);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::AUTH_OK, req, b.bytes());

    const auto p = f.makeAuthOk(addr, req, userId, username, token, ttl);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}

TEST(TCPPacketFactory, MakeRoomsListTooManyRoomsReturnsErrorPayload)
{
    auto proto = makeProtoWithCapacity(2048);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 1;

    std::vector<RoomData> rooms;
    rooms.resize(0x10000u);

    const uint16_t code = 16;
    const std::string_view msg = "LIST_ROOMS: too many rooms to fit in u16";
    Net::TCP::Writer b;
    b.u16(code);
    b.str16(msg);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::ERROR_MESSAGE, req, b.bytes());

    const auto p = f.makeRoomsList(addr, req, rooms);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}

TEST(TCPPacketFactory, MakeScoreboardListTooManyRowsReturnsErrorPayload)
{
    auto proto = makeProtoWithCapacity(2048);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 2;

    std::vector<ScoreEntry> scores;
    scores.resize(0x10000u);

    const uint16_t code = 16;
    const std::string_view msg = "SCOREBOARD_GET: too many rows to fit in u16";
    Net::TCP::Writer b;
    b.u16(code);
    b.str16(msg);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::ERROR_MESSAGE, req, b.bytes());

    const auto p = f.makeScoreboardList(addr, req, scores);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}

TEST(TCPPacketFactory, MakeRoomUpdatedTooManyPlayersReturnsErrorPayload)
{
    auto proto = makeProtoWithCapacity(2048);
    Net::Factory::TCPPacketFactory f(proto);

    const auto addr = makeAddr();
    const ReqId req = 3;

    RoomData room{};
    room.roomName = "x";
    room.maxPlayers = 4;
    room.playerNames.resize(0x10000u);

    const uint16_t code = 16;
    const std::string_view msg = "ROOM_UPDATED: too many players to fit in u16";
    Net::TCP::Writer b;
    b.u16(code);
    b.str16(msg);
    const auto expected = Net::TCP::buildPayload(Net::Protocol::TCP::ERROR_MESSAGE, req, b.bytes());

    const auto p = f.makeRoomUpdated(addr, req, room);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(bytesOf(p), expected);
}
