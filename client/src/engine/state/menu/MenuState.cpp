/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MenuState
*/

#include "MenuState.hpp"

namespace Engine
{
    MenuState::MenuState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer))
    {
    }

    void MenuState::onEnter(StateManager &manager)
    {
        _manager = &manager;
        _menu = std::make_unique<Menu>(_renderer);
    }

    void MenuState::onExit()
    {
        _menu.reset();
    }

    void MenuState::onSettings() const
    {
        if (_menu)
            _menu->onSettingsPressed();
    }

    void MenuState::update(const float dt)
    {
        _menu->update(dt);
        if (_menu->wantsSettings()) {
            _manager->changeState(
                std::make_unique<SettingsState>(_graphics, _renderer)
            );
            return;
        }
        if (_menu->wantsToQuit())
            _manager->changeState(nullptr);
    }

    void MenuState::render()
    {
        _menu->render();
    }

    bool MenuState::onMousePressed(float x, float y)
    {
        if (_menu)
            return _menu->onMousePressed(x, y);
        return false;
    }
} // namespace Engine
