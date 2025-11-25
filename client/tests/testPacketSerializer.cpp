#include <gtest/gtest.h>
#include "PacketSerializer.hpp"
#include "ProtocolDefinitions.hpp"

TEST(PacketSerializerTest, SerializeConnectPacket) {
    Network::PacketSerializer serializer;
    uint32_t clientId = 12345;
    auto packet = serializer.serializeConnectPacket(clientId);

    ASSERT_EQ(packet.size(), sizeof(Network::PacketConnect));
    EXPECT_EQ(static_cast<Network::PacketType>(packet[0]), Network::PacketType::CONNECT);
    
    EXPECT_EQ(packet[1], 1);
    uint16_t packetSize;
    std::memcpy(&packetSize, &packet[2], sizeof(uint16_t));
    EXPECT_EQ(ntohs(packetSize), sizeof(Network::PacketConnect));
    
    uint32_t serializedClientId;
    std::memcpy(&serializedClientId, &packet[4], sizeof(uint32_t));
    EXPECT_EQ(ntohl(serializedClientId), clientId);
}

TEST(PacketSerializerTest, SerializeInputPacket) {
    Network::PacketSerializer serializer;
    uint32_t entity = 1;
    float dx = 0.5f;
    float dy = -0.5f;
    uint8_t shooting = 1;
    auto packet = serializer.serializeInputPacket(entity, dx, dy, shooting);

    ASSERT_EQ(packet.size(), sizeof(Network::PacketInput));
    EXPECT_EQ(static_cast<Network::PacketType>(packet[0]), Network::PacketType::INPUT);
    
    EXPECT_EQ(packet[1], 1);
    uint16_t packetSize;
    std::memcpy(&packetSize, &packet[2], sizeof(uint16_t));
    EXPECT_EQ(ntohs(packetSize), sizeof(Network::PacketInput));
    
    uint32_t serializedEntity;
    std::memcpy(&serializedEntity, &packet[4], sizeof(uint32_t));
    EXPECT_EQ(ntohl(serializedEntity), entity);

    float serializedDx, serializedDy;
    std::memcpy(&serializedDx, &packet[8], sizeof(float));
    std::memcpy(&serializedDy, &packet[12], sizeof(float));
    EXPECT_FLOAT_EQ(serializedDx, dx);
    EXPECT_FLOAT_EQ(serializedDy, dy);

    EXPECT_EQ(packet[16], shooting);
}