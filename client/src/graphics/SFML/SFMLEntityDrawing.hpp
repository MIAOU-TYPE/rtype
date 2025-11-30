/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityDrawing
*/

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "IRenderer.hpp"
#include "ITextureManager.hpp"
#include "SFMLEntityCreation.hpp"
#include <unordered_map>

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
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
         * @return Shared pointer to the created entity.
         */
        std::shared_ptr<GraphicalEntity> createEntity(float x, float y, const std::string &spriteName);

        /**
         * @brief Renders all entities.
         */
        void renderAllEntities();

        /**
         * @brief Structure containing sprite information.
         */
        struct SpriteInfo {
            std::string path; ///> Path to the sprite file
            float width;      ///> Width of the sprite
            float height;     ///> Height of the sprite
        };

        /**
         * @brief Gets the sprite info from sprite name.
         * @param spriteName The name of the sprite.
         * @return The sprite information (path and dimensions).
         * @throws std::runtime_error if the sprite name is not found in _spriteInfo.
         */
        static SpriteInfo getSpriteInfoFromName(const std::string &spriteName);

        /**
         * @brief Gets the sprite path from sprite name.
         * @param spriteName The name of the sprite.
         * @return The full path to the sprite file.
         * @throws std::runtime_error if the sprite name is not found in _spriteInfo.
         */
        static std::string getSpritePathFromName(const std::string &spriteName);

      private:
        std::shared_ptr<IRenderer> _renderer = nullptr;                 ///> The renderer
        std::shared_ptr<ITextureManager> _textureManager = nullptr;     ///> The texture manager
        std::vector<std::shared_ptr<GraphicalEntity>> _entities = {};   ///> List of entities
        static std::unordered_map<std::string, SpriteInfo> _spriteInfo; ///> Mapping sprite names to info
    };

} // namespace Graphics
