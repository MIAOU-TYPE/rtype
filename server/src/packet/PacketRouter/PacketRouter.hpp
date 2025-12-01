/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketRouter
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "HeaderPacket.hpp"
#include "IMessageSink.hpp"
#include "IServerPacket.hpp"
#include "NetMessages.hpp"
#include "SessionManager.hpp"

class PacketRouter {
  public:
    PacketRouter(std::shared_ptr<SessionManager> &sessions, std::shared_ptr<IMessageSink> &sink);

    void handlePacket(const std::shared_ptr<Net::IServerPacket> &packet);

  private:
    bool validateHeader(const Net::IServerPacket &pkt, const HeaderPacket &header) const;

    void handleConnect(int sessionId);
    void handleInput(int sessionId, const std::uint8_t *payload, std::size_t payloadSize);
    void handlePing(int sessionId, const std::uint8_t *payload, std::size_t payloadSize);
    void handleDisconnect(int sessionId);

  private:
    std::shared_ptr<SessionManager> _sessions = nullptr;
    std::shared_ptr<IMessageSink> _sink = nullptr;

    static constexpr std::uint8_t PROTOCOL_VERSION = 1;
};
