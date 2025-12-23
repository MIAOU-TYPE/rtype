/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"
#include <stdexcept>

namespace Engine
{
    SettingsMenu::SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer)
        : _renderer(renderer)
    {
        const auto textures = renderer->textures();

        _backgroundTexture = textures->load("sprites/bg-preview.png");
        if (_backgroundTexture == Graphics::InvalidTexture)
            throw std::runtime_error("SettingsMenu: failed to load background");

        _backgroundCmd.textureId = _backgroundTexture;

        _audio = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "AUDIO");
        _left  = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "<");
        _right = std::make_unique<UIButton>(_renderer, ButtonSize::Small, ">");
        _back  = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "BACK");
    }

    void SettingsMenu::update(float, const float mouseX, const float mouseY)
    {
        const auto vp = _renderer->getViewportSize();
        const auto w = static_cast<float>(vp.width);
        const auto h = static_cast<float>(vp.height);

        const float cx = w / 2.f;
        const float cy = h / 2.f;

        _backgroundCmd.frame = {0, 0, static_cast<int>(w), static_cast<int>(h)};
        _backgroundCmd.position = {0.f, 0.f};

        _audio->setPosition(cx - _audio->bounds().w / 2.f, cy - 80.f);
        _left->setPosition(cx - 120.f, cy);
        _right->setPosition(cx + 60.f, cy);
        _back->setPosition(cx - _back->bounds().w / 2.f, h * 0.85f);

        _audio->update(mouseX, mouseY);
        _left->update(mouseX, mouseY);
        _right->update(mouseX, mouseY);
        _back->update(mouseX, mouseY);
    }

    void SettingsMenu::render() const
    {
        _renderer->draw(_backgroundCmd);
        _audio->render();
        _left->render();
        _right->render();
        _back->render();
    }

    bool SettingsMenu::onMousePressed(const float x, const float y) const
    {
        _audio->onMousePressed(x, y);
        _left->onMousePressed(x, y);
        _right->onMousePressed(x, y);
        _back->onMousePressed(x, y);
        return false;
    }

    bool SettingsMenu::onMouseReleased(const float x, const float y)
    {
        if (_back->onMouseReleased(x, y)) {
            _backRequested = true;
            _back->reset();
            return true;
        }
        return false;
    }

    bool SettingsMenu::wantsBack() const noexcept
    {
        return _backRequested;
    }
}
