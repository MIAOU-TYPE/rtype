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

    std::unique_ptr<IText> SfmlTextManager::createText()
    {
        return std::make_unique<SfmlText>(InvalidFont, _fontManager);
    }

    void SfmlTextManager::clear()
    {
    }
} // namespace Graphics
