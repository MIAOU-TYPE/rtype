/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPTypesData
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
namespace Net::Protocol::UDP
{
    /**
     * @brief Packet types for TCP communication from client to server.
     */
    constexpr uint8_t HELLO = 0x01;       ///> Client greets the server
    constexpr uint8_t LOGIN = 0x02;       ///> Client requests to log in
    constexpr uint8_t LIST_ROOMS = 0x03;  ///> Client requests the list of available rooms
    constexpr uint8_t CREATE_ROOM = 0x04; ///> Client requests to create a new room
    constexpr uint8_t JOIN_ROOM = 0x05;   ///> Client requests to join an existing room
    constexpr uint8_t LEAVE_ROOM = 0x06;  ///> Client requests to leave the current room
    constexpr uint8_t READY = 0x07;       ///> Client indicates readiness to start the game
    constexpr uint8_t START_GAME = 0x08;  ///> Client requests to start the game
    constexpr uint8_t DISCONNECT = 0x0A;  ///> Client notifies server of disconnection

    /**
     * @brief Packet types for TCP communication from server to client.
     */
    constexpr uint8_t WELCOME = 0x10;      ///> Server welcomes the client
    constexpr uint8_t ERROR = 0x11;        ///> Server notifies client of an error
    constexpr uint8_t ROOMS_LIST = 0x12;   ///> Server sends the list of available rooms
    constexpr uint8_t ROOM_CREATED = 0x13; ///> Server confirms room creation
    constexpr uint8_t ROOM_JOINED = 0x14;  ///> Server confirms joining a room
    constexpr uint8_t ROOM_LEFT = 0x15;    ///> Server confirms leaving a room
    constexpr uint8_t ROOM_UPDATE = 0x16;  ///> Server sends an update about the room
    constexpr uint8_t GAME_START = 0x17;   ///> Server notifies clients that the game is starting
} // namespace Net::Protocol::UDP