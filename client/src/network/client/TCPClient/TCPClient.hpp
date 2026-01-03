/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** TCPClient
*/

#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <vector>

#include "ANetClient.hpp"
#include "IPacket.hpp"
#include "NetWrapper.hpp"

namespace Network
{
    class TCPClientError final : public std::exception {
      public:
        explicit TCPClientError(const std::string &message) : _message("\n\t" + message)
        {
        }

        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message = "";
    };

    class TCPClient final : public ANetClient {
      public:
        explicit TCPClient(std::shared_ptr<Net::NetWrapper> net = std::make_shared<Net::NetWrapper>("NetPluginLib"));
        ~TCPClient() override;

        void start() override;
        void close() override;

        void setNonBlocking(bool nonBlocking) override;

        void receivePackets() override;
        bool sendPacket(const Net::IPacket &pkt) override;

        bool popPacket(std::shared_ptr<Net::IPacket> &pkt) override;
        std::shared_ptr<Net::IPacket> getTemplatedPacket() const noexcept override;

      private:
        static bool wouldBlock() noexcept;

        void flushWrites() noexcept;
        void parseFrames() noexcept;
        void resetState() noexcept;

      private:
        std::shared_ptr<Net::NetWrapper> _netWrapper;
        std::shared_ptr<Net::IPacket> _packetProto;

        sockaddr_in _serverAddr{};
        bool _nonBlocking = true;

        // RX framing
        std::vector<std::uint8_t> _rx;
        std::size_t _rxPos = 0;

        // TX buffering (non-bloquant)
        std::mutex _txMutex;
        std::vector<std::uint8_t> _tx;
        std::size_t _txPos = 0;

        // incoming packets queue
        std::mutex _queueMutex;
        std::queue<std::shared_ptr<Net::IPacket>> _queue;

        static constexpr std::uint32_t MAX_FRAME = 64 * 1024;
    };
} // namespace Network
