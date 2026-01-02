/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlTextManager
*/

#include "SfmlTextManager.hpp"

namespace Graphics
{
    SfmlTextManager::SfmlTextManager(std::shared_ptr<SfmlFontManager> fontManager)
        : _fontManager(std::move(fontManager))
    {
    }

    std::unique_ptr<IText> SfmlTextManager::createText(const unsigned int characterSize, const Color &color)
    {
        auto text = std::make_unique<SfmlText>(InvalidFont, _fontManager);

        text->setCharacterSize(characterSize);
        text->setColor(color);
        if (_fontManager) {
            const FontHandle font = _fontManager->load("fonts/font.ttf");
            text->setFont(font);
        }
        return text;
    }

    void SfmlTextManager::clear()
    {
    }
} // namespace Graphics
