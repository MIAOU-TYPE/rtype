/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testPluginLib
*/

#include <cstring>
#include <gtest/gtest.h>
#include "NetWrapper.hpp"

using namespace Net;

const std::string defaultPath = "../../"
// #ifdef _WIN32
//                                 // "Debug/"
// #else
//                                 ""
// #endif
    ;

TEST(NetWrapperTests, LoadValidPlugin)
{
    ASSERT_NO_THROW({ NetWrapper wrapperNetPluginLib("NetPluginLib", defaultPath); });
}

TEST(NetWrapperTests, LoadInvalidPluginThrows)
{
    EXPECT_THROW(NetWrapper wrapper("invalid_plugin_path"), NetWrapperError);
}

TEST(NetWrapperTests, SocketCreationThroughWrapper)
{
    NetWrapper wrapper("NetPluginLib", defaultPath);
    wrapper.initNetwork();

    socketHandle s = wrapper.socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT_NE(s, kInvalidSocket);

    wrapper.closeSocket(s);
}

TEST(NetWrapperTests, SetSocketOptionThroughWrapper)
{
    NetWrapper wrapper("NetPluginLib", defaultPath);
    wrapper.initNetwork();
    socketHandle s = wrapper.socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT_NE(s, kInvalidSocket);

    int enable = 1;
    int ret = wrapper.setSocketOpt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    EXPECT_EQ(ret, 0);

    wrapper.closeSocket(s);
}

TEST(NetWrapperTests, SendAndReceiveLocalUDP)
{
    NetWrapper wrapper("NetPluginLib", defaultPath);
    wrapper.initNetwork();

    socketHandle s1 = wrapper.socket(AF_INET, SOCK_DGRAM, 0);
    socketHandle s2 = wrapper.socket(AF_INET, SOCK_DGRAM, 0);

    ASSERT_NE(s1, kInvalidSocket);
    ASSERT_NE(s2, kInvalidSocket);

    sockaddr_in addrRecv{};
    addrRecv.sin_family = AF_INET;
    addrRecv.sin_port = htons(0);
    addrRecv.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    ASSERT_EQ(::bind(s2, (sockaddr *) &addrRecv, sizeof(addrRecv)), 0);

    socklen_t len = sizeof(addrRecv);
    ASSERT_EQ(::getsockname(s2, (sockaddr *) &addrRecv, &len), 0);

    sockaddr_in addrSend = addrRecv;

    const char *msg = "HelloPlugin";
    auto sent = wrapper.sendTo(s1, msg, strlen(msg), 0, (sockaddr *) &addrSend, sizeof(addrSend));
    ASSERT_GT(sent, 0);

    char buffer[128] = {};
    sockaddr_in src{};
    socklen_t srcLen = sizeof(src);

    auto rec = wrapper.recvFrom(s2, buffer, sizeof(buffer), 0, (sockaddr *) &src, &srcLen);

    ASSERT_GT(rec, 0);
    EXPECT_STREQ(buffer, "HelloPlugin");

    wrapper.closeSocket(s1);
    wrapper.closeSocket(s2);
}
