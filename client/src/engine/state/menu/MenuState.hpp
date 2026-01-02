/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MenuState
*/

#pragma once

#include "EventRegistry.hpp"
#include "IGameState.hpp"
#include "IGraphics.hpp"
#include "IRenderer.hpp"
#include "Menu.hpp"
#include "MusicRegistry.hpp"
#include "SettingsState.hpp"
#include "SoundRegistry.hpp"
#include "StateManager.hpp"

namespace Engine
{
    /**
     * @brief Represents the menu state of the game.
     */
    class MenuState : public IGameState {
      public:
        /**
         * @brief Construct a new Menu State object.
         *
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the renderer interface.
         * @param musicRegistry Shared pointer to the music registry.
         * @param soundRegistry Shared pointer to the sound registry.
         */
        explicit MenuState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
            std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry);

        /**
         * @brief Destroy the Menu State object.
         */
        void onEnter(StateManager &manager) override;

        /**
         * @brief Update the menu state.
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame) override;

        /**
         * @brief Render the menu state.
         */
        void render() override;

      private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Shared pointer to the graphics interface.
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the renderer interface.
        std::shared_ptr<MusicRegistry> _musicRegistry;  ///> Shared pointer to the music registry.
        std::shared_ptr<SoundRegistry> _soundRegistry;  ///> Shared pointer to the sound registry.

        std::unique_ptr<Menu> _menu;                                     ///> Unique pointer to the menu.
        StateManager *_manager = nullptr;                                ///> Pointer to the state manager.
        Graphics::AudioHandle _menuMusicHandle = Graphics::InvalidAudio; ///> Handle to the menu music.
    };
} // namespace Engine
