/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LobbyState
*/

#pragma once

#include <memory>
#include "AuthContext.hpp"
#include "IGameState.hpp"
#include "IGraphics.hpp"
#include "IRenderer.hpp"
#include "Lobby.hpp"
#include "MenuState.hpp"
#include "MusicRegistry.hpp"
#include "RoomManager.hpp"
#include "RoomMenu.hpp"
#include "ScoreboardContext.hpp"
#include "SoundRegistry.hpp"

namespace Engine
{
    /**
     * @class LobbyState
     * @brief Represents the lobby state of the game.
     * @details This state manages the room lobby where players can wait before starting the game.
     */
    class LobbyState final : public IGameState {
      public:
        /**
         * @brief Constructs a LobbyState with the given graphics, renderer, and room manager.
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the graphics renderer.
         * @param musicRegistry Shared pointer to the music registry.
         * @param soundRegistry Shared pointer to the sound registry.
         * @param roomManager Shared pointer to the room manager.
         * @param eventBus Shared pointer to the event bus.
         * @param authCtx Shared pointer to the room manager.
         * @param scoreCtx Shared pointer to the scoreboard context.
         */
        LobbyState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
            std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry,
            std::shared_ptr<RoomManager> roomManager, std::shared_ptr<EventBus> eventBus,
            std::shared_ptr<AuthContext> authCtx, std::shared_ptr<ScoreboardContext> scoreCtx);

        /**
         * @brief Called when entering the state.
         */
        void onEnter() override;

        /**
         * @brief Updates the room state based on the input frame.
         * @param manager Reference to the state manager.
         * @param frame The last input frame.
         */
        void update(StateManager &manager, const InputFrame &frame) override;

        /**
         * @brief Renders the room lobby.
         */
        void render() override;

      private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Shared pointer to the graphics interface.
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the graphics renderer.
        std::shared_ptr<MusicRegistry> _musicRegistry;  ///> Shared pointer to the music registry.
        std::shared_ptr<SoundRegistry> _soundRegistry;  ///> Shared pointer to the sound registry.

        std::shared_ptr<RoomManager> _roomManager;    ///> Shared pointer to the room manager.
        std::unique_ptr<RoomMenu> _lobby;             ///> Unique pointer to the room lobby.
        std::unique_ptr<LobbyWaitPage> _lobbyPage;    ///> Unique pointer to the lobby page.
        std::shared_ptr<EventBus> _eventBus;          ///> Shared pointer to the event bus.
        std::shared_ptr<AuthContext> _authCtx;        ///> Shared pointer to the auth context.
        std::shared_ptr<ScoreboardContext> _scoreCtx; ///> Shared pointer to the scoreboard context.
    };
} // namespace Engine
