/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderTextureManager
*/

#include "RenderTextureManager.hpp"
#include <iostream>

namespace Graphics
{
    RenderTextureManager::RenderTextureManager(unsigned int initialWidth, unsigned int initialHeight)
    {
        _renderTexture = std::make_unique<sf::RenderTexture>();
        if (!_renderTexture->resize({initialWidth, initialHeight})) {
            std::cerr << "{RenderTextureManager::RenderTextureManager} Failed to create render texture" << std::endl;
            _renderTexture.reset();
            throw RenderTextureError("{RenderTextureManager::RenderTextureManager}: Failed to create render texture");
        }
    }

    bool RenderTextureManager::isAvailable() const noexcept
    {
        return _renderTexture != nullptr;
    }

    bool RenderTextureManager::resize(unsigned int width, unsigned int height)
    {
        if (!_renderTexture) {
            _renderTexture = std::make_unique<sf::RenderTexture>();
        }

        if (!_renderTexture->resize({width, height})) {
            std::cerr << "{RenderTextureManager::resize} Failed to resize render texture" << std::endl;
            _renderTexture.reset();
            return false;
        }

        return true;
    }

    void RenderTextureManager::clear()
    {
        if (_renderTexture) {
            _renderTexture->clear();
        }
    }

    void RenderTextureManager::display()
    {
        if (_renderTexture) {
            _renderTexture->display();
        }
    }

    void RenderTextureManager::disable()
    {
        _renderTexture.reset();
    }

    sf::RenderTexture *RenderTextureManager::getRenderTexture() noexcept
    {
        return _renderTexture.get();
    }

    sf::Sprite RenderTextureManager::getSprite() const
    {
        if (!_renderTexture) {
            throw RenderTextureError("{RenderTextureManager::getSprite}: Render texture is not available");
        }
        return sf::Sprite(_renderTexture->getTexture());
    }
} // namespace Graphics
