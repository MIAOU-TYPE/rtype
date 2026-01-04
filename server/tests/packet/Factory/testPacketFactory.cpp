/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testUDPPacketFactory
*/
#include <cstring>
#include <gtest/gtest.h>
#include <memory>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif
#include "DamageData.hpp"
#include "DefaultData.hpp"
#include "Endian.hpp"
#include "UDPPacketFactory.hpp"
#include "mockIPacket.hpp"

static sockaddr_in makeAddr(uint32_t ip, uint16_t port)
{
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = ip;
    a.sin_port = port;
    return a;
}

TEST(UDPPacketFactory, ConstructorThrowsOnNull)
{
    EXPECT_THROW(Net::Factory::UDPPacketFactory(nullptr), Net::Factory::FactoryError);
}

TEST(UDPPacketFactory, ConstructorAcceptsValidPacket)
{
    std::shared_ptr<Net::IPacket> pkt = std::make_shared<MockPacket>();
    EXPECT_NO_THROW(Net::Factory::UDPPacketFactory factory(pkt));
}

TEST(UDPPacketFactory, MakeDefaultPacket)
{
    auto pkt = std::make_shared<MockPacket>();
    Net::Factory::UDPPacketFactory f(pkt);

    sockaddr_in addr = makeAddr(0x12345678, 5555);
    uint8_t flag = Net::Protocol::UDP::PING;

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

TEST(UDPPacketFactory, MakeDamagePacket)
{
    auto pkt = std::make_shared<MockPacket>();
    Net::Factory::UDPPacketFactory f(pkt);

    sockaddr_in addr = makeAddr(0xDEADBEEF, 7777);
    uint32_t id = 4242;
    uint16_t amount = 1337;

    auto p = f.makeDamage(addr, id, amount);
    ASSERT_NE(p, nullptr);

    const auto *raw = reinterpret_cast<const DamageData *>(p->buffer());

    EXPECT_EQ(raw->header.type, Net::Protocol::UDP::DAMAGE_EVENT);
    EXPECT_EQ(raw->id, htonl(id));
    EXPECT_EQ(raw->amount, htons(amount));
}
