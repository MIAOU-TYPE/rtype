/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testUDPServer
*/

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif
#include <cstring>
#include <gtest/gtest.h>

#include "UDPPacket.hpp"

using namespace Net;

// ------------------------------------------------------------
// Construction
// ------------------------------------------------------------

TEST(UDPPacket, DefaultConstructorInitializesEmpty)
{
    UDPPacket pkt;

    EXPECT_EQ(pkt.size(), 0);
    EXPECT_NE(pkt.buffer(), nullptr);
    EXPECT_EQ(pkt.capacity(), UDPPacket::MAX_SIZE);

    // Vérifier que le buffer est bien 0-initialisé
    for (size_t i = 0; i < pkt.capacity(); i++)
        EXPECT_EQ(pkt.buffer()[i], 0);
}

// ------------------------------------------------------------
// Buffer + Size
// ------------------------------------------------------------

TEST(UDPPacket, WriteAndReadBuffer)
{
    UDPPacket pkt;

    const char *str = "HelloWorld";
    size_t len = strlen(str);

    // Copie manuelle dans le buffer
    std::memcpy(pkt.buffer(), str, len);
    pkt.setSize(len);

    EXPECT_EQ(pkt.size(), len);

    std::string out(reinterpret_cast<char *>(pkt.buffer()), pkt.size());
    EXPECT_EQ(out, "HelloWorld");
}

TEST(UDPPacket, SetSizeDoesNotChangeData)
{
    UDPPacket pkt;

    const char *msg = "DATA";
    std::memcpy(pkt.buffer(), msg, 4);
    pkt.setSize(4);

    EXPECT_EQ(pkt.size(), 4);

    // Resize to smaller
    pkt.setSize(2);
    EXPECT_EQ(pkt.size(), 2);
    EXPECT_EQ(pkt.buffer()[0], 'D');
    EXPECT_EQ(pkt.buffer()[1], 'A');
}

// ------------------------------------------------------------
// Address handling
// ------------------------------------------------------------

TEST(UDPPacket, SetAndGetAddress)
{
    UDPPacket pkt;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4242);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    pkt.setAddress(addr);

    const sockaddr_in *out = pkt.address();

    ASSERT_NE(out, nullptr);
    EXPECT_EQ(out->sin_family, AF_INET);
    EXPECT_EQ(ntohs(out->sin_port), 4242);

    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &out->sin_addr, buf, sizeof(buf));

    EXPECT_STREQ(buf, "127.0.0.1");
}

// ------------------------------------------------------------
// Printing operator
// ------------------------------------------------------------

TEST(UDPPacket, StreamOperatorPrintsPrintableCharacters)
{
    UDPPacket pkt;

    const char *msg = "abcDEF123";
    std::memcpy(pkt.buffer(), msg, strlen(msg));
    pkt.setSize(strlen(msg));

    std::stringstream ss;
    ss << pkt;

    std::string out = ss.str();

    // operator<< ajoute un '\n' en fin
    EXPECT_EQ(out, "abcDEF123\n");
}

TEST(UDPPacket, StreamOperatorReplacesNonPrintableCharacters)
{
    UDPPacket pkt;

    uint8_t data[5] = {'A', 0x01, 'B', 0xFF, 'C'};
    std::memcpy(pkt.buffer(), data, 5);
    pkt.setSize(5);

    std::stringstream ss;
    ss << pkt;

    std::string out = ss.str();

    // caractères non imprimables → '.'
    EXPECT_EQ(out, "A.B.C\n");
}
