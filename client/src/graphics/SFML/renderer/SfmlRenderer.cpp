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
        _textManager = std::make_shared<SfmlTextManager>(_fontManager);
        _colorBlindManager = std::make_unique<ColorBlindManager>(_resourceManager);
        _renderTexture = std::make_unique<sf::RenderTexture>();
        const auto windowSize = _window->getSize();
        if (!_renderTexture->resize(windowSize)) {
            _renderTexture.reset();
            throw RenderException("{SfmlRenderer::SfmlRenderer}: Failed to create render texture");
        }
    }

    Extent2u SfmlRenderer::getViewportSize() const noexcept
    {
        const auto size = _window->getSize();
        return {size.x, size.y};
    }

    void SfmlRenderer::beginFrame()
    {
        const bool wantsPostProcess = (_colorBlindManager && _colorBlindManager->isShaderAvailable());
        const auto windowSize = _window->getSize();
        const auto texSize = _renderTexture->getSize();
        if (!wantsPostProcess) {
            _window->clear();
            return;
        }
        if (!_renderTexture)
            _renderTexture = std::make_unique<sf::RenderTexture>();
        if (texSize.x != windowSize.x || texSize.y != windowSize.y) {
            if (!_renderTexture->resize(windowSize)) {
                std::cerr << "[SfmlRenderer] Failed to resize render texture, disabling colorblind shader\n";
                if (_colorBlindManager)
                    _colorBlindManager->setMode(ColorBlindMode::NONE);
                _renderTexture.reset();
                _window->clear();
                return;
            }
        }
        _renderTexture->clear();
    }

    void SfmlRenderer::endFrame()
    {
        if (_renderTexture && _colorBlindManager->isShaderAvailable()) {
            _renderTexture->display();
            sf::Sprite sprite(_renderTexture->getTexture());
            _window->clear();
            if (!_colorBlindManager->isShaderAvailable()) {
                std::cerr << "[SfmlRenderer] Colorblind shader not available during endFrame\n";
                _window->draw(sprite);
            } else {
                _window->draw(sprite, _colorBlindManager->getShader());
            }
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

    void SfmlRenderer::draw(const Engine::RenderCommand &cmd)
    {
        try {
            sf::Sprite sprite(_textureManager->get(cmd.textureId));

            sprite.setTextureRect({{cmd.frame.x, cmd.frame.y}, {cmd.frame.w, cmd.frame.h}});

            sprite.setPosition({cmd.position.x, cmd.position.y});
            sprite.setScale({cmd.scale.x, cmd.scale.y});

            if (_renderTexture && _colorBlindManager->isShaderAvailable()) {
                _renderTexture->draw(sprite);
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

            if (_renderTexture && _colorBlindManager->isShaderAvailable()) {
                _renderTexture->draw(sfText.get());
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

            if (mode != ColorBlindMode::NONE && !_renderTexture) {
                _renderTexture = std::make_unique<sf::RenderTexture>();
                const auto windowSize = _window->getSize();
                if (!_renderTexture->resize(windowSize)) {
                    std::cerr << "[SfmlRenderer] Failed to create render texture" << std::endl;
                    _renderTexture.reset();
                }
            }
        }
    }
} // namespace Graphics
