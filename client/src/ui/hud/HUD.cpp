/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** HUD
*/

#include "HUD.hpp"

namespace Engine
{
    HUD::HUD(std::shared_ptr<Graphics::IRenderer> renderer, std::function<int()> getScore,
        std::function<int()> getCurrentLife, std::function<int()> getMaxLife)
        : _renderer(std::move(renderer)), _getScore(getScore), _getCurrentLife(getCurrentLife), _getMaxLife(getMaxLife)
    {
        auto fontHandle = _renderer->fonts()->load("fonts/font.ttf");
        if (!_renderer->fonts()->isValid(fontHandle)) {
            throw HUDError("Failed to load font 'fonts/font.ttf' for HUD");
        }
        _scoreText = _renderer->texts()->createText(24, {255, 255, 255, 255});
        _scoreText->setFont(fontHandle);
        _scoreText->setPosition(10.f, 10.f);
        _scoreText->setString("Score: 0");

        _healthText = _renderer->texts()->createText(24, {255, 255, 255, 255});
        _healthText->setFont(fontHandle);
        _healthText->setPosition(10.f, 40.f);
        _healthText->setString("Health: " + std::to_string(_getCurrentLife()) + "/" + std::to_string(_getMaxLife()));
    }

    void HUD::render()
    {
        if (_scoreText) {
            _scoreText->setString("Score: " + std::to_string(_getScore()));
            _renderer->draw(*_scoreText);
        }
        if (_healthText) {
            _healthText->setString(
                "Health: " + std::to_string(_getCurrentLife()) + "/" + std::to_string(_getMaxLife()));
            _renderer->draw(*_healthText);
        }
    }
} // namespace Engine