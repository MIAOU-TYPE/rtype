#include <gtest/gtest.h>
#include "UdpClient.hpp"

TEST(UdpClientTest, ConnectDisconnect)
{
    Network::UdpClient client;

    EXPECT_NO_THROW(client.connectToServer("127.0.0.1", 12345));
    EXPECT_NO_THROW(client.disconnectFromServer());
}

TEST(UdpClientTest, IsConnected)
{
    Network::UdpClient client;
    EXPECT_FALSE(client.isConnected());
    client.connectToServer("127.0.0.1", 12345);
    EXPECT_TRUE(client.isConnected());
    client.disconnectFromServer();
    EXPECT_FALSE(client.isConnected());
}