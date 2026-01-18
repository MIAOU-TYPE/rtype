/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby
*/

#include "Lobby.hpp"

namespace
{
    [[nodiscard]] std::string twoDigits(const size_t v)
    {
        if (v < 10)
            return "0" + std::to_string(v);
        return std::to_string(v);
    }
} // namespace

namespace Engine
{
    Lobby::Lobby(const std::shared_ptr<Graphics::IRenderer> &renderer) : AMenu(renderer)
    {
        loadBackground("sprites/bg-preview.png");
        loadPanel("sprites/popup.png", true);

        _leaveBtn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "LEAVE");
        _startBtn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "START");

        _titleText = _renderer->texts()->createText(40, {255, 255, 255, 255});
        _titleText->setString("LOBBY");

        _subtitleText = _renderer->texts()->createText(20, {180, 220, 255, 255});
        _subtitleText->setString("");

        _dividerText = _renderer->texts()->createText(18, {150, 190, 230, 255});
        _dividerText->setString("------------------------------");

        _playersHeaderText = _renderer->texts()->createText(18, {200, 235, 255, 255});
        _playersHeaderText->setString("PLAYERS");

        _playersCountText = _renderer->texts()->createText(18, {200, 235, 255, 255});
        _playersCountText->setString("0/0");

        _statusText = _renderer->texts()->createText(18, {255, 140, 140, 255});
        _statusText->setString("");

        _hintText = _renderer->texts()->createText(18, {190, 190, 190, 255});
        _hintText->setString("");

        _playerTexts.reserve(_lobbyCapacity);
        for (size_t i = 0; i < _lobbyCapacity; ++i) {
            auto t = _renderer->texts()->createText(24, {255, 255, 255, 255});
            t->setString("");
            _playerTexts.push_back(std::move(t));
        }

        rebuildTexts();
        layout();
    }

    void Lobby::onEnter()
    {
        _startRequested = false;
        _leaveRequested = false;
        resetButtons(_startBtn.get(), _leaveBtn.get());
        rebuildTexts();
        layout();
    }

    void Lobby::setLobbyName(std::string name)
    {
        _lobbyName = std::move(name);
        rebuildTexts();
        layout();
    }

    void Lobby::setPlayers(std::vector<std::string> players)
    {
        _players = std::move(players);
        rebuildTexts();
        layout();
    }

    void Lobby::setStartEnabled(const bool v)
    {
        _canStart = v;
        rebuildTexts();
        layout();
    }

    void Lobby::setMaxPlayers(const size_t maxPlayers)
    {
        _lobbyCapacity = maxPlayers;
        _playerTexts.clear();
        _playerTexts.reserve(_lobbyCapacity);
        for (size_t i = 0; i < _lobbyCapacity; ++i) {
            auto t = _renderer->texts()->createText(24, {255, 255, 255, 255});
            t->setString("");
            _playerTexts.push_back(std::move(t));
        }
        rebuildTexts();
        layout();
    }

    bool Lobby::wantsStart() const noexcept
    {
        return _startRequested;
    }

    bool Lobby::wantsLeave() const noexcept
    {
        return _leaveRequested;
    }

    bool Lobby::needsUpdate() const noexcept
    {
        return _needUpdate;
    }

    void Lobby::consumeStart() noexcept
    {
        _startRequested = false;
    }

    void Lobby::consumeLeave() noexcept
    {
        _leaveRequested = false;
    }

    void Lobby::consumeUpdate() noexcept
    {
        _needUpdate = false;
    }

    void Lobby::rebuildTexts() const
    {
        if (_subtitleText) {
            _subtitleText->setString(_lobbyName.empty() ? "Waiting room" : _lobbyName);
        }

        if (_playersCountText)
            _playersCountText->setString(std::to_string(_players.size()) + "/" + std::to_string(_lobbyCapacity));

        for (const auto &t : _playerTexts)
            t->setString("");

        if (_players.empty()) {
            if (!_playerTexts.empty())
                _playerTexts.at(0)->setString("— waiting for players —");
        } else {
            const size_t n = std::min<size_t>(_players.size(), _playerTexts.size());
            for (size_t i = 0; i < n; ++i)
                _playerTexts.at(i)->setString(twoDigits(i + 1) + "  " + _players.at(i));

            for (size_t i = n; i < _playerTexts.size(); ++i)
                _playerTexts.at(i)->setString(twoDigits(i + 1) + "  — empty —");
        }

        if (_statusText)
            _statusText->setString(_canStart ? "" : "Only the host can start.");

        if (_hintText)
            _hintText->setString(_canStart ? "Press ENTER or click START" : "Waiting for the host...");
    }

    void Lobby::layout()
    {
        layoutBackground();
        layoutPanel();
        const auto vp = viewportF();
        const float ui = std::clamp(std::min(vp.w / 1280.f, vp.h / 720.f), 0.55f, 1.0f);
        _startBtn->setUIScale(ui, ui);
        _leaveBtn->setUIScale(ui, ui);

        const auto inner = innerRect();

        const float paddingL = inner.w * 0.10f;
        const float xLeft = inner.x + paddingL;
        const float xRight = inner.x + inner.w - paddingL;

        if (_titleText)
            _titleText->setPosition(inner.cx() - _titleText->getWidth() * 0.5f, inner.y + inner.h * 0.10f - 115.f);
        const float ySubtitle = inner.y + inner.h * 0.16f;
        const float yDivider = inner.y + inner.h * 0.22f;
        const float yHeader = inner.y + inner.h * 0.30f;
        if (_subtitleText)
            _subtitleText->setPosition(xLeft, ySubtitle);
        if (_dividerText)
            _dividerText->setPosition(xLeft, yDivider);
        if (_playersHeaderText)
            _playersHeaderText->setPosition(xLeft, yHeader);
        if (_playersCountText)
            _playersCountText->setPosition(xRight - _playersCountText->getWidth(), yHeader);

        const float startY = inner.y + inner.h * 0.36f;
        constexpr float lineH = 34.f;
        for (size_t i = 0; i < _playerTexts.size(); ++i) {
            const auto &t = _playerTexts.at(i);
            if (t)
                t->setPosition(xLeft, startY + static_cast<float>(i) * lineH);
        }

        if (_statusText)
            _statusText->setPosition(xLeft, inner.y + inner.h * 0.74f);
        if (_hintText)
            _hintText->setPosition(xLeft, inner.y + inner.h * 0.78f);

        const float buttonsY = inner.y + inner.h * 0.90f;
        const float bwLeave = _leaveBtn->bounds().w;
        const float gap = inner.w * 0.04f;

        _leaveBtn->setPosition(xLeft, buttonsY);
        _startBtn->setPosition(xLeft + bwLeave + gap, buttonsY);
    }

    void Lobby::update(const InputFrame &frame)
    {
        handleInput(frame);
        updateButtons(frame.mouseX, frame.mouseY, _startBtn.get(), _leaveBtn.get());

        const auto now = std::chrono::steady_clock::now();
        if (_lastRefresh.time_since_epoch().count() == 0)
            _lastRefresh = now;
        if (now - _lastRefresh >= refreshPeriod) {
            _lastRefresh = now;
            _needUpdate = true;
        }
    }

    void Lobby::render() const
    {
        renderBackground();
        if (_panelTex != Graphics::InvalidTexture)
            _renderer->draw(_panelCmd);

        if (_titleText)
            _renderer->draw(*_titleText);
        if (_subtitleText)
            _renderer->draw(*_subtitleText);
        if (_dividerText)
            _renderer->draw(*_dividerText);

        if (_playersHeaderText)
            _renderer->draw(*_playersHeaderText);
        if (_playersCountText)
            _renderer->draw(*_playersCountText);

        for (const auto &t : _playerTexts)
            if (t)
                _renderer->draw(*t);

        if (_statusText)
            _renderer->draw(*_statusText);
        if (_hintText)
            _renderer->draw(*_hintText);

        _startBtn->render();
        _leaveBtn->render();
    }

    void Lobby::handleInput(const InputFrame &frame)
    {
        if (frame.mousePressed)
            handleMousePressed(frame);
        if (frame.mouseReleased)
            handleMouseReleased(frame);
        if (frame.keyPressed)
            handleKeyPressed(frame);
    }

    void Lobby::handleMousePressed(const InputFrame &frame) const
    {
        pressButtons(frame.mouseX, frame.mouseY, _startBtn.get(), _leaveBtn.get());
    }

    void Lobby::handleMouseReleased(const InputFrame &frame)
    {
        enum class Action { None, Start, Leave };
        const auto start = pickAction<Action>(
            frame.mouseX, frame.mouseY, {{_startBtn.get(), Action::Start}, {_leaveBtn.get(), Action::Leave}});

        if (start == Action::Start && _canStart)
            _startRequested = true;
        if (start == Action::Leave)
            _leaveRequested = true;
    }

    void Lobby::handleKeyPressed(const InputFrame &frame)
    {
        if (frame.key == Key::Enter && _canStart)
            _startRequested = true;
    }
} // namespace Engine
