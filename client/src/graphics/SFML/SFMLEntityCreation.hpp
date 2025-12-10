/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityCreation
*/

#pragma once

#include <exception>
#include <memory>
#include <string>
#include "IRenderer.hpp"
#include "ITextureManager.hpp"
#include "SFMLAnimationManager.hpp"

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    class SFMLEntityDrawing; // Forward declaration

    /**
     * @class GraphicalEntityError
     * @brief Exception class for graphical entity-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class GraphicalEntityError : public std::exception {
      public:
        /**
         * @brief Constructor for GraphicalEntityError.
         * @param message The error message to be associated with the exception.
         */
        explicit GraphicalEntityError(const std::string &message) : _message("\n\t" + message)
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
     * @class GraphicalEntity
     * @brief Represents a graphical entity with position and sprite.
     *
     * This class handles the graphical representation of game entities
     * with a position and associated sprite.
     */
    class GraphicalEntity {
      public:
        /**
         * @brief Constructor for GraphicalEntity.
         * @param x The X position of the entity.
         * @param y The Y position of the entity.
         * @param spriteName The name of the sprite (without path).
         * @param textureManager Shared pointer to the texture manager.
         * @param entityDrawing Reference to the entity drawing manager for sprite info.
         * @param id Unique identifier for the entity.
         */
        GraphicalEntity(float x, float y, const std::string &spriteName,
            std::shared_ptr<ITextureManager> textureManager, const SFMLEntityDrawing &entityDrawing, size_t id = 0);

        /**
         * @brief Destructor for GraphicalEntity.
         */
        ~GraphicalEntity() = default;

        /**
         * @brief Sets the position of the entity.
         * @param x The new X position.
         * @param y The new Y position.
         */
        void setPosition(float x, float y);

        /**
         * @brief Gets the X position of the entity.
         * @return The X position.
         */
        float getX() const;

        /**
         * @brief Gets the Y position of the entity.
         * @return The Y position.
         */
        float getY() const;

        /**
         * @brief Gets the sprite name.
         * @return The sprite name.
         */
        const std::string &getSpriteName() const;

        /**
         * @brief Gets the width of the entity sprite.
         * @return The width in pixels.
         */
        float getWidth() const;

        /**
         * @brief Gets the height of the entity sprite.
         * @return The height in pixels.
         */
        float getHeight() const;

        /**
         * @brief Renders the entity.
         * @param renderer The renderer to use.
         */
        void render(const std::shared_ptr<IRenderer> &renderer);

        /**
         * @brief Updates the entity (including animations).
         * @param deltaTime Time elapsed since last update in seconds.
         */
        void update(float deltaTime);

        /**
         * @brief Sets the current animation.
         * @param animationName Name of the animation to set.
         * @throws GraphicalEntityError if animation doesn't exist.
         */
        void setAnimation(const std::string &animationName);

        /**
         * @brief Gets the animation manager.
         * @return Shared pointer to the animation manager.
         */
        std::shared_ptr<SFMLAnimationManager> getAnimationManager() const;

        /**
         * @brief Checks if the current animation is finished (for non-looping animations).
         * @return True if the current animation is finished, false otherwise.
         */
        bool isCurrentAnimationFinished() const;

        /**
         * @brief Gets the name of the current animation.
         * @return The name of the current animation, empty if no animation manager.
         */
        std::string getCurrentAnimationName() const;

        /**
         * @brief Gets the unique ID of the entity.
         * @return The unique ID.
         */
        size_t getId() const;

        /**
         * @brief Sets the unique ID of the entity.
         * @param id The new unique ID.
         */
        void setId(size_t id);

      private:
        float _x = 0.0f;                                            ///> X position
        float _y = 0.0f;                                            ///> Y position
        std::string _spriteName = "";                               ///> Name of the sprite
        std::shared_ptr<ITextureManager> _textureManager = nullptr; ///> Texture manager
        std::unique_ptr<ISprite> _sprite = nullptr;                 ///> The sprite instance
        const SFMLEntityDrawing &_entityDrawing;                    ///> Reference to entity drawing for sprite info
        std::shared_ptr<SFMLAnimationManager> _animationManager = nullptr; ///> Animation manager
        size_t _id = 0;                                                    ///> Unique identifier
    };

} // namespace Graphics