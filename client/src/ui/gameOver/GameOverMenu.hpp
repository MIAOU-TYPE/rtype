/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameOverMenu
*/

#pragma once

#include <memory>
#include "AMenu.hpp"
#include "ClientWorld.hpp"
#include "IRenderer.hpp"
#include "IText.hpp"
#include "InputState.hpp"
#include "UIButton.hpp"

namespace Engine
{
    /**
     * @brief Menu displayed when the player loses the game.
     */
    class GameOverMenu final : public AMenu {
      public:
        /**
         * @brief Construct a new Game Over Menu object.
         * @param renderer The renderer to use for drawing the menu.
         * @param world Weak pointer to the client world to retrieve scores.
         */
        explicit GameOverMenu(
            const std::shared_ptr<Graphics::IRenderer> &renderer, std::weak_ptr<World::ClientWorld> world);

        /**
         * @brief Called when the menu is entered.
         */
        void onEnter();

        /**
         * @brief Layout the menu elements.
         */
        void layout() override;

        /**
         * @brief Update the menu state based on input.
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame) override;

        /**
         * @brief Render the menu.
         */
        void render() const override;

        /**
         * @brief Check if the player requested to go back to the main menu.
         * @return true if back to menu was requested, false otherwise.
         */
        [[nodiscard]] bool wantsBackToMenu() const noexcept;

        /**
         * @brief Check if the player requested to quit the game.
         * @return true if quit was requested, false otherwise.
         */
        [[nodiscard]] bool wantsQuit() const noexcept;

      private:
        /**
         * @brief Handle mouse press events.
         * @param frame The current input frame.
         */
        void handleMousePressed(const InputFrame &frame) const;

        /**
         * @brief Handle mouse release events.
         * @param frame The current input frame.
         */
        void handleMouseReleased(const InputFrame &frame);

        std::unique_ptr<Graphics::IText> _title; ///> The "Game Over" title text.
        std::unique_ptr<UI::UIButton> _back;     ///> The "Back to Menu" button.
        std::unique_ptr<UI::UIButton> _quit;     ///> The "Quit" button.

        std::weak_ptr<World::ClientWorld> _world;                  ///> Weak pointer to the client world.
        std::unique_ptr<Graphics::IText> _scoresTitle;             ///> The "Scores" title text.
        std::vector<std::unique_ptr<Graphics::IText>> _scoreLines; ///> Text lines for displaying player scores.

        bool _backRequested = false; ///> Flag indicating if back to menu was requested.
        bool _quitRequested = false; ///> Flag indicating if quit was requested.
    };
} // namespace Engine
