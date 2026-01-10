/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AMenu
*/

#pragma once

namespace Engine
{

    template <typename Action>
    Action AMenu::pickAction(const float mx, const float my,
        std::initializer_list<std::pair<UI::UIButton *, Action>> items) noexcept
    {
        for (const auto &it : items) {
            if (!it.first)
                continue;
            const Action act = it.second;
            if (it.first->onMouseReleased(mx, my)) {
                it.first->reset();
                return act;
            }
            if (it.first->bounds().contains(mx, my)) {
                it.first->reset();
                return act;
            }
        }
        return Action::None;
    }

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
