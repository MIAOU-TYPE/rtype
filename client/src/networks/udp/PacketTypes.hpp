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
        CONNECT,        ///< Client -> Server : Request to connect
        INPUT,          ///< Client -> Server : Send player input
        PING,           ///< Client -> Server : Ping request
        DISCONNECT,     ///< Client -> Server : Notify disconnection
        ACCEPT,         ///< Server -> Client : Accept connection
        REJECT,         ///< Server -> Client : Reject connection
        SNAPSHOT,       ///< Server -> Client : Send game state
        ENTITY_CREATE,  ///< Server -> Client : Notify entity creation
        ENTITY_DESTROY, ///< Server -> Client : Notify entity destruction
        PONG,           ///< Server -> Client : Pong response
        DAMAGE_EVENT,   ///< Server -> Client : Notify damage event
        GAME_OVER,      ///< Server -> Client : Notify game over
    };

} // namespace Network