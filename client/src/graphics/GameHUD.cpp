/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameHUD
*/

#include "GameHUD.hpp"
#include <iomanip>
#include <sstream>

using namespace Graphics;

GameHUD::GameHUD(
    std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager, const std::string &fontPath)
    : _renderer(std::move(renderer)), _textureManager(std::move(textureManager)), _fontPath(fontPath)
{
    if (!_renderer) {
        throw HUDError("Renderer cannot be null");
    }

    if (!_textureManager) {
        throw HUDError("Texture manager cannot be null");
    }

    try {
        initializeTextElements();
    } catch (const std::exception &e) {
        throw HUDError("Failed to initialize HUD: " + std::string(e.what()));
    }
}

void GameHUD::initializeTextElements()
{
    _scoreText = _textureManager->createText(_fontPath, "SCORE: 00000", FONT_SIZE);
    if (!_scoreText) {
        throw HUDError("Failed to create score text");
    }
    _scoreText->setColor(255, 255, 255, 255);
    _scoreText->setPosition(600.0f, TOP_BAR_Y);

    _highScoreText = _textureManager->createText(_fontPath, "HI: 00000", FONT_SIZE);
    if (!_highScoreText) {
        throw HUDError("Failed to create high score text");
    }
    _highScoreText->setColor(255, 255, 0, 255);
    _highScoreText->setPosition(1000.0f, TOP_BAR_Y);

    _levelText = _textureManager->createText(_fontPath, "LVL: 1", FONT_SIZE);
    if (!_levelText) {
        throw HUDError("Failed to create level text");
    }
    _levelText->setColor(100, 255, 100, 255);
    float levelWidth = _levelText->getWidth();
    _levelText->setPosition(RIGHT_MARGIN - levelWidth, TOP_BAR_Y);

    _healthText = _textureManager->createText(_fontPath, "HP: 100%", FONT_SIZE);
    if (!_healthText) {
        throw HUDError("Failed to create health text");
    }
    _healthText->setColor(0, 255, 0, 255);
    _healthText->setPosition(LEFT_MARGIN, TOP_BAR_Y);

    _weaponText = _textureManager->createText(_fontPath, "WEAPON: Basic", FONT_SIZE);
    if (!_weaponText) {
        throw HUDError("Failed to create weapon text");
    }
    _weaponText->setColor(200, 200, 255, 255);
    _weaponText->setPosition(LEFT_MARGIN, BOTTOM_BAR_Y);
}

void GameHUD::setScore(unsigned int score)
{
    _score = score;
    std::ostringstream oss;
    oss << "SCORE: " << std::setw(5) << std::setfill('0') << score;
    updateText(_scoreText, oss.str());
}

void GameHUD::setHighScore(unsigned int highScore)
{
    _highScore = highScore;
    std::ostringstream oss;
    oss << "HI: " << std::setw(5) << std::setfill('0') << highScore;
    updateText(_highScoreText, oss.str());
}

void GameHUD::setLevel(unsigned int level)
{
    _level = level;
    std::ostringstream oss;
    oss << "LVL: " << level;
    updateText(_levelText, oss.str());

    float levelWidth = _levelText->getWidth();
    _levelText->setPosition(RIGHT_MARGIN - levelWidth, TOP_BAR_Y);
}

void GameHUD::setHealth(unsigned int health)
{
    _health = (health > 100) ? 100 : health;

    std::ostringstream oss;
    oss << "HP: " << _health << "%";
    updateText(_healthText, oss.str());

    if (_health <= 25) {
        _healthText->setColor(255, 0, 0, 255);
    } else if (_health <= 50) {
        _healthText->setColor(255, 165, 0, 255);
    } else if (_health <= 75) {
        _healthText->setColor(255, 255, 0, 255);
    } else {
        _healthText->setColor(0, 255, 0, 255);
    }
}

void GameHUD::setWeapon(const std::string &weaponName, int ammo)
{
    _weaponName = weaponName;
    _ammo = ammo;

    std::ostringstream oss;
    oss << "WEAPON: " << weaponName;
    if (ammo >= 0) {
        oss << " [" << ammo << "]";
    }
    updateText(_weaponText, oss.str());
}

void GameHUD::render(IRenderer &renderer)
{
    if (!_visible) {
        return;
    }

    try {
        if (_healthText) {
            renderer.renderText(*_healthText);
        }
        if (_scoreText) {
            renderer.renderText(*_scoreText);
        }
        if (_highScoreText) {
            renderer.renderText(*_highScoreText);
        }
        if (_levelText) {
            renderer.renderText(*_levelText);
        }
        if (_weaponText) {
            renderer.renderText(*_weaponText);
        }
    } catch (const std::exception &e) {
        throw HUDError("Failed to render HUD: " + std::string(e.what()));
    }
}

void GameHUD::setVisible(bool visible)
{
    _visible = visible;
}

bool GameHUD::isVisible() const
{
    return _visible;
}

void GameHUD::updateText(std::unique_ptr<IText> &text, const std::string &content)
{
    if (text) {
        text->setString(content);
    }
}
