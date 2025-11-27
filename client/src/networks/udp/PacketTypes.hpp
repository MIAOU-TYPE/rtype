/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketTypes
*/

#pragma once

#include <cstdint>

namespace Network
{

    /**
     * @brief Enumeration of UDP packet types.
     * @details This enumeration defines the various types of UDP packets
     * used in the network communication between client and server.
     */
    constexpr uint8_t CONNECT = 0x01;
    constexpr uint8_t DISCONNECT = 0x02;
    constexpr uint8_t INPUT = 0x03;
    constexpr uint8_t PING = 0x04;
    constexpr uint8_t ACCEPT = 0x10;
    constexpr uint8_t REJECT = 0x11;
    constexpr uint8_t SNAPSHOT = 0x12;
    constexpr uint8_t ENTITY_CREATE = 0x13;
    constexpr uint8_t ENTITY_DESTROY = 0x14;
    constexpr uint8_t PONG = 0x15;
    constexpr uint8_t DAMAGE_EVENT = 0x16;
    constexpr uint8_t GAME_OVER = 0x17;

} // namespace Network