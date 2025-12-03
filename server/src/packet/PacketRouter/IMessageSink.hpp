/*
** EPITECH PROJECT, 2025
** RType
** File description:
** IMessageSink
*/

#pragma once
#include <cstdint>
#include <iostream>
#include "NetMessages.hpp"

/**
 * @brief Interface for handling incoming network messages.
 */
class IMessageSink {
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IMessageSink() = default;

    /**
     * @brief Called when a player connects.
     * @param sessionId The ID of the connected player.
     */
    virtual void onPlayerConnect(int sessionId) = 0;

    /**
     * @brief Called when a player disconnects.
     * @param sessionId The ID of the disconnected player.
     */
    virtual void onPlayerDisconnect(int sessionId) = 0;

    /**
     * @brief Called when a player sends input.
     * @param sessionId The ID of the player.
     * @param msg The player input message.
     */
    virtual void onPlayerInput(int sessionId, const PlayerInputMessage &msg) = 0;

    /**
     * @brief Called when a ping message is received.
     * @param sessionId The ID of the player.
     * @param timestamp The timestamp of the ping.
     */
    virtual void onPing(int sessionId, std::uint64_t timestamp) = 0;
};

class tempMessageSink : public IMessageSink {
  public:
    virtual ~tempMessageSink() = default;

    void onPlayerConnect(int sessionId) override
    {
        (void) sessionId;
        std::cout << "Player connected: " << sessionId << std::endl;
    }

    void onPlayerDisconnect(int sessionId) override
    {
        (void) sessionId;
        std::cout << "Player disconnected: " << sessionId << std::endl;
    }

    void onPlayerInput(int sessionId, const PlayerInputMessage &msg) override
    {
        (void) sessionId;
        (void) msg;
        std::cout << "Player input: " << sessionId << std::endl;
    }

    void onPing(int sessionId, std::uint64_t timestamp) override
    {
        (void) sessionId;
        (void) timestamp;
        std::cout << "Ping received from: " << sessionId << " at " << timestamp << std::endl;
    }
};
