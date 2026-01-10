/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AMenuS
*/

#include "AMenu.hpp"

namespace Engine
{
    AMenu::AMenu(std::shared_ptr<Graphics::IRenderer> renderer) : _renderer(std::move(renderer))
    {
        if (!_renderer)
            throw std::runtime_error("{AMenu} null renderer");
    }

    void AMenu::loadBackground(const std::string &path)
    {
        const auto textures = _renderer->textures();
        _backgroundTexture = textures->load(path);
        if (_backgroundTexture == Graphics::InvalidTexture)
            throw std::runtime_error("{AMenu} failed to load background: " + path);
        _backgroundCmd.textureId = _backgroundTexture;
    }

    void AMenu::layoutBackground()
    {
        if (_backgroundTexture == Graphics::InvalidTexture)
            return;
        const auto [width, height] = _renderer->getViewportSize();
        const auto [th, tw] = _renderer->textures()->getSize(_backgroundTexture);
        const auto w = static_cast<float>(width);
        const auto h = static_cast<float>(height);
        _backgroundCmd.frame = {0, 0, static_cast<int>(tw), static_cast<int>(th)};
        _backgroundCmd.position = {0.f, 0.f};
        _backgroundCmd.scale = {w / static_cast<float>(tw), h / static_cast<float>(th)};
    }

    void AMenu::renderBackground() const
    {
        if (_backgroundTexture != Graphics::InvalidTexture)
            _renderer->draw(_backgroundCmd);
    }
} // namespace Engine
