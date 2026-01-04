/*
** EPITECH PROJECT, 2025
** UIButton.tpp
** File description:
** UIButton
*/

#pragma once

namespace UI
{
    template <typename Fn>
    bool UIButton::onClickReleased(const float x, const float y, Fn &&fn)
    {
        if (!onMouseReleased(x, y))
            return false;
        reset();
        std::forward<Fn>(fn)();
        return true;
    }

    template <typename Fn>
    bool UIButton::onClickPressed(const float x, const float y, Fn &&fn)
    {
        if (!onMousePressed(x, y))
            return false;
        std::forward<Fn>(fn)();
        return true;
    }
} // namespace UI
