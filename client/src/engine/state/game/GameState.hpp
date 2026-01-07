/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameState
*/

#pragma once

#include "IGameState.hpp"
#include "IGraphics.hpp"
#include "IRenderer.hpp"
#include "Menu.hpp"
#include "MusicRegistry.hpp"
#include "SoundRegistry.hpp"

namespace Engine
{
    /**
     * @brief Represents the Game state of the game.
     */
    class GameState : public IGameState {
      public:
        /**
         * @brief Construct a new Game State object.
         * @param musicRegistry Shared pointer to the music registry.
         * @param soundRegistry Shared pointer to the sound registry.
         */
        explicit GameState(std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry);

        /**
         * @brief Called when entering the state.
         */
        void onEnter() override;

        /**
         * @brief Update the Game state.
         * @param manager Reference to the state manager.
         * @param frame The current input frame.
         */
        void update(StateManager &manager, const InputFrame &frame) override;

        /**
         * @brief Render the Game state.
         */
        void render() override;

      private:
        std::shared_ptr<MusicRegistry> _musicRegistry; ///> Shared pointer to the music registry.
        std::shared_ptr<SoundRegistry> _soundRegistry; ///> Shared pointer to the sound registry.
    };
} // namespace Engine
