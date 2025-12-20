/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** ITextManager
*/

#pragma once

#include <memory>
#include "IText.hpp"

namespace Graphics
{
    /**
     * @class ITextManager
     * @brief Factory and lifetime manager for text objects.
     * This interface is responsible for creating backend-specific
     * text instances while keeping the engine backend-agnostic.
     */
    class ITextManager {
      public:
        virtual ~ITextManager() = default;

        /**
         * @brief Create a new text object.
         * The returned text is independent and fully configurable
         * through the IText interface.
         */
        virtual std::unique_ptr<IText> createText() = 0;

        /**
         * @brief Destroy all texts managed by this manager.
         */
        virtual void clear() = 0;
    };
} // namespace Graphics
