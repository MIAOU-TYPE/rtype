/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** udp_client
*/

#ifndef _WIN32
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include     <unistd.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #define ssize_t SSIZE_T
    #define close closesocket
    #define sleep(x) Sleep((x) * 1000)

#endif
#include <cstring>
#include <iostream>
#include <array>

#include "DefaultData.hpp"
#include "Endian.hpp"
#include "SnapEntityData.hpp"
#include "TypesData.hpp"



// ------------------------------------------------------------
// Configuration
// ------------------------------------------------------------
static constexpr const char *SERVER_IP = "127.0.0.1";
static constexpr uint16_t SERVER_PORT = 8080;

// Example protocol values (adapt to your enum)
static constexpr uint8_t PROTOCOL_VERSION = 1;
static constexpr uint8_t PACKET_TYPE_CONNECT = 1;

int main()
{
    #ifdef _WIN32
WSADATA wsa;
if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    std::cerr << "WSAStartup failed\n";
    return 1;
}
#endif

    // --------------------------------------------------------
    // Create UDP socket
    // --------------------------------------------------------
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // --------------------------------------------------------
    // Server address
    // --------------------------------------------------------
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    // --------------------------------------------------------
    // Build packet
    // --------------------------------------------------------
    DefaultData packet;
    packet.header.type = Net::Protocol::CONNECT;
    packet.header.version = PROTOCOL_VERSION;
    packet.header.size = htons(sizeof(DefaultData));

    // --------------------------------------------------------
    // Send packet
    // --------------------------------------------------------
    #ifndef _WIN32
        ssize_t sent =
            sendto(sock, &packet, sizeof(packet), 0, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
    #else
        ssize_t sent =
            sendto(sock, reinterpret_cast<const char *>(&packet), sizeof(packet), 0,
                   reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
    #endif
    if (sent < 0) {
        perror("sendto");
        close(sock);
        return 1;
    }

    constexpr size_t MAX_PACKET_SIZE = 4096;
    std::array<uint8_t, MAX_PACKET_SIZE> buffer{};

    while (1) {
        sockaddr_in fromAddr{};
        socklen_t fromAddrLen = sizeof(fromAddr);
        #ifndef _WIN32
        ssize_t received =
            recvfrom(sock, buffer.data(), buffer.size(), 0, reinterpret_cast<sockaddr *>(&fromAddr), &fromAddrLen);
        #else
        ssize_t received =
            recvfrom(sock, reinterpret_cast<char *>(buffer.data()), buffer.size(), 0,
                        reinterpret_cast<sockaddr *>(&fromAddr), &fromAddrLen);
        #endif
        if (received < 0) {
            perror("recvfrom");
            close(sock);
            return 1;
        }

        DefaultData &packet = *reinterpret_cast<DefaultData *>(buffer.data());

        if (packet.header.type == Net::Protocol::SNAPSHOT) {
            auto *batch = reinterpret_cast<const SnapshotBatchHeader *>(buffer.data());
            std::cout << "\n[SNAPSHOT]\n";
            std::cout << "  packet size : " << ntohs(batch->header.size) << "\n";
            std::cout << "  entity count: " << ntohs(batch->count) << "\n";
            auto *entities = reinterpret_cast<const SnapshotEntityData *>(buffer.data() + sizeof(SnapshotBatchHeader));
            const uint16_t count = ntohs(batch->count);

            for (uint16_t i = 0; i < count; ++i) {
                const SnapshotEntityData &raw = entities[i];

                uint64_t entityId = ntohll(raw.entity);
                float x = ntohf(raw.x);
                float y = ntohf(raw.y);
                uint8_t sprite = raw.spriteId;

                std::cout << "Entity[" << i << "] "
                          << "id=" << entityId << " pos=(" << x << "," << y << ")"
                          << " sprite=" << static_cast<int>(sprite) << "\n";
            }

        } else {
            switch (packet.header.type) {
                case Net::Protocol::ACCEPT: std::cout << "[Client] Received ACCEPT packet from server.\n"; break;
                case Net::Protocol::REJECT:
                    std::cout << "[Client] Received REJECT packet from server.\n";
                    exit(0);
                    break;
                case Net::Protocol::ENTITY_CREATE:
                    std::cout << "[Client] Received ENTITY_CREATE packet from server.\n";
                    break;
                case Net::Protocol::ENTITY_DESTROY:
                    std::cout << "[Client] Received ENTITY_DESTROY packet from server.\n";
                    break;
                case Net::Protocol::PONG: std::cout << "[Client] Received PONG packet from server.\n"; break;
                case Net::Protocol::DAMAGE_EVENT:
                    std::cout << "[Client] Received DAMAGE_EVENT packet from server.\n";
                    break;
                case Net::Protocol::GAME_OVER: std::cout << "[Client] Received GAME_OVER packet from server.\n"; break;
                default: std::cout << "[Client] Received unknown packet type.\n"; exit(0);
            }
        }
    }

    std::cout << "[Client] Sent DefaultData packet (" << sent << " bytes)\n";

    sleep(1);
    close(sock);

    return 0;
}
