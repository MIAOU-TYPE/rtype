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
    Action AMenu::pickAction(
        const float mx, const float my, std::initializer_list<std::pair<UI::UIButton *, Action>> items) const noexcept
    {
        for (const auto &it : items) {
            UI::UIButton *btn = it.first;
            if (!btn)
                continue;

            if (btn->onMouseReleased(mx, my)) {
                btn->reset();
                return it.second;
            }
        }
        return Action::None;
    }

    template <typename... B>
    void AMenu::resetButtons(B *...b) const noexcept
    {
        ((b ? b->reset() : void()), ...);
    }

    template <typename... B>
    void AMenu::updateButtons(const float mx, const float my, B *...b) const noexcept
    {
        ((b ? b->update(mx, my) : void()), ...);
    }

    template <typename... B>
    void AMenu::pressButtons(const float mx, const float my, B *...b) const noexcept
    {
        ((b ? (void) b->onMousePressed(mx, my) : void()), ...);
    }
} // namespace Engine
