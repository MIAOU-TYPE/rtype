/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameHUD
*/

#pragma once

#include <memory>
#include <string>
#include "IHUD.hpp"
#include "IRenderer.hpp"
#include "IText.hpp"
#include "ITextureManager.hpp"

namespace Graphics
{
    /**
     * @class HUDError
     * @brief Custom exception class for HUD-related errors.
     *
     * This exception is thrown when errors occur during HUD initialization,
     * rendering, or update operations.
     */
    class HUDError : public std::exception {
      public:
        /**
         * @brief Construct a new HUD Error object.
         * @param message The error message describing what went wrong.
         */
        explicit HUDError(const std::string &message) : _message("\n\tHUD Error: " + message)
        {
        }

        /**
         * @brief Get the error message.
         * @return const char* Pointer to the error message string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> The formatted error message
    };

    /**
     * @class GameHUD
     * @brief Implementation of the HUD interface for displaying game information.
     *
     * This class manages the rendering of game statistics including score, lives,
     * health, level, and weapon information on the screen.
     */
    class GameHUD : public IHUD {
      public:
        /**
         * @brief Construct a new Game HUD object.
         * @param renderer Shared pointer to the renderer.
         * @param textureManager Shared pointer to the texture manager.
         * @param fontPath Path to the font file to use for HUD text.
         * @throws HUDError if initialization fails.
         */
        GameHUD(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager,
            const std::string &fontPath);

        /**
         * @brief Destroy the Game HUD object.
         */
        ~GameHUD() override = default;

        /**
         * @brief Update the player's score display.
         * @param score The current score value.
         */
        void setScore(unsigned int score) override;

        /**
         * @brief Update the high score display.
         * @param highScore The high score value.
         */
        void setHighScore(unsigned int highScore) override;

        /**
         * @brief Update the current level display.
         * @param level The current level number.
         */
        void setLevel(unsigned int level) override;

        /**
         * @brief Update the player's health display.
         * @param health The current health value (0-100).
         */
        void setHealth(unsigned int health) override;

        /**
         * @brief Update the weapon status display.
         * @param weaponName The name of the current weapon.
         * @param ammo The remaining ammunition (-1 for unlimited).
         */
        void setWeapon(const std::string &weaponName, int ammo = -1) override;

        /**
         * @brief Render the HUD on screen.
         * @param renderer The renderer to use for drawing.
         */
        void render(IRenderer &renderer) override;

        /**
         * @brief Set the visibility of the HUD.
         * @param visible True to show the HUD, false to hide it.
         */
        void setVisible(bool visible) override;

        /**
         * @brief Check if the HUD is currently visible.
         * @return true if the HUD is visible, false otherwise.
         */
        bool isVisible() const override;

      private:
        /**
         * @brief Initialize all HUD text elements.
         * @throws HUDError if text creation fails.
         */
        void initializeTextElements();

        /**
         * @brief Update the text content of a text element.
         * @param text The text element to update.
         * @param content The new content string.
         */
        void updateText(std::unique_ptr<IText> &text, const std::string &content);

        std::shared_ptr<IRenderer> _renderer;             ///> Renderer for drawing
        std::shared_ptr<ITextureManager> _textureManager; ///> Texture manager for loading fonts
        std::string _fontPath;                            ///> Path to the font file

        // HUD Text Elements
        std::unique_ptr<IText> _scoreText;     ///> Score display text
        std::unique_ptr<IText> _highScoreText; ///> High score display text
        std::unique_ptr<IText> _levelText;     ///> Level display text
        std::unique_ptr<IText> _healthText;    ///> Health display text
        std::unique_ptr<IText> _weaponText;    ///> Weapon info display text

        // HUD Data
        unsigned int _score = 0;           ///> Current score
        unsigned int _highScore = 0;       ///> High score
        unsigned int _level = 1;           ///> Current level
        unsigned int _health = 100;        ///> Current health (0-100)
        std::string _weaponName = "Basic"; ///> Current weapon name
        int _ammo = -1;                    ///> Ammunition count (-1 = unlimited)
        bool _visible = true;              ///> HUD visibility flag

        // Layout constants
        static constexpr float TOP_BAR_Y = 20.0f;      ///> Y position for top bar elements
        static constexpr float BOTTOM_BAR_Y = 1050.0f; ///> Y position for bottom bar elements
        static constexpr float LEFT_MARGIN = 30.0f;    ///> Left margin for HUD elements
        static constexpr float RIGHT_MARGIN = 1890.0f; ///> Right margin for HUD elements
        static constexpr unsigned int FONT_SIZE = 24;  ///> Font size for HUD text
    };

} // namespace Graphics
