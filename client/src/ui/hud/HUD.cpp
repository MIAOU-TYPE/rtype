/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** HUD
*/

#include "HUD.hpp"

namespace Engine
{
    HUD::HUD(std::shared_ptr<Graphics::IRenderer> renderer, World::ClientWorld &world)
        : _renderer(std::move(renderer)), _world(world)
    {
    }

    void HUD::initialize()
    {
        _fontHandle = _renderer->fonts()->load("fonts/font.ttf");
        if (!_renderer->fonts()->isValid(_fontHandle)) {
            throw HUDError("Failed to load font 'fonts/font.ttf' for HUD");
        }
        _scoreText = _renderer->texts()->createText(24, {255, 255, 255, 255});
        _scoreText->setFont(_fontHandle);
        _scoreText->setPosition(10.f, 10.f);
        _scoreText->setString("Score: 0");
    }

    void HUD::render()
    {
        if (_scoreText) {
            _scoreText->setString("Score: " + std::to_string(_world.getScore()));
            _renderer->draw(*_scoreText);
        }
    }
} // namespace Engine