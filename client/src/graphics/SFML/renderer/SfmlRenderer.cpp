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

        _soundManager = std::make_shared<SfmlSoundManager>(_resourceManager);
        _musicManager = std::make_shared<SfmlMusicManager>(_resourceManager);
        _fontManager = std::make_shared<SfmlFontManager>(_resourceManager);
        _textureManager = std::make_shared<SfmlTextureManager>(_resourceManager);
        _textManager = std::make_shared<SfmlTextManager>(_fontManager);
    }

    Extent2u SfmlRenderer::getViewportSize() const noexcept
    {
        const auto size = _window->getSize();
        return {size.x, size.y};
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

    [[nodiscard]] std::shared_ptr<ITextManager> SfmlRenderer::texts() const noexcept
    {
        return _textManager;
    }

    std::shared_ptr<IAudioManager> SfmlRenderer::sounds() const noexcept
    {
        return _soundManager;
    }

    std::shared_ptr<IAudioManager> SfmlRenderer::musics() const noexcept
    {
        return _musicManager;
    }

    void SfmlRenderer::draw(const Engine::RenderCommand &cmd)
    {
        try {
            sf::Sprite sprite(_textureManager->get(cmd.textureId));

            sprite.setTextureRect({{cmd.frame.x, cmd.frame.y}, {cmd.frame.w, cmd.frame.h}});

            sprite.setPosition({cmd.position.x, cmd.position.y});
            sprite.setScale({cmd.scale.x, cmd.scale.y});

            _window->draw(sprite);
        } catch (const std::exception &e) {
            std::cerr << "{SfmlRenderer::draw}: " << e.what() << std::endl;
        }
    }

    void SfmlRenderer::drawText(const IText &text)
    {
        try {
            const auto &sfText = dynamic_cast<const SfmlText &>(text);
            _window->draw(sfText.get());
        } catch (const std::bad_cast &) {
            std::cerr << "{SfmlRenderer::drawText}: IText is not a SfmlText\n";
        } catch (const TextError &e) {
            std::cerr << "{SfmlRenderer::drawText}: " << e.what() << '\n';
        }
    }
} // namespace Graphics
