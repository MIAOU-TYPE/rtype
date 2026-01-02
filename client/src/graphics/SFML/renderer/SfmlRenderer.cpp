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
        _colorBlindManager = std::make_unique<ColorBlindManager>(_resourceManager);
        const auto windowSize = _window->getSize();
        _renderTextureManager = std::make_unique<RenderTextureManager>(windowSize.x, windowSize.y);
    }

    Extent2u SfmlRenderer::getViewportSize() const noexcept
    {
        const auto size = _window->getSize();
        return {size.x, size.y};
    }

    void SfmlRenderer::beginFrame()
    {
        if (_renderTextureManager && _renderTextureManager->isAvailable() && _colorBlindManager->isShaderAvailable()) {
            const auto windowSize = _window->getSize();
            const auto textureSize = _renderTextureManager->getRenderTexture()->getSize();

            if (windowSize != textureSize) {
                if (!_renderTextureManager->resize(windowSize.x, windowSize.y)) {
                    std::cerr << "{SfmlRenderer::beginFrame} Failed to resize render texture, disabling shader"
                              << std::endl;
                    _renderTextureManager->disable();
                }
            }
            _renderTextureManager->clear();
        }
        _window->clear();
    }

    void SfmlRenderer::endFrame()
    {
        if (_renderTextureManager && _renderTextureManager->isAvailable() && _colorBlindManager->isShaderAvailable()) {
            _renderTextureManager->display();
            sf::Sprite sprite = _renderTextureManager->getSprite();
            _window->draw(sprite, _colorBlindManager->getShader());
        }
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

    std::shared_ptr<ITextManager> SfmlRenderer::texts() const noexcept
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

            if (_renderTextureManager && _renderTextureManager->isAvailable()
                && _colorBlindManager->isShaderAvailable()) {
                _renderTextureManager->getRenderTexture()->draw(sprite);
            } else {
                _window->draw(sprite);
            }
        } catch (const std::exception &e) {
            std::cerr << "{SfmlRenderer::draw}: " << e.what() << std::endl;
        }
    }

    void SfmlRenderer::draw(const IText &text)
    {
        try {
            const auto &sfText = dynamic_cast<const SfmlText &>(text);

            if (_renderTextureManager && _renderTextureManager->isAvailable()
                && _colorBlindManager->isShaderAvailable()) {
                _renderTextureManager->getRenderTexture()->draw(sfText.get());
            } else {
                _window->draw(sfText.get());
            }
        } catch (const std::bad_cast &) {
            std::cerr << "{SfmlRenderer::draw}: IText is not a SfmlText\n";
        } catch (const TextError &e) {
            std::cerr << "{SfmlRenderer::draw}: " << e.what() << '\n';
        }
    }

    void SfmlRenderer::setColorBlindMode(ColorBlindMode mode)
    {
        if (_colorBlindManager) {
            _colorBlindManager->setMode(mode);

            if (mode != ColorBlindMode::NONE && (!_renderTextureManager || !_renderTextureManager->isAvailable())) {
                const auto windowSize = _window->getSize();
                if (!_renderTextureManager) {
                    _renderTextureManager = std::make_unique<RenderTextureManager>(windowSize.x, windowSize.y);
                } else {
                    if (!_renderTextureManager->resize(windowSize.x, windowSize.y)) {
                        std::cerr << "{SfmlRenderer::setColorBlindMode} Failed to create render texture" << std::endl;
                    }
                }
            }
        }
    }

    ColorBlindMode SfmlRenderer::getColorBlindMode() const noexcept
    {
        if (_colorBlindManager)
            return _colorBlindManager->getMode();
        return ColorBlindMode::NONE;
    }
} // namespace Graphics
