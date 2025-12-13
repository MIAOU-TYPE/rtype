/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testPacket
*/
#include <gtest/gtest.h>
#include "UDPPacket.hpp"

using namespace Net;

TEST(UDPPacketTests, DefaultConstructor)
{
    UDPPacket pkt;

    EXPECT_EQ(pkt.size(), 0);
    EXPECT_EQ(pkt.capacity(), UDPPacket::MAX_SIZE);
    EXPECT_NE(pkt.buffer(), nullptr);

    for (size_t i = 0; i < pkt.capacity(); i++)
        EXPECT_EQ(pkt.buffer()[i], 0);
}

TEST(UDPPacketTests, SetAndGetSize)
{
    UDPPacket pkt;

    pkt.setSize(10);
    EXPECT_EQ(pkt.size(), 10);

    pkt.setSize(0);
    EXPECT_EQ(pkt.size(), 0);

    pkt.setSize(UDPPacket::MAX_SIZE);
    EXPECT_EQ(pkt.size(), UDPPacket::MAX_SIZE);
}

TEST(UDPPacketTests, BufferWriteAndRead)
{
    UDPPacket pkt;

    pkt.setSize(5);
    uint8_t *buf = pkt.buffer();

    buf[0] = 'H';
    buf[1] = 'e';
    buf[2] = 'l';
    buf[3] = 'l';
    buf[4] = 'o';

    EXPECT_EQ(pkt.buffer()[0], 'H');
    EXPECT_EQ(pkt.buffer()[4], 'o');
}

TEST(UDPPacketTests, AddressSetAndGet)
{
    UDPPacket pkt;

    sockaddr_in addr{};
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(0x7F000001);

    pkt.setAddress(addr);

    const sockaddr_in *retrieved = pkt.address();

    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->sin_port, htons(8080));
    EXPECT_EQ(retrieved->sin_family, AF_INET);
    EXPECT_EQ(retrieved->sin_addr.s_addr, htonl(0x7F000001));
}

TEST(UDPPacketTests, CloneCreatesDeepCopy)
{
    UDPPacket pkt;
    pkt.setSize(4);

    pkt.buffer()[0] = 1;
    pkt.buffer()[1] = 2;
    pkt.buffer()[2] = 3;
    pkt.buffer()[3] = 4;

    sockaddr_in addr{};
    addr.sin_port = htons(5000);
    pkt.setAddress(addr);

    auto clone = pkt.clone();
    ASSERT_NE(clone, nullptr);

    EXPECT_EQ(clone->size(), pkt.size());
    EXPECT_EQ(clone->address()->sin_port, pkt.address()->sin_port);
    EXPECT_EQ(clone->address()->sin_family, pkt.address()->sin_family);
    EXPECT_EQ(clone->address()->sin_addr.s_addr, pkt.address()->sin_addr.s_addr);

    clone->buffer()[0] = 99;
    EXPECT_NE(clone->buffer()[0], pkt.buffer()[0]);
}

TEST(UDPPacketTests, StreamOperatorPrintsAsAsciiOrDot)
{
    UDPPacket pkt;
    pkt.setSize(4);

    pkt.buffer()[0] = 'A';
    pkt.buffer()[1] = 'B';
    pkt.buffer()[2] = 0x01;
    pkt.buffer()[3] = '!';

    std::stringstream ss;
    ss << pkt;

    EXPECT_EQ(ss.str(), "AB.!\n");
}

TEST(UDPPacketTests, StreamOperatorSharedPtr)
{
    auto pkt = std::make_shared<UDPPacket>();
    pkt->setSize(3);
    pkt->buffer()[0] = 'X';
    pkt->buffer()[1] = 'Y';
    pkt->buffer()[2] = 'Z';

    std::stringstream ss;
    ss << pkt;

    EXPECT_EQ(ss.str(), "XYZ\n");
}
