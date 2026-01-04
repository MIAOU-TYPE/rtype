/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderTextureManager
*/

#include "RenderTextureManager.hpp"

namespace Graphics
{
    RenderTextureManager::RenderTextureManager(unsigned int initialWidth, unsigned int initialHeight)
    {
        _renderTexture = std::make_unique<sf::RenderTexture>();
        if (!_renderTexture->resize({initialWidth, initialHeight})) {
            std::cerr << "{RenderTextureManager::RenderTextureManager} Failed to create render texture" << std::endl;
            return;
        }
    }

    bool RenderTextureManager::isAvailable() const noexcept
    {
        return _renderTexture != nullptr;
    }

    bool RenderTextureManager::resize(unsigned int width, unsigned int height)
    {
        if (!_renderTexture) {
            std::cerr << "{RenderTextureManager::resize} Render texture is not initialized" << std::endl;
            return false;
        }

        if (!_renderTexture->resize({width, height})) {
            std::cerr << "{RenderTextureManager::resize} Failed to resize render texture" << std::endl;
            return false;
        }

        return true;
    }

    void RenderTextureManager::clear() const
    {
        if (_renderTexture) {
            _renderTexture->clear();
        }
    }

    void RenderTextureManager::display() const
    {
        if (_renderTexture) {
            _renderTexture->display();
        }
    }

    sf::RenderTexture *RenderTextureManager::getRenderTexture() noexcept
    {
        return _renderTexture.get();
    }

    sf::Sprite RenderTextureManager::getSprite() const
    {
        if (!_renderTexture || _renderTexture->getTexture().getSize().x == 0
            || _renderTexture->getTexture().getSize().y == 0) {
            std::cerr << "{RenderTextureManager::getSprite} Render texture is not available" << std::endl;
        }
        return sf::Sprite(_renderTexture->getTexture());
    }
} // namespace Graphics
