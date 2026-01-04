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
        FontHandle font = InvalidFont;
        if (_fontManager)
            font = _fontManager->load("fonts/font.ttf");
        auto text = std::make_unique<SfmlText>(font, _fontManager);
        text->setCharacterSize(characterSize);
        text->setColor(color);
        return text;
    }

    void SfmlTextManager::clear()
    {
    }
} // namespace Graphics
