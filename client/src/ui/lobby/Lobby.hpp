/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby
*/

#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "AMenu.hpp"
#include "IRenderer.hpp"
#include "IText.hpp"
#include "InputState.hpp"
#include "UIButton.hpp"

namespace Engine
{
    /**
     * @class LobbyWaitPage
     * @brief Represents the lobby waiting page UI.
     * @details This class manages the UI elements and interactions for the lobby waiting page,
     * allowing players to see the lobby status and start the game.
     */
    class LobbyWaitPage final : public AMenu {
      public:
        /**
         * @brief Constructs a LobbyWaitPage with the given renderer.
         * @param renderer Shared pointer to the graphics renderer.
         */
        explicit LobbyWaitPage(const std::shared_ptr<Graphics::IRenderer> &renderer);

        /**
         * @brief Called when entering the lobby page.
         */
        void onEnter();

        /**
         * @brief Updates the lobby page based on the input frame.
         * @param frame The last input frame.
         */
        void update(const InputFrame &frame) override;

        /**
         * @brief Renders the lobby page.
         */
        void render() const override;

        /**
         * @brief Lays out the UI elements of the lobby page.
         */
        void layout() override;

        /**
         * @brief Sets the name of the lobby.
         * @param name The name of the lobby.
         */
        void setLobbyName(std::string name);

        /**
         * @brief Sets the list of players in the lobby.
         * @param players A vector of player names.
         */
        void setPlayers(std::vector<std::string> players);

        /**
         * @brief Enables or disables the start button.
         * @param v True to enable the start button, false to disable it.
         */
        void setStartEnabled(bool v);

        /**
         * @brief Sets the maximum number of players allowed in the lobby.
         * @param maxPlayers The maximum number of players.
         */
        void setMaxPlayers(size_t maxPlayers);

        /**
         * @brief Checks if the start button has been requested.
         * @return True if the start button was requested, false otherwise.
         */
        [[nodiscard]] bool wantsStart() const noexcept;

        /**
         * @brief Consumes the start request.
         */
        void consumeStart() noexcept;

      private:
        /**
         * @brief Handles input events for the lobby page.
         * @param frame The last input frame.
         */
        void handleInput(const InputFrame &frame);

        /**
         * @brief Handles mouse press events.
         * @param frame The last input frame.
         */
        void handleMousePressed(const InputFrame &frame) const;

        /**
         * @brief Handles mouse release events.
         * @param frame The last input frame.
         */
        void handleMouseReleased(const InputFrame &frame);

        /**
         * @brief Handles key press events.
         * @param frame The last input frame.
         */
        void handleKeyPressed(const InputFrame &frame);

        /**
         * @brief Rebuilds the text elements based on the current state.
         */
        void rebuildTexts() const;

        std::unique_ptr<UI::UIButton> _startBtn; ///> Start button UI element.

        std::shared_ptr<Graphics::IText> _titleText;    ///> Title text element.
        std::shared_ptr<Graphics::IText> _subtitleText; ///> Subtitle text element.
        std::shared_ptr<Graphics::IText> _dividerText;  ///> Divider text element.

        std::shared_ptr<Graphics::IText> _playersHeaderText;        ///> Players header text element.
        std::shared_ptr<Graphics::IText> _playersCountText;         ///> Players count text element.
        std::vector<std::shared_ptr<Graphics::IText>> _playerTexts; ///> Player name text elements.

        std::shared_ptr<Graphics::IText> _statusText; ///> Status text element.
        std::shared_ptr<Graphics::IText> _hintText;   ///> Hint text element.

        std::string _lobbyName;            ///> Name of the lobby.
        std::vector<std::string> _players; ///> List of player names.

        bool _canStart = true;        ///> Indicates if the start button is enabled.
        bool _startRequested = false; ///> Indicates if the start button was requested.

        size_t LobbyCapacity = 4; ///> Maximum capacity of the lobby.
    };
} // namespace Engine
