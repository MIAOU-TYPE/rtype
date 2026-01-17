/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameOverState
*/

#pragma once
#include <memory>
#include "AuthContext.hpp"
#include "EventBus.hpp"
#include "GameOverMenu.hpp"
#include "IGameState.hpp"
#include "Event.hpp"
#include "IGraphics.hpp"
#include "IRenderer.hpp"
#include "MenuState.hpp"
#include "MusicRegistry.hpp"
#include "RoomManager.hpp"
#include "ScoreboardContext.hpp"
#include "SoundRegistry.hpp"

namespace Engine
{
    /**
     * @brief State representing the game over screen.
     */
    class GameOverState final : public IGameState {
    public:
        /**
         * @brief Construct a new Game Over State object.
         * @param graphics The graphics interface.
         * @param renderer The renderer interface.
         * @param musicRegistry The music registry.
         * @param soundRegistry The sound registry.
         * @param roomManager The room manager.
         * @param eventBus The event bus.
         * @param authCtx The authentication context.
         * @param scoreCtx The scoreboard context.
         */
        GameOverState(std::shared_ptr<Graphics::IGraphics> graphics,
            std::shared_ptr<Graphics::IRenderer> renderer,
            std::shared_ptr<MusicRegistry> musicRegistry,
            std::shared_ptr<SoundRegistry> soundRegistry,
            std::shared_ptr<RoomManager> roomManager,
            std::shared_ptr<EventBus> eventBus,
            std::shared_ptr<AuthContext> authCtx,
            std::shared_ptr<ScoreboardContext> scoreCtx,
            std::weak_ptr<World::ClientWorld> world);

        /**
         * @brief Called when the state is entered.
         */
        void onEnter() override;

        /**
         * @brief Update the state based on input.
         * @param manager The state manager.
         * @param frame The current input frame.
         */
        void update(StateManager &manager, const InputFrame &frame) override;

        /**
         * @brief Render the state.
         */
        void render() override;

    private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> The graphics interface.
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> The renderer interface.
        std::shared_ptr<MusicRegistry> _musicRegistry;  ///> The music registry.
        std::shared_ptr<SoundRegistry> _soundRegistry;  ///> The sound registry.
        std::shared_ptr<RoomManager> _roomManager;  ///> The room manager.
        std::shared_ptr<EventBus> _eventBus;    ///> The event bus.
        std::shared_ptr<AuthContext> _authCtx;  ///> The authentication context.
        std::shared_ptr<ScoreboardContext> _scoreCtx;   ///> The scoreboard context.
        std::unique_ptr<GameOverMenu> _menu;    ///> The game over menu.
        std::weak_ptr<World::ClientWorld> _world;   ///> Weak pointer to the client world.
    };
}
