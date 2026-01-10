/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AMenu
*/

#pragma once

namespace Engine
{

    template <typename... B>
    void AMenu::resetButtons(B *...b) noexcept
    {
        ((b ? b->reset() : void()), ...);
    }

    template <typename... B>
    void AMenu::updateButtons(const float mx, const float my, B *...b) noexcept
    {
        ((b ? b->update(mx, my) : void()), ...);
    }

    template <typename... B>
    void AMenu::pressButtons(const float mx, const float my, B *...b) noexcept
    {
        ((b ? (void)b->onMousePressed(mx, my) : void()), ...);
    }
} // namespace Engine
