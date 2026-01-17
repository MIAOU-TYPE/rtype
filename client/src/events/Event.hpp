/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Event
*/

#pragma once
#include <string>
#include <utility>
#include "GameConfig.hpp"
#include "Key.hpp"

namespace Engine
{
    /**
     * @brief Base class for all events.
     */
    struct Event {
        /**
         * @brief Virtual destructor for Event.
         */
        virtual ~Event() = default;
    };

    /**
     * @brief Event triggered when the window is closed.
     */
    struct WindowClosed : Event {};

    /**
     * @brief Event triggered when a key is pressed.
     */
    struct KeyPressed : Event {
        /**
         * @brief Constructor for KeyPressed event.
         * @param k The key that was pressed.
         */
        explicit KeyPressed(const Key k) : key(k)
        {
        }

        Key key; ///> The key that was pressed.
    };

    /**
     * @brief Event triggered when a key is released.
     */
    struct KeyReleased : Event {
        /**
         * @brief Constructor for KeyReleased event.
         * @param k The key that was released.
         */
        explicit KeyReleased(const Key k) : key(k)
        {
        }

        Key key; ///> The key that was released.
    };

    /**
     * @brief Event triggered when the mouse is moved.
     */
    struct MouseMoved : Event {
        /**
         * @brief Constructor for MouseMoved event.
         * @param x The x position of the mouse.
         * @param y The y position of the mouse.
         */
        explicit MouseMoved(int x, int y) : posX(x), posY(y)
        {
        }

        int posX; ///> The x position of the mouse.
        int posY; ///> The y position of the mouse.
    };

    /**
     * @brief Event triggered when a mouse button is pressed.
     */
    struct MousePressed : Event {
        /**
         * @brief Constructor for MousePressed event.
         * @param k The mouse button that was pressed.
         * @param x The x position of the mouse when pressed.
         * @param y The y position of the mouse when pressed.
         */
        explicit MousePressed(const Key k, const int x, const int y) : key(k), posX(x), posY(y)
        {
        }

        Key key;      ///> The mouse button that was pressed.
        int posX = 0; ///> The x position of the mouse when pressed.
        int posY = 0; ///> The y position of the mouse when
    };

    /**
     * @brief Event triggered when a mouse button is released.
     */
    struct MouseReleased : Event {
        /**
         * @brief Constructor for MouseReleased event.
         * @param k The mouse button that was released.
         * @param x The x position of the mouse when released.
         * @param y The y position of the mouse when released.
         */
        explicit MouseReleased(const Key k, const int x, const int y) : key(k), posX(x), posY(y)
        {
        }

        Key key;      ///> The mouse button that was released.
        int posX = 0; ///> The x position of the mouse when released.
        int posY = 0; ///> The y position of the mouse when released.
    };

    /**
     * @brief Event triggered when the window is resized.
     */
    struct WindowResized {
        uint32_t width;  ///> The new width of the window.
        uint32_t height; ///> The new height of the window.
    };

    /**
     * @brief Event triggered when a request to create a room is made.
     */
    struct CreateRoomRequested : Event {
        /**
         * @brief Constructor for CreateRoomRequested event.
         * @param name The name of the new room.
         * @param maxP The maximum number of players allowed in the room.
         * @param diff The difficulty level for the room.
         * @param level The path to the level configuration file.
         * @param music The path to the background music for the world.
         */
        explicit CreateRoomRequested(std::string name, const uint8_t maxP, Difficulty diff, std::string level,
            std::string music = ::DEFAULT_GAME_MUSIC)
            : roomName(std::move(name)), maxPlayers(maxP), difficulty(diff), levelPath(std::move(level)),
              worldMusic(std::move(music))
        {
        }

        std::string roomName;  ///> The name of the new room.
        uint8_t maxPlayers;    ///> The maximum number of players allowed in the room.
        Difficulty difficulty; ///> The difficulty level for the room.
        std::string levelPath; ///> The path to the level configuration file.
        std::string worldMusic; ///> The path to the background music for the world.
    };

    /**
     * @brief Event triggered when a request to join a room is made.
     */
    struct JoinRoomRequested : Event {
        /**
         * @brief Constructor for JoinRoomRequested event.
         * @param id The ID of the room to join.
         */
        explicit JoinRoomRequested(const uint32_t id) : roomId(id)
        {
        }

        uint32_t roomId; ///> The ID of the room to join.
    };

    /**
     * @brief Event triggered when a request to list available rooms is made.
     */
    struct ListRoomRequested : Event {
        /**
         * @brief Constructor for ListRoomRequested event.
         */
        ListRoomRequested() = default;
    };

    /**
     * @brief Event triggered when a request to register a new user is made.
     */
    struct AuthRegisterRequested : Event {
        /**
         * @brief Constructor for AuthRegisterRequested event.
         * @param u The username for registration.
         * @param p The password for registration.
         */
        explicit AuthRegisterRequested(std::string u, std::string p) : username(std::move(u)), password(std::move(p))
        {
        }

        std::string username; ///> The username for registration.
        std::string password; ///> The password for registration.
    };

    /**
     * @brief Event triggered when a request to login is made.
     */
    struct AuthLoginRequested : Event {
        /**
         * @brief Constructor for AuthLoginRequested event.
         * @param u The username for login.
         * @param p The password for login.
         */
        explicit AuthLoginRequested(std::string u, std::string p) : username(std::move(u)), password(std::move(p))
        {
        }

        std::string username; ///> The username for login.
        std::string password; ///> The password for login.
    };

    /**
     * @brief Event triggered when a request to submit a score is made.
     */
    struct ScoreboardGetRequested : Event {
        /**
         * @brief Constructor for ScoreboardGetRequested event.
         * @param limit_ The maximum number of scores to retrieve.
         */
        explicit ScoreboardGetRequested(const size_t limit_ = 10) : limit(limit_)
        {
        }

        size_t limit; ///> The maximum number of scores to retrieve.
    };

    /**
     * @brief Event triggered when a ping is sent.
     */
    struct PongReceived : Event {
        /**
         * @brief Constructor for PongReceived event.
         * @param tmstmp The timestamp of the pong received.
         */
        explicit PongReceived(const uint32_t tmstmp) : timestamp(tmstmp)
        {
        }

        uint32_t timestamp; ///> The timestamp of the pong received.
    };

    /**
     * @brief Event triggered when a request to start the game is made.
     */
    struct StartGameRequested : Event {};

    /**
     * @brief Event triggered when a request to leave the room is made.
     */
    struct LeaveRoomRequested : Event {};

    /**
     * @brief Event triggered when a request to update the room is made.
     */
    struct UpdateRoomRequested : Event {};
} // namespace Engine
