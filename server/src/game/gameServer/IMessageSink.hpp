/*
** EPITECH PROJECT, 2025
** RType
** File description:
** IMessageSink
*/

#pragma once
#include <cstdint>
#include <iostream>
#include "InputComponent.hpp"

namespace Net
{
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
        virtual void onPlayerInput(int sessionId, const Game::InputComponent &msg) = 0;

        /**
         * @brief Called when a ping message is received.
         * @param sessionId The ID of the player.
         */
        virtual void onPing(int sessionId) = 0;
    };
} // namespace Net