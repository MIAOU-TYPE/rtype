/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** IHUD
*/

#pragma once

#include <memory>
#include <string>

namespace Graphics
{
    class IRenderer;

    /**
     * @class IHUD
     * @brief Interface for Heads-Up Display management.
     *
     * This interface defines methods for managing and rendering game HUD elements
     * such as score, lives, level, health, and other status information.
     */
    class IHUD {
      public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IHUD() = default;

        /**
         * @brief Update the player's score display.
         * @param score The current score value.
         */
        virtual void setScore(unsigned int score) = 0;

        /**
         * @brief Update the high score display.
         * @param highScore The high score value.
         */
        virtual void setHighScore(unsigned int highScore) = 0;

        /**
         * @brief Update the current level display.
         * @param level The current level number.
         */
        virtual void setLevel(unsigned int level) = 0;

        /**
         * @brief Update the player's health display.
         * @param health The current health value (0-100).
         */
        virtual void setHealth(unsigned int health) = 0;

        /**
         * @brief Update the weapon status display.
         * @param weaponName The name of the current weapon.
         * @param ammo The remaining ammunition (-1 for unlimited).
         */
        virtual void setWeapon(const std::string &weaponName, int ammo = -1) = 0;

        /**
         * @brief Render the HUD on screen.
         * @param renderer The renderer to use for drawing.
         */
        virtual void render(IRenderer &renderer) = 0;

        /**
         * @brief Set the visibility of the HUD.
         * @param visible True to show the HUD, false to hide it.
         */
        virtual void setVisible(bool visible) = 0;

        /**
         * @brief Check if the HUD is currently visible.
         * @return true if the HUD is visible, false otherwise.
         */
        virtual bool isVisible() const = 0;
    };

} // namespace Graphics
