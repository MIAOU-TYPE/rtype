/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"

namespace Engine
{
    SettingsState::SettingsState(
        std::shared_ptr<Graphics::IGraphics> graphics,
        std::shared_ptr<Graphics::IRenderer> renderer)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer))
    {
    }

    void SettingsState::onEnter(StateManager &manager)
    {
        _manager = &manager;
        _menu = std::make_unique<SettingsMenu>(_renderer);
    }

    void SettingsState::onExit()
    {
        _menu.reset();
    }

    void SettingsState::update(const float dt)
    {
        _menu->update(dt);
        if (_menu->wantsBack()) {
            _manager->changeState(
                std::make_unique<MenuState>(_graphics, _renderer)
            );
        }
    }

    void SettingsState::render()
    {
        _menu->render();
    }

    bool SettingsState::onMousePressed(float x, float y)
    {
        if (_menu)
            return _menu->onMousePressed(x, y);
        return false;
    }
}
