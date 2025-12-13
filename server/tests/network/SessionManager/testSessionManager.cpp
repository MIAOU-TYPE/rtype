/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testSessionManager
*/

#include <gtest/gtest.h>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif

#include <thread>
#include "SessionManager.hpp"

using namespace Net::Server;

static sockaddr_in makeAddr(uint32_t ip, uint16_t port)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = port;
    return addr;
}

TEST(AddressKeyTests, HashEquality)
{
    AddressKey k1{0x11223344, 8080};
    AddressKey k2{0x11223344, 8080};
    AddressKey k3{0xAABBCCDD, 8080};

    AddressKeyHash h;

    EXPECT_TRUE(k1 == k2);
    EXPECT_NE(h(k1), 0u);

    EXPECT_EQ(h(k1), h(k2));

    EXPECT_NE(h(k1), h(k3));
}

TEST(SessionManagerTests, CreateNewSession)
{
    SessionManager sm;

    sockaddr_in addr = makeAddr(0x12345678, 5000);

    int id1 = sm.getOrCreateSession(addr);
    int id2 = sm.getOrCreateSession(addr);

    EXPECT_EQ(id1, id2);

    EXPECT_EQ(sm.getSessionId(addr), id1);
}

TEST(SessionManagerTests, CreateDifferentSessions)
{
    SessionManager sm;

    sockaddr_in addr1 = makeAddr(0x11111111, 5000);
    sockaddr_in addr2 = makeAddr(0x22222222, 5001);

    int id1 = sm.getOrCreateSession(addr1);
    int id2 = sm.getOrCreateSession(addr2);

    EXPECT_NE(id1, id2);
    EXPECT_EQ(sm.getSessionId(addr1), id1);
    EXPECT_EQ(sm.getSessionId(addr2), id2);
}

TEST(SessionManagerTests, RemoveSession)
{
    SessionManager sm;

    sockaddr_in addr = makeAddr(0x76543210, 4000);

    int id = sm.getOrCreateSession(addr);

    EXPECT_EQ(sm.getSessionId(addr), id);
    ASSERT_NE(sm.getAddress(id), nullptr);

    sm.removeSession(id);

    EXPECT_EQ(sm.getSessionId(addr), -1);
    EXPECT_EQ(sm.getAddress(id), nullptr);
}

TEST(SessionManagerTests, GetAddressReturnsCorrectPointer)
{
    SessionManager sm;

    sockaddr_in addr = makeAddr(0x10203040, 9999);
    int id = sm.getOrCreateSession(addr);

    const sockaddr_in *sin = sm.getAddress(id);
    ASSERT_NE(sin, nullptr);

    EXPECT_EQ(sin->sin_addr.s_addr, addr.sin_addr.s_addr);
    EXPECT_EQ(sin->sin_port, addr.sin_port);
}

TEST(SessionManagerTests, UnknownAddressReturnsMinusOne)
{
    SessionManager sm;

    sockaddr_in addr = makeAddr(0xAAAAAAAA, 1234);
    EXPECT_EQ(sm.getSessionId(addr), -1);
}

TEST(SessionManagerTests, ThreadSafetyBasic)
{
    SessionManager sm;

    sockaddr_in addr = makeAddr(0x11111111, 1111);

    int id1 = -1;
    int id2 = -1;

    std::thread t1([&] {
        id1 = sm.getOrCreateSession(addr);
    });
    std::thread t2([&] {
        id2 = sm.getOrCreateSession(addr);
    });

    t1.join();
    t2.join();

    EXPECT_EQ(id1, id2);
}

TEST(SessionManagerTests, TestApplyFunctionToEmptySessions)
{
    SessionManager sm;

    bool called = false;
    sm.forEachSession([&](int sessionId, const sockaddr_in &addr) {
        called = true;
    });

    EXPECT_FALSE(called);
}

TEST(SessionManagerTests, TestApplyFunctionToMultipleSessions)
{
    SessionManager sm;

    sockaddr_in addr1 = makeAddr(0xAAAAAAAA, 2000);
    sockaddr_in addr2 = makeAddr(0xBBBBBBBB, 2001);
    sockaddr_in addr3 = makeAddr(0xCCCCCCCC, 2002);

    int id1 = sm.getOrCreateSession(addr1);
    int id2 = sm.getOrCreateSession(addr2);
    int id3 = sm.getOrCreateSession(addr3);

    std::vector<int> collectedIds;
    sm.forEachSession([&](int sessionId, const sockaddr_in &addr) {
        collectedIds.push_back(sessionId);
    });

    EXPECT_EQ(collectedIds.size(), 3);
    EXPECT_NE(std::find(collectedIds.begin(), collectedIds.end(), id1), collectedIds.end());
    EXPECT_NE(std::find(collectedIds.begin(), collectedIds.end(), id2), collectedIds.end());
    EXPECT_NE(std::find(collectedIds.begin(), collectedIds.end(), id3), collectedIds.end());
}