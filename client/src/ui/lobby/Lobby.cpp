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

        _chatField = std::make_unique<UI::UITextField>(_renderer, "Type a message...", false);
        _chatSendBtn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "SEND");

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

        _chatHeaderText = _renderer->texts()->createText(18, {200, 235, 255, 255});
        _chatHeaderText->setString("CHAT");

        _chatTexts.reserve(_chatCapacity);
        for (size_t i = 0; i < _chatCapacity; ++i) {
            auto t = _renderer->texts()->createText(18, {255, 255, 255, 255});
            t->setString("");
            _chatTexts.push_back(std::move(t));
        }

        rebuildTexts();
        layout();
    }

    void Lobby::onEnter()
    {
        _startRequested = false;
        resetButtons(_startBtn.get());
        resetButtons(_leaveBtn.get());
        _chatSubmitted = false;
        _chatSubmittedMessage.clear();
        if (_chatField) {
            _chatField->clear();
            _chatField->setFocused(false);
        }
        resetButtons(_chatSendBtn.get());

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
            if (_lobbyName.empty())
                _subtitleText->setString("Waiting room");
            else
                _subtitleText->setString(_lobbyName);
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

        for (auto &t : _chatTexts)
            if (t)
                t->setString("");

        const size_t visible = (_chatVisible == 0) ? _chatCapacity : _chatVisible;
        const size_t n = std::min(_chatMessages.size(), visible);
        const size_t start = (_chatMessages.size() > n) ? (_chatMessages.size() - n) : 0;

        const size_t pad = visible - n;
        for (size_t i = 0; i < n; ++i) {
            _chatTexts.at(pad + i)->setString(_chatMessages.at(start + i));
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

        const float playersX = vp.cx - contentW * 0.5f;

        if (_playersHeaderText)
            _playersHeaderText->setPosition(playersX, h * 0.34f);
        if (_playersCountText)
            _playersCountText->setPosition(playersX + contentW - _playersCountText->getWidth(), h * 0.34f);

        const float startY = h * 0.40f;
        for (size_t i = 0; i < _playerTexts.size(); ++i) {
            constexpr float playerLineH = 34.f;
            const auto &t = _playerTexts.at(i);
            if (!t)
                continue;
            t->setPosition(playersX, startY + static_cast<float>(i) * playerLineH);
        }

        if (_statusText)
            _statusText->setPosition(vp.cx - _statusText->getWidth() * 0.5f, h * 0.78f);
        if (_hintText)
            _hintText->setPosition(vp.cx - _hintText->getWidth() * 0.5f, h * 0.82f);

        placeCentered(*_startBtn, vp.cx, h * 0.80f);
        placeCentered(*_leaveBtn, vp.cx, h * 0.90f);

        constexpr float marginX = 24.f;
        constexpr float marginY = 24.f;
        constexpr float headerGap = 24.f;
        constexpr float chatLineH = 20.f;

        const float chatW = w * 0.30f;
        const float chatX = (w - marginX) - chatW;

        const float leaveBtnTopY = (h * 0.90f) - 70.f;
        const float chatBottomY = leaveBtnTopY - marginY;

        const float chatTopY = std::max(h * 0.55f, chatBottomY - 220.f);

        if (_chatHeaderText) {
            _chatHeaderText->setPosition(chatX + chatW - _chatHeaderText->getWidth(), chatTopY);
        }

        const float linesTopY = chatTopY + headerGap;
        constexpr float inputH = 42.f;
        constexpr float inputMarginTop = 6.f;
        const float availableH = chatBottomY - linesTopY - (inputMarginTop + inputH);
        size_t maxLines = 0;
        if (availableH > 0.f)
            maxLines = static_cast<size_t>(availableH / chatLineH);

        _chatVisible = std::min(_chatCapacity, maxLines);

        for (size_t i = 0; i < _chatTexts.size(); ++i) {
            const auto &t = _chatTexts[i];
            if (!t)
                continue;

            if (i < _chatVisible) {
                const float y = linesTopY + static_cast<float>(i) * chatLineH;
                const float x = chatX + chatW - t->getWidth();
                t->setPosition(x, y);
            } else {
                t->setPosition(-10000.f, -10000.f);
            }
        }

        constexpr float inputGap = 10.f;
        constexpr float sendW = 110.f;

        const float inputY = linesTopY + static_cast<float>(_chatVisible) * chatLineH + 6.f;

        if (_chatField) {
            _chatField->setPosition(chatX, inputY);
            _chatField->setWidth(chatW - sendW - inputGap);
        }

        if (_chatSendBtn) {
            const float sendCx = chatX + chatW - sendW * 0.5f;
            const float sendCy = inputY + inputH * 0.5f;
            placeCentered(*_chatSendBtn, sendCx, sendCy);
        }
    }

    void Lobby::update(const InputFrame &frame)
    {
        updateButtons(frame.mouseX, frame.mouseY, _startBtn.get(), _leaveBtn.get(), _chatSendBtn.get());
        handleInput(frame);

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

        if (_chatHeaderText)
            _renderer->draw(*_chatHeaderText);

        for (const auto &t : _chatTexts)
            if (t)
                _renderer->draw(*t);
        if (_chatField)
            _chatField->render();
        if (_chatSendBtn)
            _chatSendBtn->render();
    }

    void Lobby::handleInput(const InputFrame &frame)
    {
        if (frame.mousePressed)
            handleMousePressed(frame);
        if (frame.mouseReleased)
            handleMouseReleased(frame);
        if (frame.keyPressed)
            handleKeyPressed(frame);
        if (frame.keyReleased)
            handleKeyReleased(frame);
    }

    void Lobby::handleMousePressed(const InputFrame &frame) const
    {
        pressButtons(frame.mouseX, frame.mouseY, _startBtn.get());
        pressButtons(frame.mouseX, frame.mouseY, _leaveBtn.get());
        pressButtons(frame.mouseX, frame.mouseY, _chatSendBtn.get());

        if (_chatField)
            _chatField->onMousePressed(frame.mouseX, frame.mouseY);
    }

    void Lobby::handleMouseReleased(const InputFrame &frame)
    {
        enum class Action { None, Start, Leave, SendChat };
        const auto a = pickAction<Action>(frame.mouseX, frame.mouseY,
            {{_startBtn.get(), Action::Start}, {_leaveBtn.get(), Action::Leave},
                {_chatSendBtn.get(), Action::SendChat}});

        if (a == Action::Start && _canStart)
            _startRequested = true;
        if (a == Action::Leave)
            _leaveRequested = true;
        if (a == Action::SendChat) {
            if (_chatField && !_chatField->value().empty()) {
                _chatSubmitted = true;
                _chatSubmittedMessage = _chatField->value();
                _chatField->clear();
                _chatField->setFocused(true);
            }
        }
    }

    void Lobby::handleKeyPressed(const InputFrame &frame)
    {
        if (_chatField && _chatField->isFocused()) {
            if (frame.key == Key::Enter) {
                if (!_chatField->value().empty()) {
                    _chatSubmitted = true;
                    _chatSubmittedMessage = _chatField->value();
                    _chatField->clear();
                    _chatField->setFocused(true);
                }
                return;
            }
            if (frame.key == Key::Escape) {
                _chatField->setFocused(false);
                return;
            }
        }
        if (frame.key == Key::Enter && _canStart)
            _startRequested = true;
    }

    void Lobby::handleKeyReleased(const InputFrame &frame) const
    {
        if (_chatField && _chatField->isFocused())
            _chatField->onKeyPressed(frame.key);
    }
} // namespace Engine
