/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityDrawing
*/

#pragma once

#include <algorithm>
#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "IRenderer.hpp"
#include "ITextureManager.hpp"
#include "SFMLAnimation.hpp"
#include "SFMLAnimationManager.hpp"
#include "SFMLEntityCreation.hpp"
#include <unordered_map>

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @brief Structure containing animation information with loop control.
     */
    struct AnimationInfo {
        std::shared_ptr<SFMLAnimation> animation; ///> The animation
    };

    /**
     * @brief Structure containing sprite information.
     */
    struct SpriteInfo {
        std::string path;                      ///> Path to the sprite file
        float width;                           ///> Width of the sprite
        float height;                          ///> Height of the sprite
        std::vector<AnimationInfo> animations; ///> List of animations with loop info
        std::string defaultAnimation;          ///> Name of the default animation
    };

    /**
     * @class SFMLEntityDrawingError
     * @brief Exception class for SFML entity drawing-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class SFMLEntityDrawingError : public std::exception {
      public:
        /**
         * @brief Constructor for SFMLEntityDrawingError.
         * @param message The error message to be associated with the exception.
         */
        explicit SFMLEntityDrawingError(const std::string &message) : _message("\n\t" + message)
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
     * @class SFMLEntityDrawing
     * @brief Manager for drawing entities using SFML.
     *
     * This class manages the creation and rendering of graphical entities.
     */
    class SFMLEntityDrawing {
      public:
        /**
         * @brief Constructor for SFMLEntityDrawing.
         * @param renderer Shared pointer to the renderer.
         * @param textureManager Shared pointer to the texture manager.
         */
        SFMLEntityDrawing(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager);

        /**
         * @brief Creates and adds a new entity.
         * @param x The X position of the entity.
         * @param y The Y position of the entity.
         * @param spriteName The name of the sprite.
         * @param id Unique identifier for the entity.
         * @return Shared pointer to the created entity.
         */
        std::shared_ptr<GraphicalEntity> createEntity(float x, float y, const std::string &spriteName, size_t id = 0);

        /**
         * @brief Updates all entities (including animations).
         * @param deltaTime Time elapsed since last update in seconds.
         */
        void updateAllEntities(float deltaTime);

        /**
         * @brief Renders all entities.
         */
        void renderAllEntities();

        /**
         * @brief Gets the sprite info from sprite name.
         * @param spriteName The name of the sprite.
         * @return The sprite information (path and dimensions).
         * @throws SFMLEntityDrawingError if the sprite name is not found in _spriteInfo.
         */
        SpriteInfo getSpriteInfoFromName(const std::string &spriteName) const;

        /**
         * @brief Gets the sprite path from sprite name.
         * @param spriteName The name of the sprite.
         * @return The full path to the sprite file.
         */
        std::string getSpritePathFromName(const std::string &spriteName) const;

        /**
         * @brief Creates an animation manager for a specific sprite.
         * @param spriteName The name of the sprite.
         * @return Shared pointer to the created animation manager.
         * @throws SFMLEntityDrawingError if sprite name is not found.
         */
        std::shared_ptr<SFMLAnimationManager> createAnimationManager(const std::string &spriteName) const;

        /**
         * @brief Gets an entity by index for external animation control.
         * @param index The index of the entity.
         * @return Shared pointer to the entity, nullptr if index is invalid.
         */
        std::shared_ptr<GraphicalEntity> getEntity(size_t index) const;

        /**
         * @brief Gets the number of entities.
         * @return The number of entities.
         */
        size_t getEntityCount() const;

        /**
         * @brief Checks if an animation should loop for a specific sprite.
         * @param spriteName The name of the sprite.
         * @param animationName The name of the animation.
         * @return True if the animation should loop, false otherwise.
         * @throws SFMLEntityDrawingError if sprite or animation is not found.
         */
        bool shouldAnimationLoop(const std::string &spriteName, const std::string &animationName) const;

        /**
         * @brief Gets an entity by its unique ID.
         * @param id The unique ID of the entity.
         * @return Shared pointer to the entity, nullptr if not found.
         */
        std::shared_ptr<GraphicalEntity> getEntityById(size_t id) const;

        /**
         * @brief Removes an entity by its unique ID.
         * @param id The unique ID of the entity to remove.
         * @return True if entity was found and removed, false otherwise.
         */
        bool removeEntityById(size_t id);

        /**
         * @brief Removes an entity by pointer.
         * @param entity Shared pointer to the entity to remove.
         * @return True if entity was found and removed, false otherwise.
         */
        bool removeEntity(const std::shared_ptr<GraphicalEntity> &entity);

        /**
         * @brief Clears all entities.
         */
        void clearAllEntities();

      private:
        std::shared_ptr<IRenderer> _renderer = nullptr;               ///> The renderer
        std::shared_ptr<ITextureManager> _textureManager = nullptr;   ///> The texture manager
        std::vector<std::shared_ptr<GraphicalEntity>> _entities = {}; ///> List of entities
        std::unordered_map<std::string, SpriteInfo> _spriteInfo;      ///> Mapping sprite names to info
        size_t _nextEntityId = 0;                                     ///> Next available entity ID
    };

} // namespace Graphics
