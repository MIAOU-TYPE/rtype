/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AMenuS
*/

#include "AMenu.hpp"

namespace Engine
{
    AMenu::AMenu(std::shared_ptr<Graphics::IRenderer> renderer) : _renderer(std::move(renderer))
    {
        if (!_renderer)
            throw std::runtime_error("{AMenu} null renderer");
    }

    AMenu::ViewportF AMenu::viewportF() const noexcept
    {
        const auto [width, height] = _renderer->getViewportSize();
        const auto w = static_cast<float>(width);
        const auto h = static_cast<float>(height);
        return {w, h, w * 0.5f, h * 0.5f};
    }

    void AMenu::loadBackground(const std::string &path)
    {
        const auto textures = _renderer->textures();
        _backgroundTexture = textures->load(path);
        if (_backgroundTexture == Graphics::InvalidTexture)
            throw std::runtime_error("{AMenu} failed to load background: " + path);
        _backgroundCmd.textureId = _backgroundTexture;
    }

    void AMenu::layoutBackground()
    {
        if (_backgroundTexture == Graphics::InvalidTexture)
            return;
        const auto [width, height] = _renderer->getViewportSize();
        const auto [twidth, theight] = _renderer->textures()->getSize(_backgroundTexture);
        const auto w = static_cast<float>(width);
        const auto h = static_cast<float>(height);
        _backgroundCmd.frame = {0, 0, static_cast<int>(twidth), static_cast<int>(theight)};
        _backgroundCmd.position = {0.f, 0.f};
        _backgroundCmd.scale = {w / static_cast<float>(twidth), h / static_cast<float>(theight)};
    }

    void AMenu::renderBackground() const
    {
        if (_backgroundTexture != Graphics::InvalidTexture)
            _renderer->draw(_backgroundCmd);
    }

    void AMenu::placeCentered(UI::UIButton &b, const float cx, const float y) noexcept
    {
        b.setPosition(cx - b.bounds().w * 0.5f, y);
    }

    void AMenu::layoutStepper(UI::UIButton &label, UI::UIButton &down, UI::UIButton &up, const float colX,
        const float y, const float margin) noexcept
    {
        const float labelHalf = label.bounds().w * 0.5f;
        const float btnHalf = down.bounds().w * 0.5f;
        label.setPosition(colX - labelHalf, y);
        down.setPosition(colX - (labelHalf + margin + btnHalf) - btnHalf, y);
        up.setPosition(colX + (labelHalf + margin + btnHalf) - btnHalf, y);
    }

    void AMenu::layoutChoiceWithNext(UI::UIButton &mainBtn, UI::UIButton &nextBtn, const float colX, const float y,
        const float labelMargin, const float spacingX, const float viewW, const float rightMargin) noexcept
    {
        const float mainHalf = mainBtn.bounds().w * 0.5f;
        const float leftX = colX - (mainHalf + labelMargin);
        mainBtn.setPosition(leftX, y);

        const float maxX = viewW - nextBtn.bounds().w - rightMargin;
        const float desiredX = colX + (mainHalf + labelMargin) + spacingX;
        nextBtn.setPosition(std::min(desiredX, maxX), y);
    }

    void AMenu::layoutRowCentered(
        UI::UIButton &left, UI::UIButton &right, const float cx, const float y, const float gap) noexcept
    {
        const float total = left.bounds().w + gap + right.bounds().w;
        const float startX = cx - total * 0.5f;
        left.setPosition(startX, y);
        right.setPosition(startX + left.bounds().w + gap, y);
    }

    void AMenu::layoutColumnCentered(const float viewW, const float yStart, const float yStep,
        std::initializer_list<UI::UIButton *> buttons) noexcept
    {
        float y = yStart;
        for (auto *b : buttons) {
            if (b)
                b->setPosition((viewW - b->bounds().w) * 0.5f, y);
            y += yStep;
        }
    }
} // namespace Engine
