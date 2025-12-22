/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Menu
*/

#include "Menu.hpp"
#include <algorithm>
#include <stdexcept>

namespace
{
    constexpr float REF_WIDTH = 1920.f;
    constexpr float REF_HEIGHT = 1080.f;
    constexpr float LOGO_W = 1000.f;
    constexpr float LOGO_H = 670.f;
} // namespace

namespace Engine
{
    Menu::Menu(const std::shared_ptr<Graphics::IRenderer> &renderer) : _renderer(renderer)
    {
        const auto font = renderer->fonts()->load("fonts/r-type.otf");
        const auto textManager = renderer->texts();
        const auto textureManager = renderer->textures();

        if (font == Graphics::InvalidFont)
            throw std::runtime_error("Failed to load font: fonts/r-type.otf");
        if (!textManager || !textureManager)
            throw std::runtime_error("Invalid text or texture manager");

        _backgroundTexture = textureManager->load("sprites/bg-preview.png");
        _logoTexture = textureManager->load("sprites/menu_logo.png");

        if (_backgroundTexture == Graphics::InvalidTexture)
            throw std::runtime_error("Failed to load background");
        if (_logoTexture == Graphics::InvalidTexture)
            throw std::runtime_error("Failed to load logo");

        _backgroundCmd.textureId = _backgroundTexture;
        _logoCmd.textureId = _logoTexture;

        _start = textManager->createText();
        _start->setFont(font);
        _start->setString("Press ENTER to Start");

        _quit = textManager->createText();
        _quit->setFont(font);
        _quit->setString("Press ESC to Quit");
    }

    void Menu::update(float)
    {
        const auto viewport = _renderer->getViewportSize();
        const float w = static_cast<float>(viewport.width);
        const float h = static_cast<float>(viewport.height);
        const float scale = std::min(w / REF_WIDTH, h / REF_HEIGHT);
        const float logoW = LOGO_W * scale;
        const float logoH = LOGO_H * scale;

        _backgroundCmd.frame = {0, 0, static_cast<int>(viewport.width), static_cast<int>(viewport.height)};
        _backgroundCmd.position = {0.f, 0.f};

        _logoCmd.frame = {0, 0, static_cast<int>(logoW), static_cast<int>(logoH)};
        _logoCmd.position = {(w - logoW) / 2.f, h * 0.05f};

        _start->setCharacterSize(static_cast<unsigned int>(35 * scale));
        _start->setPosition((w - _start->getWidth()) / 2.f, h * 0.70f);

        _quit->setCharacterSize(static_cast<unsigned int>(35 * scale));
        _quit->setPosition((w - _quit->getWidth()) / 2.f, h * 0.85f);
    }

    void Menu::render() const
    {
        _renderer->draw(_backgroundCmd);
        _renderer->draw(_logoCmd);
        _renderer->drawText(*_start);
        _renderer->drawText(*_quit);
    }

    void Menu::onEnterPressed()
    {
        _startRequested = true;
    }

    void Menu::onEscapePressed()
    {
        _quitRequested = true;
    }

    bool Menu::wantsToStart() const noexcept
    {
        return _startRequested;
    }

    bool Menu::wantsToQuit() const noexcept
    {
        return _quitRequested;
    }
} // namespace Engine
