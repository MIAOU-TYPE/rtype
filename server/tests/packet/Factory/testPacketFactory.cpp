/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testPacketFactory
*/
#include <cstring>
#include <gtest/gtest.h>
#include <memory>
#include <netinet/in.h>

#include "DamageData.hpp"
#include "DefaultData.hpp"
#include "Endian.hpp"
#include "EntityCreateData.hpp"
#include "EntityDestroyData.hpp"
#include "PacketFactory.hpp"
#include "mockIPacket.hpp"

static sockaddr_in makeAddr(uint32_t ip, uint16_t port)
{
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = ip;
    a.sin_port = port;
    return a;
}

TEST(PacketFactory, ConstructorThrowsOnNull)
{
    EXPECT_THROW(Net::Factory::PacketFactory(nullptr), Net::Factory::FactoryError);
}

TEST(PacketFactory, ConstructorAcceptsValidPacket)
{
    std::shared_ptr<Net::IPacket> pkt = std::make_shared<MockPacket>();
    EXPECT_NO_THROW(Net::Factory::PacketFactory factory(pkt));
}

TEST(PacketFactory, MakeDefaultPacket)
{
    auto pkt = std::make_shared<MockPacket>();
    Net::Factory::PacketFactory f(pkt);

    sockaddr_in addr = makeAddr(0x12345678, 5555);
    uint8_t flag = Net::Protocol::PING;

    auto p = f.makeDefault(addr, flag);
    ASSERT_NE(p, nullptr);

    const auto *raw = reinterpret_cast<const DefaultData *>(p->buffer());

    EXPECT_EQ(raw->header.type, flag);
    EXPECT_EQ(raw->header.version, 1);
    EXPECT_EQ(raw->header.size, htons(sizeof(DefaultData)));

    ASSERT_NE(p->address(), nullptr);
    EXPECT_EQ(p->address()->sin_addr.s_addr, addr.sin_addr.s_addr);
    EXPECT_EQ(p->address()->sin_port, addr.sin_port);
}

TEST(PacketFactory, MakeEntityCreatePacket)
{
    auto pkt = std::make_shared<MockPacket>();
    Net::Factory::PacketFactory f(pkt);

    sockaddr_in addr = makeAddr(0x11223344, 9999);

    size_t id = 777;
    float x = 12.5f;
    float y = 99.75f;
    uint16_t sprite = 42;

    auto p = f.makeEntityCreate(addr, id, x, y, sprite);
    ASSERT_NE(p, nullptr);

    const auto *raw = reinterpret_cast<const EntityCreateData *>(p->buffer());

    EXPECT_EQ(raw->header.type, Net::Protocol::ENTITY_CREATE);
    EXPECT_EQ(raw->header.version, 1);
    EXPECT_EQ(raw->header.size, htons(sizeof(EntityCreateData)));

    EXPECT_EQ(raw->id, htobe64(id));
    EXPECT_EQ(raw->x, htonf(x));
    EXPECT_EQ(raw->y, htonf(y));
    EXPECT_EQ(raw->sprite, htons(sprite));
}

TEST(PacketFactory, MakeEntityDestroyPacket)
{
    auto pkt = std::make_shared<MockPacket>();
    Net::Factory::PacketFactory f(pkt);

    sockaddr_in addr = makeAddr(0xAABBCCDD, 2222);
    size_t id = 123456;

    auto p = f.makeEntityDestroy(addr, id);
    ASSERT_NE(p, nullptr);

    const auto *raw = reinterpret_cast<const EntityDestroyData *>(p->buffer());

    EXPECT_EQ(raw->header.type, Net::Protocol::ENTITY_DESTROY);
    EXPECT_EQ(raw->id, htobe64(id));
}

TEST(PacketFactory, MakeDamagePacket)
{
    auto pkt = std::make_shared<MockPacket>();
    Net::Factory::PacketFactory f(pkt);

    sockaddr_in addr = makeAddr(0xDEADBEEF, 7777);
    uint32_t id = 4242;
    uint16_t amount = 1337;

    auto p = f.makeDamage(addr, id, amount);
    ASSERT_NE(p, nullptr);

    const auto *raw = reinterpret_cast<const DamageData *>(p->buffer());

    EXPECT_EQ(raw->header.type, Net::Protocol::DAMAGE_EVENT);
    EXPECT_EQ(raw->id, htonl(id));
    EXPECT_EQ(raw->amount, htons(amount));
}
