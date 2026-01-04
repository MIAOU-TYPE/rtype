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
        try {
            const auto textures = renderer->textures();
            _backgroundTexture = textures->load("sprites/bg-preview.png");
            _logoTexture = textures->load("sprites/menu_logo.png");
            if (_backgroundTexture == Graphics::InvalidTexture)
                throw MenuError("{Menu::Menu} failed to load sprite/bg-preview.png texture");
            if (_logoTexture == Graphics::InvalidTexture)
                throw MenuError("{Menu::Menu} failed to load sprite/menu_logo.png texture");

            _backgroundCmd.textureId = _backgroundTexture;
            _logoCmd.textureId = _logoTexture;

            _play = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "PLAY");
            _settings = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "SETTINGS");
            _quit = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "QUIT");
        } catch (const std::exception &e) {
            throw MenuError(std::string("{Menu::Menu} initialization failed: ") + e.what());
        }
    }

    void Menu::onEnter()
    {
        _startRequested = false;
        _quitRequested = false;
        _settingsRequested = false;

        _play->reset();
        _settings->reset();
        _quit->reset();

        layout();
    }

    void Menu::layout()
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
        _logoCmd.position = {(w - static_cast<float>(logoSize.width) * LOGO_SCALE) * 0.5f, h * 0.05f};

        const float buttonX = (w - _play->bounds().w) / 2.f;
        _play->setPosition(buttonX, h * 0.63f);
        _settings->setPosition(buttonX, h * 0.76f);
        _quit->setPosition(buttonX, h * 0.89f);
    }

    void Menu::update(const InputFrame &frame)
    {
        handleInput(frame);

        _play->update(frame.mouseX, frame.mouseY);
        _settings->update(frame.mouseX, frame.mouseY);
        _quit->update(frame.mouseX, frame.mouseY);
    }

    void Menu::render() const
    {
        _renderer->draw(_backgroundCmd);
        _renderer->draw(_logoCmd);
        _play->render();
        _settings->render();
        _quit->render();
    }

    void Menu::handleInput(const InputFrame &frame)
    {
        if (frame.keyPressed) {
            switch (frame.key) {
                case Key::S: _settingsRequested = true; break;
                case Key::Q: _quitRequested = true; break;
                case Key::Enter: _startRequested = true; break;
                default: break;
            }
        }

        if (frame.mousePressed) {
            _play->onMousePressed(frame.mouseX, frame.mouseY);
            _settings->onMousePressed(frame.mouseX, frame.mouseY);
            _quit->onMousePressed(frame.mouseX, frame.mouseY);
        }

        if (_play->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                _startRequested = true;
            }))
            return;
        if (_settings->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                _settingsRequested = true;
            }))
            return;
        if (_quit->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                _quitRequested = true;
            })) {}
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
