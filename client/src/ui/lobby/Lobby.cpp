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

    [[nodiscard]] float maxWidth(const std::initializer_list<std::shared_ptr<Graphics::IText>> &texts)
    {
        float m = 0.f;
        for (const auto &t : texts) {
            if (!t)
                continue;
            m = std::max(m, static_cast<float>(t->getWidth()));
        }
        return m;
    }
} // namespace

namespace Engine
{
    Lobby::Lobby(const std::shared_ptr<Graphics::IRenderer> &renderer) : AMenu(renderer)
    {
        loadBackground("sprites/bg-preview.png");

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

        _playerTexts.reserve(LobbyCapacity);
        for (size_t i = 0; i < LobbyCapacity; ++i) {
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
        resetButtons(_startBtn.get());
        resetButtons(_leaveBtn.get());
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
        LobbyCapacity = maxPlayers;
        _playerTexts.clear();
        _playerTexts.reserve(LobbyCapacity);
        for (size_t i = 0; i < LobbyCapacity; ++i) {
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
            if (_lobbyName.empty())
                _subtitleText->setString("Waiting room");
            else
                _subtitleText->setString(_lobbyName);
        }

        if (_playersCountText)
            _playersCountText->setString(std::to_string(_players.size()) + "/" + std::to_string(LobbyCapacity));

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

        if (_statusText) {
            if (_canStart)
                _statusText->setString("");
            else
                _statusText->setString("Only the host can start.");
        }

        if (_hintText) {
            if (_canStart)
                _hintText->setString("Press ENTER or click START");
            else
                _hintText->setString("Waiting for the host...");
        }
    }

    void Lobby::layout()
    {
        const auto vp = viewportF();
        const float w = vp.w;
        const float h = vp.h;

        layoutBackground();

        if (_titleText)
            _titleText->setPosition(vp.cx - _titleText->getWidth() * 0.5f, h * 0.16f);
        if (_subtitleText)
            _subtitleText->setPosition(vp.cx - _subtitleText->getWidth() * 0.5f, h * 0.23f);
        if (_dividerText)
            _dividerText->setPosition(vp.cx - _dividerText->getWidth() * 0.5f, h * 0.28f);

        float listMaxW = 0.f;
        for (const auto &t : _playerTexts)
            if (t)
                listMaxW = std::max(listMaxW, t->getWidth());

        const float headerRowW = maxWidth({_playersHeaderText, _playersCountText});
        const float contentW = std::max(listMaxW, headerRowW);
        const float xLeft = std::clamp(vp.cx - contentW * 0.5f, w * 0.12f, w * 0.88f - contentW);

        if (_playersHeaderText)
            _playersHeaderText->setPosition(xLeft, h * 0.34f);
        if (_playersCountText)
            _playersCountText->setPosition(xLeft + contentW - _playersCountText->getWidth(), h * 0.34f);

        const float startY = h * 0.40f;
        for (size_t i = 0; i < _playerTexts.size(); ++i) {
            constexpr float lineH = 34.f;
            const auto &t = _playerTexts.at(i);
            if (!t)
                continue;
            t->setPosition(xLeft, startY + static_cast<float>(i) * lineH);
        }

        if (_statusText)
            _statusText->setPosition(vp.cx - _statusText->getWidth() * 0.5f, h * 0.78f);
        if (_hintText)
            _hintText->setPosition(vp.cx - _hintText->getWidth() * 0.5f, h * 0.82f);

        placeCentered(*_startBtn, vp.cx, h * 0.80f);
        placeCentered(*_leaveBtn, vp.cx, h * 0.90f);
    }

    void Lobby::update(const InputFrame &frame)
    {
        handleInput(frame);
        updateButtons(frame.mouseX, frame.mouseY, _startBtn.get());
        updateButtons(frame.mouseX, frame.mouseY, _leaveBtn.get());

        const auto now = std::chrono::steady_clock::now();
        if (lastRefresh.time_since_epoch().count() == 0)
            lastRefresh = now;
        if (now - lastRefresh >= refreshPeriod) {
            lastRefresh = now;
            _needUpdate = true;
        }
    }

    void Lobby::render() const
    {
        renderBackground();

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
        pressButtons(frame.mouseX, frame.mouseY, _startBtn.get());
        pressButtons(frame.mouseX, frame.mouseY, _leaveBtn.get());
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
