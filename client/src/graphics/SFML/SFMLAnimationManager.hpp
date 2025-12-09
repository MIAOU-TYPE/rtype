/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLAnimationManager
*/

#pragma once

#include <exception>
#include <memory>
#include <string>
#include "SFMLAnimation.hpp"
#include <unordered_map>

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @class SFMLAnimationManagerError
     * @brief Exception class for SFML animation manager-related errors.
     */
    class SFMLAnimationManagerError : public std::exception {
      public:
        /**
         * @brief Constructor for SFMLAnimationManagerError.
         * @param message The error message to be associated with the exception.
         */
        explicit SFMLAnimationManagerError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message = ""; ///> Error message
    };

    /**
     * @class SFMLAnimationManager
     * @brief Manages multiple animations for an entity.
     */
    class SFMLAnimationManager {
      public:
        /**
         * @brief Constructor for SFMLAnimationManager.
         */
        SFMLAnimationManager() = default;

        /**
         * @brief Adds an animation to the manager.
         * @param animation Shared pointer to the animation to add.
         * @throws SFMLAnimationManagerError if animation is null or name already exists.
         */
        void addAnimation(std::shared_ptr<SFMLAnimation> animation);

        /**
         * @brief Sets the current active animation.
         * @param animationName Name of the animation to set as active.
         * @throws SFMLAnimationManagerError if animation name doesn't exist.
         */
        void setCurrentAnimation(const std::string &animationName);

        /**
         * @brief Updates the current active animation.
         * @param deltaTime Time elapsed since last update in seconds.
         */
        void update(float deltaTime);

        /**
         * @brief Gets the current active animation frame.
         * @return Reference to the current animation frame.
         * @throws SFMLAnimationManagerError if no active animation is set.
         */
        const AnimationFrame &getCurrentFrame() const;

        /**
         * @brief Gets the current active animation.
         * @return Shared pointer to the current animation.
         * @throws SFMLAnimationManagerError if no active animation is set.
         */
        std::shared_ptr<SFMLAnimation> getCurrentAnimation() const;

        /**
         * @brief Checks if an animation exists.
         * @param animationName Name of the animation to check.
         * @return True if animation exists, false otherwise.
         */
        bool hasAnimation(const std::string &animationName) const;

        /**
         * @brief Gets the name of the current active animation.
         * @return Name of the current animation, empty string if none set.
         */
        const std::string &getCurrentAnimationName() const;

        /**
         * @brief Resets the current animation to its first frame.
         */
        void resetCurrentAnimation();

      private:
        std::unordered_map<std::string, std::shared_ptr<SFMLAnimation>> _animations = {}; ///> Map of animations
        std::shared_ptr<SFMLAnimation> _currentAnimation = nullptr;                       ///> Current active animation
        std::string _currentAnimationName = "";                                           ///> Name of current animation
    };

} // namespace Graphics