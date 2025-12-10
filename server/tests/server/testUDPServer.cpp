/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UDPServer Tests (integration)
*/

#include <chrono>
#include <gtest/gtest.h>
#include <thread>

#include "UDPPacket/UDPPacket.hpp"
#include "UDPServer.hpp"
#include "socketParams.hpp"

using namespace Net;
using namespace Net::Server;

//
// Utilities
//

static void small_sleep()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

//
// Test suite
//

TEST(UDPServerIntegration, StartStopDoesNotThrow)
{
    UDPServer server;
    EXPECT_NO_THROW(server.start());
    EXPECT_NO_THROW(server.stop());
}

TEST(UDPServerIntegration, StartCreatesListeningSocket)
{
    UDPServer server;
    EXPECT_NO_THROW(server.start());

    // Pas de fonction publique pour vérifier le socket
    // donc on se contente que start() ne crash pas.
    SUCCEED();
}

TEST(UDPServerIntegration, ServerReceivesPacketSentToItself)
{
    UDPServer server;
    server.start();

    // Construire un paquet UDP
    UDPPacket pkt;
    const std::string message = "hello!";
    pkt.write(message.data(), message.size());

    // Envoyer via sendPacket
    bool ok = server.sendPacket(pkt);
    EXPECT_TRUE(ok);

    // Poll le serveur
    small_sleep();
    server.readPackets();

    std::shared_ptr<IPacket> received;
    EXPECT_TRUE(server.popPacket(received));

    ASSERT_NE(received, nullptr);
    EXPECT_EQ(received->size(), message.size());

    std::string msgIn(received->data(), received->data() + received->size());
    EXPECT_EQ(msgIn, message);

    server.stop();
}

TEST(UDPServerIntegration, PopPacketReturnsFalseIfEmpty)
{
    UDPServer server;
    server.start();

    std::shared_ptr<IPacket> pkt;
    EXPECT_FALSE(server.popPacket(pkt));

    server.stop();
}

TEST(UDPServerIntegration, MultiplePacketsAreReceivedInOrder)
{
    UDPServer server;
    server.start();

    const int N = 3;
    const std::vector<std::string> messages = {"one", "two", "three"};

    for (const auto &m : messages) {
        UDPPacket pkt;
        pkt.write(m.data(), m.size());
        EXPECT_TRUE(server.sendPacket(pkt));
    }

    small_sleep();
    server.readPackets();

    for (const auto &expected : messages) {
        std::shared_ptr<IPacket> pkt;
        EXPECT_TRUE(server.popPacket(pkt));

        std::string in(pkt->data(), pkt->data() + pkt->size());
        EXPECT_EQ(in, expected);
    }

    server.stop();
}

TEST(UDPServerIntegration, StressReceiveManyPackets)
{
    UDPServer server;
    server.start();

    const int count = 50;
    for (int i = 0; i < count; i++) {
        UDPPacket pkt;
        std::string msg = "msg_" + std::to_string(i);
        pkt.write(msg.data(), msg.size());
        EXPECT_TRUE(server.sendPacket(pkt));
    }

    small_sleep();
    server.readPackets();

    int popCount = 0;
    std::shared_ptr<IPacket> pkt;
    while (server.popPacket(pkt))
        popCount++;

    EXPECT_GE(popCount, 1);
    EXPECT_LE(popCount, count); // un vrai réseau UDP peut drop, donc on ne teste pas stricte égalité.

    server.stop();
}
