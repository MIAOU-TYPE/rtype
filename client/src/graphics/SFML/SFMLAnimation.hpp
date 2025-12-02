/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLAnimation
*/

#pragma once

#include <exception>
#include <string>
#include <vector>

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @class SFMLAnimationError
     * @brief Exception class for SFML animation-related errors.
     */
    class SFMLAnimationError : public std::exception {
      public:
        /**
         * @brief Constructor for SFMLAnimationError.
         * @param message The error message to be associated with the exception.
         */
        explicit SFMLAnimationError(const std::string &message) : _message("\n\t" + message)
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
     * @brief Structure representing a single animation frame.
     */
    struct AnimationFrame {
        int x;          ///> X position in sprite sheet
        int y;          ///> Y position in sprite sheet
        int width;      ///> Width of the frame
        int height;     ///> Height of the frame
        float duration; ///> Duration of this frame in seconds
    };

    /**
     * @class SFMLAnimation
     * @brief Manages sprite sheet animations.
     */
    class SFMLAnimation {
      public:
        /**
         * @brief Constructor for SFMLAnimation.
         * @param name The name of the animation.
         * @param frames Vector of animation frames.
         * @param loop Whether the animation should loop.
         */
        SFMLAnimation(const std::string &name, const std::vector<AnimationFrame> &frames, bool loop = true);

        /**
         * @brief Updates the animation based on delta time.
         * @param deltaTime Time elapsed since last update in seconds.
         */
        void update(float deltaTime);

        /**
         * @brief Gets the current frame of the animation.
         * @return Reference to the current animation frame.
         * @throws SFMLAnimationError if no frames are available.
         */
        const AnimationFrame &getCurrentFrame() const;

        /**
         * @brief Resets the animation to the first frame.
         */
        void reset();

        /**
         * @brief Checks if the animation has finished (for non-looping animations).
         * @return True if animation is finished, false otherwise.
         */
        bool isFinished() const;

        /**
         * @brief Gets the animation name.
         * @return The name of the animation.
         */
        const std::string &getName() const;

        /**
         * @brief Sets whether the animation should loop.
         * @param loop True to enable looping, false otherwise.
         */
        void setLoop(bool loop);

        /**
         * @brief Gets the number of frames in the animation.
         * @return The total number of frames.
         */
        size_t getFrameCount() const;

      private:
        std::string _name = "";                   ///> Animation name
        std::vector<AnimationFrame> _frames = {}; ///> Animation frames
        size_t _currentFrameIndex = 0;            ///> Current frame index
        float _currentFrameTime = 0.0f;           ///> Time elapsed for current frame
        bool _loop = true;                        ///> Whether animation should loop
        bool _finished = false;                   ///> Whether animation has finished
    };

} // namespace Graphics