/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPServer
*/

#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>

#include "AServer.hpp"
#include "NetWrapper.hpp"
#include "TCPPacket.hpp"

namespace Net::Server
{
    class TCPServer final : public AServer {
      public:
        TCPServer(std::shared_ptr<NetWrapper> net, std::shared_ptr<IPacket> packetPrototype);

        ~TCPServer() override
        {
            stop();
        }

        void start() override;
        void stop() override;

        void setNonBlocking(bool nonBlocking) override;

        void readPackets() override;
        bool sendPacket(const IPacket &pkt) override;

        bool popPacket(std::shared_ptr<IPacket> &pkt) override;

      private:
        struct EndpointKey {
            uint32_t ip;
            uint16_t port;

            bool operator==(const EndpointKey &o) const noexcept
            {
                return ip == o.ip && port == o.port;
            }
        };

        struct EndpointHash {
            size_t operator()(const EndpointKey &k) const noexcept
            {
                return (static_cast<size_t>(k.ip) << 16) ^ static_cast<size_t>(k.port);
            }
        };

        static EndpointKey keyFrom_(const sockaddr_in &a) noexcept
        {
            return {a.sin_addr.s_addr, a.sin_port};
        }

        static uint32_t readU32BE(const uint8_t *p);
        static void writeU32BE_(uint8_t *p, uint32_t v);

        void acceptLoop();
        void readClients();
        void dropClient_(socketHandle s, const sockaddr_in &addr);

        bool sendAll(socketHandle s, const uint8_t *data, size_t len) const;

      private:
        std::shared_ptr<NetWrapper> _netWrapper;
        std::shared_ptr<IPacket> _packetProto;

        bool _nonBlocking = true;

        socketHandle _listenSock = kInvalidSocket;

        struct ClientState {
            sockaddr_in addr{};
            std::vector<uint8_t> rx;
        };

        mutable std::mutex _mutex;
        std::unordered_map<socketHandle, ClientState> _clients;
        std::unordered_map<EndpointKey, socketHandle, EndpointHash> _endpointToSock;

        mutable std::mutex _queueMutex;
        std::queue<std::shared_ptr<IPacket>> _queue;

        static constexpr uint32_t MaxFrame = 64 * 1024;
    };
} // namespace Net::Server
