/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlRenderer
*/
#include "SfmlRenderer.hpp"

namespace Graphics
{
    SfmlRenderer::SfmlRenderer(
        const std::shared_ptr<sf::RenderWindow> &window, std::shared_ptr<Resources::IResourceManager> resourceManager)
        : _window(window), _resourceManager(std::move(resourceManager))
    {
        if (!_window)
            throw RenderException("{SfmlRenderer::SfmlRenderer}: SFML RenderWindow cannot be null");
        if (!_resourceManager)
            throw RenderException("{SfmlRenderer::SfmlRenderer}: Resource manager cannot be null");

        _fontManager = std::make_shared<SfmlFontManager>(_resourceManager);
        _textureManager = std::make_shared<SfmlTextureManager>(_resourceManager);
    }

    void SfmlRenderer::beginFrame()
    {
        _window->clear();
    }

    void SfmlRenderer::endFrame()
    {
        _window->display();
    }

    std::shared_ptr<IFontManager> SfmlRenderer::fonts() const noexcept
    {
        return _fontManager;
    }

    std::shared_ptr<ITextureManager> SfmlRenderer::textures() const noexcept
    {
        return _textureManager;
    }

    void SfmlRenderer::draw(const Engine::RenderCommand &cmd)
    {
        sf::Sprite sprite(_textureManager->get(cmd.textureId));

        sprite.setTextureRect(
            sf::Rect<int>(sf::Vector2i(cmd.frame.x, cmd.frame.y), sf::Vector2i(cmd.frame.w, cmd.frame.h)));

        sprite.setPosition(sf::Vector2f(cmd.position.x, cmd.position.y));

        _window->draw(sprite);
    }

} // namespace Graphics
