// client/src/ui/gameOver/GameOverMenu.cpp
/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameOverMenu
*/

#include "GameOverMenu.hpp"

namespace Engine
{
    GameOverMenu::GameOverMenu(
        const std::shared_ptr<Graphics::IRenderer> &renderer, std::weak_ptr<World::ClientWorld> world)
        : AMenu(renderer), _world(std::move(world))
    {
        loadBackground("sprites/bg-preview.png");
        _title = _renderer->texts()->createText(64, {255, 255, 255, 255});
        _title->setString("GAME OVER");
        _scoresTitle = _renderer->texts()->createText(28, {255, 255, 255, 255});
        _scoresTitle->setString("SCORES");
        for (int i = 0; i < 4; ++i) {
            auto t = _renderer->texts()->createText(22, {255, 255, 255, 255});
            t->setString("");
            _scoreLines.push_back(std::move(t));
        }
        _back = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "MENU");
        _quit = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "QUIT");
    }

    void GameOverMenu::onEnter()
    {
        _backRequested = false;
        _quitRequested = false;
        resetButtons(_back.get(), _quit.get());
        layout();
    }

    void GameOverMenu::layout()
    {
        layoutBackground();
        const auto v = viewportF();
        if (_title)
            _title->setPosition(v.cx - 135.f, v.h * 0.14f);
        const float titleY = v.h * 0.32f;
        const float lineStartY = v.h * 0.38f;
        if (_scoresTitle)
            _scoresTitle->setPosition(v.cx - 60.f, titleY);
        for (std::size_t i = 0; i < _scoreLines.size(); ++i)
            if (_scoreLines[i])
                _scoreLines[i]->setPosition(v.cx - 260.f, lineStartY + static_cast<float>(i) * 34.f);
        layoutColumnCentered(v.w, v.h * 0.60f, 110.f, {_back.get(), _quit.get()});
    }

    void GameOverMenu::update(const InputFrame &frame)
    {
        updateButtons(frame.mouseX, frame.mouseY, _back.get(), _quit.get());
        if (frame.mousePressed)
            handleMousePressed(frame);
        if (frame.mouseReleased)
            handleMouseReleased(frame);
        if (const auto w = _world.lock()) {
            auto scores = w->getRoomScores();
            if (scores.empty()) {
                if (!_scoreLines.empty())
                    _scoreLines[0]->setString("Player 1: 0 pts");
                for (std::size_t i = 1; i < _scoreLines.size(); ++i)
                    _scoreLines[i]->setString("");
                return;
            }
            std::ranges::sort(scores, [](const auto &a, const auto &b) {
                return a.second > b.second;
            });

            const std::size_t n = std::min(scores.size(), _scoreLines.size());
            for (std::size_t i = 0; i < n; ++i) {
                const std::string name = "Player " + std::to_string(i + 1);
                _scoreLines[i]->setString(name + ": " + std::to_string(scores[i].second) + " pts");
            }
            for (std::size_t i = n; i < _scoreLines.size(); ++i)
                _scoreLines[i]->setString("");
        }
    }

    void GameOverMenu::handleMousePressed(const InputFrame &frame) const
    {
        pressButtons(frame.mouseX, frame.mouseY, _back.get(), _quit.get());
    }

    void GameOverMenu::handleMouseReleased(const InputFrame &frame)
    {
        enum class Action { None, Back, Quit };
        const auto a =
            pickAction<Action>(frame.mouseX, frame.mouseY, {{_back.get(), Action::Back}, {_quit.get(), Action::Quit}});
        if (a == Action::Back)
            _backRequested = true;
        if (a == Action::Quit)
            _quitRequested = true;
    }

    void GameOverMenu::render() const
    {
        renderBackground();
        if (_title)
            _renderer->draw(*_title);
        if (_scoresTitle)
            _renderer->draw(*_scoresTitle);
        for (const auto &t : _scoreLines)
            if (t)
                _renderer->draw(*t);
        _back->render();
        _quit->render();
    }

    bool GameOverMenu::wantsBackToMenu() const noexcept
    {
        return _backRequested;
    }

    bool GameOverMenu::wantsQuit() const noexcept
    {
        return _quitRequested;
    }
} // namespace Engine