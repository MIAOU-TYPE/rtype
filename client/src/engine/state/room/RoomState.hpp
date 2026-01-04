/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomState
*/

#pragma once

#include <memory>

#include "IGameState.hpp"
#include "IGraphics.hpp"
#include "IRenderer.hpp"
#include "MenuState.hpp"
#include "MusicRegistry.hpp"
#include "RoomManager.hpp"
#include "RoomMenu.hpp"
#include "SoundRegistry.hpp"

namespace Engine
{
    /**
     * @class RoomState
     * @brief Game state for managing the room menu.
     * This state handles the room menu where players can create or join game rooms.
     */
    class RoomState final : public IGameState {
      public:
        /**
         * @brief Constructs a RoomState with the given graphics, renderer, and room manager.
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the graphics renderer.
         * @param musicRegistry Shared pointer to the music registry.
         * @param soundRegistry Shared pointer to the sound registry.
         * @param roomManager Shared pointer to the room manager.
         */
        RoomState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
            std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry,
            std::shared_ptr<RoomManager> roomManager);

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
         * @brief Renders the room menu.
         */
        void render() override;

      private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Shared pointer to the graphics interface.
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the graphics renderer.
        std::shared_ptr<MusicRegistry> _musicRegistry;  ///> Shared pointer to the music registry.
        std::shared_ptr<SoundRegistry> _soundRegistry;  ///> Shared pointer to the sound registry.

        std::shared_ptr<RoomManager> _roomManager; ///> Shared pointer to the room manager.
        std::unique_ptr<RoomMenu> _menu;           ///> Unique pointer to the room menu.
    };
} // namespace Engine
