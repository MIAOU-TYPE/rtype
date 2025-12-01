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

#include <iostream>
#include "HeaderPacket.hpp"
#include "IMessageSink.hpp"
#include "IServerPacket.hpp"
#include "NetMessages.hpp"
#include "PacketFactory.hpp"
#include "SessionManager.hpp"

#ifndef _WIN32
    #include <arpa/inet.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

/**
 * @class PacketRouter
 * @brief Routes incoming packets to appropriate handlers based on packet type.
 *
 * The PacketRouter class is responsible for processing incoming packets,
 * validating their headers, and routing them to the correct handler functions
 * based on the packet type. It interacts with a SessionManager to manage
 * player sessions and an IMessageSink to notify about various events such as
 * player connections, disconnections, inputs, and pings.
 */
class PacketRouter {
  public:
    /**
     * @brief Constructs a PacketRouter with the given SessionManager and IMessageSink.
     * @param sessions Shared pointer to the SessionManager for managing player sessions.
     * @param sink Shared pointer to the IMessageSink for handling routed messages.
     */
    PacketRouter(std::shared_ptr<SessionManager> &sessions, std::shared_ptr<IMessageSink> &sink);

    /**
     * @brief Handles an incoming packet by routing it to the appropriate handler.
     * @param packet Shared pointer to the incoming IServerPacket to be processed.
     */
    void handlePacket(const std::shared_ptr<Net::IServerPacket> &packet);

  private:
    /**
     * @brief Validates the header of an incoming packet.
     * @param pkt The incoming IServerPacket to validate.
     * @param header The HeaderPacket extracted from the incoming packet.
     * @return True if the header is valid, false otherwise.
     */
    bool validateHeader(const Net::IServerPacket &pkt, const HeaderPacket &header) const;

    /**
     * @brief Handler for player connection packets.
     * @param sessionId The ID of the connected player.
     */
    void handleConnect(int sessionId);

    /**
     * @brief Handler for player input packets.
     * @param sessionId The ID of the player.
     * @param payload Pointer to the payload data of the input packet.
     * @param payloadSize Size of the payload data.
     */
    void handleInput(int sessionId, const std::uint8_t *payload, std::size_t payloadSize);

    /**
     * @brief Handler for player ping packets.
     * @param sessionId The ID of the player.
     * @param payload Pointer to the payload data of the ping packet.
     * @param payloadSize Size of the payload data.
     */
    void handlePing(int sessionId, const std::uint8_t *payload, std::size_t payloadSize);

    /**
     * @brief Handler for player disconnection packets.
     * @param sessionId The ID of the disconnected player.
     */
    void handleDisconnect(int sessionId);

  private:
    std::shared_ptr<SessionManager> _sessions =
        nullptr;                                   ///> Pointer to the SessionManager for managing player sessions.
    std::shared_ptr<IMessageSink> _sink = nullptr; ///> Pointer to the IMessageSink for handling routed messages.

    static constexpr std::uint8_t PROTOCOL_VERSION = 1; ///> Expected protocol version for incoming packets.
};
