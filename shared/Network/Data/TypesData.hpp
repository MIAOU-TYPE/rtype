/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PacketTypes
*/

#pragma once
#include <cstdint>

/**
 * @brief Namespace for network-related definitions and functionalities.
 * @details This namespace encapsulates all components related to network communication,
 * including packet types, protocols, and utilities.
 */
/**
 * @brief Namespace for protocol-related definitions and functionalities.
 * @details This namespace contains definitions and enumerations related to
 * the network protocol used for communication between client and server.
 */
namespace Net::Protocol
{
    /**
     * @brief Enumeration of UDP packet types.
     * @details This enumeration defines the various types of UDP packets
     * used in the network communication between client and server.
     */

    /**
     * @brief Packet from client to server.
     */
    constexpr uint8_t CONNECT = 0x01;    ///> Client requests to connect to the server
    constexpr uint8_t DISCONNECT = 0x02; ///> Client notifies server of disconnection
    constexpr uint8_t INPUT = 0x03;      ///> Client sends input commands to the server
    constexpr uint8_t PING = 0x04;       ///> Client sends a ping to check server latency

    /**
     * @brief Packet from server to client.
     */
    constexpr uint8_t ACCEPT = 0x10;       ///> Server accepts the client's connection request
    constexpr uint8_t REJECT = 0x11;       ///> Server rejects the client's connection request
    constexpr uint8_t SNAPSHOT = 0x12;     ///> Server sends a game state snapshot to the client
    constexpr uint8_t PONG = 0x13;         ///> Server responds to client's ping
    constexpr uint8_t DAMAGE_EVENT = 0x14; ///> Server notifies client of a damage event
    constexpr uint8_t GAME_OVER = 0x15;    ///> Server notifies client of game over event

} // namespace Net::Protocol