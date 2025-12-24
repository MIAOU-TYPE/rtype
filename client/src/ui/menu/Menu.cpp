/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Menu
*/

#include "Menu.hpp"

namespace Engine
{
    Menu::Menu(const std::shared_ptr<Graphics::IRenderer> &renderer) : _renderer(renderer)
    {
        const auto textures = renderer->textures();
        _backgroundTexture = textures->load("sprites/bg-preview.png");
        _logoTexture = textures->load("sprites/menu_logo.png");
        if (_backgroundTexture == Graphics::InvalidTexture || _logoTexture == Graphics::InvalidTexture)
            throw std::runtime_error("Menu: failed to load textures");

        _backgroundCmd.textureId = _backgroundTexture;
        _logoCmd.textureId = _logoTexture;

        _play = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "PLAY");
        _settings = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "SETTINGS");
        _quit = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "QUIT");
    }

    void Menu::update(float, const float mouseX, const float mouseY)
    {
        const auto vp = _renderer->getViewportSize();
        const auto w = static_cast<float>(vp.width);
        const auto h = static_cast<float>(vp.height);
        const auto texSize = _renderer->textures()->getSize(_backgroundTexture);

        _backgroundCmd.frame = {0, 0, static_cast<int>(texSize.width), static_cast<int>(texSize.height)};
        _backgroundCmd.position = {0.f, 0.f};
        _backgroundCmd.scale = {static_cast<float>(vp.width) / static_cast<float>(texSize.width),
            static_cast<float>(vp.height) / static_cast<float>(texSize.height)};

        const auto logoSize = _renderer->textures()->getSize(_logoTexture);

        _logoCmd.frame = {0, 0, static_cast<int>(logoSize.width), static_cast<int>(logoSize.height)};

        constexpr float LOGO_SCALE = 1.0f;
        _logoCmd.scale = {LOGO_SCALE, LOGO_SCALE};
        _logoCmd.position = {(w - logoSize.width * LOGO_SCALE) * 0.5f, h * 0.05f};

        const float buttonX = (w - _play->bounds().w) / 2.f;
        _play->setPosition(buttonX, h * 0.63f);
        _settings->setPosition(buttonX, h * 0.76f);
        _quit->setPosition(buttonX, h * 0.89f);
        _play->update(mouseX, mouseY);
        _settings->update(mouseX, mouseY);
        _quit->update(mouseX, mouseY);
    }

    void Menu::render() const
    {
        _renderer->draw(_backgroundCmd);
        _renderer->draw(_logoCmd);
        _play->render();
        _settings->render();
        _quit->render();
    }

    bool Menu::onMousePressed(const float x, const float y) const
    {
        _play->onMousePressed(x, y);
        _settings->onMousePressed(x, y);
        _quit->onMousePressed(x, y);
        return false;
    }

    bool Menu::onMouseReleased(const float x, const float y)
    {
        if (_play->onMouseReleased(x, y)) {
            _startRequested = true;
            _play->reset();
            return true;
        }
        if (_settings->onMouseReleased(x, y)) {
            _settingsRequested = true;
            _settings->reset();
            return true;
        }
        if (_quit->onMouseReleased(x, y)) {
            _quitRequested = true;
            _quit->reset();
            return true;
        }
        return false;
    }

    bool Menu::wantsToStart() const noexcept
    {
        return _startRequested;
    }

    bool Menu::wantsToQuit() const noexcept
    {
        return _quitRequested;
    }

    bool Menu::wantsSettings() const noexcept
    {
        return _settingsRequested;
    }
} // namespace Engine
