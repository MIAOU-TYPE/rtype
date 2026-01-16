/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlTextManager
*/

#pragma once

#include <memory>

#include "ITextManager.hpp"
#include "SfmlFontManager.hpp"
#include "SfmlText.hpp"

namespace Graphics
{
    /**
     * @class SfmlTextManager
     * @brief SFML implementation of ITextManager.
     * Responsible for creating SFML-backed text objects.
     */
    class SfmlTextManager final : public ITextManager {
      public:
        /**
         * @brief Constructor for SfmlTextManager.
         * @param fontManager Shared pointer to the SfmlFontManager for font management.
         */
        explicit SfmlTextManager(std::shared_ptr<SfmlFontManager> fontManager);

        /**
         * @brief Creates a new text object.
         * @param characterSize Font size.
         * @param color Text color.
         * @return A new IText instance with font/size/color already configured.
         */
        [[nodiscard]] std::unique_ptr<IText> createText(unsigned int characterSize, const Color &color) override;

        /**
         * @brief Clears all managed text resources.
         */
        void clear() override;

      private:
        std::shared_ptr<SfmlFontManager> _fontManager; ///> Shared pointer to the font manager.
    };
} // namespace Graphics
