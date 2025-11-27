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
     * @brief Enumeration of all network packet types
     *
     * Defines the different types of packets that can be exchanged
     * between client and server in the R-Type game protocol.
     */
    enum class PacketType : uint8_t {
        CONNECT = 0,        ///< Client -> Server : Request to connect
        INPUT = 1,          ///< Client -> Server : Send player input
        PING = 2,           ///< Client -> Server : Ping request
        DISCONNECT = 3,     ///< Client -> Server : Notify disconnection
        ACCEPT = 4,         ///< Server -> Client : Accept connection
        REJECT = 5,         ///< Server -> Client : Reject connection
        SNAPSHOT = 6,       ///< Server -> Client : Send game state
        ENTITY_CREATE = 7,  ///< Server -> Client : Notify entity creation
        ENTITY_DESTROY = 8, ///< Server -> Client : Notify entity destruction
        PONG = 9,           ///< Server -> Client : Pong response
        DAMAGE_EVENT = 10,  ///< Server -> Client : Notify damage event
        GAME_OVER = 11,     ///< Server -> Client : Notify game over
    };

} // namespace Network