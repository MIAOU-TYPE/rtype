/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GraphicsTypes
*/

#pragma once

namespace Graphics
{
    /**
     * @struct Extent2u
     * @brief Represents a 2D extent with unsigned integer dimensions.
     */
    struct Extent2u {
        unsigned int width;
        unsigned int height;
    };

    /**
     * @enum ColorBlindMode
     * @brief Enumeration of available colorblind filter modes.
     */
    enum class ColorBlindMode {
        NONE,         ///> No filter applied (normal vision)
        DEUTERANOPIA, ///> Red-green color blindness (green deficiency)
        PROTANOPIA,   ///> Red-green color blindness (red deficiency)
        TRITANOPIA    ///> Blue-yellow color blindness
    };
} // namespace Graphics
