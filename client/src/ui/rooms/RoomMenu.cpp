/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomMenu
*/

#include "RoomMenu.hpp"

namespace
{
    Engine::Difficulty shiftDifficulty(Engine::Difficulty d, const int step) noexcept
    {
        int v = static_cast<int>(d);
        v = (v + step) % 3;
        if (v < 0)
            v += 3;
        return static_cast<Engine::Difficulty>(v);
    }

    std::string_view difficultyToStringUI(Engine::Difficulty d) noexcept
    {
        switch (d) {
            case Engine::Difficulty::Easy: return "easy";
            case Engine::Difficulty::Medium: return "medium";
            case Engine::Difficulty::Hard: return "hard";
        }
        return "unknown";
    }

    [[nodiscard]] bool isVisible(const Engine::FloatRect &rect, const float top, const float bottom) noexcept
    {
        return rect.y >= top && (rect.y + rect.h) <= bottom;
    }

    [[nodiscard]] ListMetrics computeListMetrics(
        const float h, const std::unordered_map<uint32_t, std::unique_ptr<UI::UIButton>> &buttons)
    {
        ListMetrics m{};
        m.listTop = h * 0.22f;
        m.listBottom = h * 0.78f;
        m.listH = m.listBottom - m.listTop;

        float rowH = 80.f;
        if (!buttons.empty()) {
            const auto &first = buttons.begin()->second;
            rowH = first->bounds().h * 1.2f;
        }
        m.rowH = rowH;
        m.scrollStep = rowH;

        m.contentH = m.rowH * static_cast<float>(buttons.size());
        m.maxScroll = std::max(0.f, m.contentH - m.listH);
        return m;
    }

    [[nodiscard]] float snapScroll(float scroll, const float step, const float maxScroll)
    {
        scroll = std::clamp(scroll, 0.f, maxScroll);
        if (step > 0.f) {
            scroll = std::round(scroll / step) * step;
            scroll = std::clamp(scroll, 0.f, maxScroll);
        }
        return scroll;
    }

    void placeButtons(const float cx, const float yStart, const float rowH,
        const std::unordered_map<uint32_t, std::unique_ptr<UI::UIButton>> &buttons)
    {
        float y = yStart;
        for (const auto &btn : buttons | std::views::values) {
            const auto b = btn->bounds();
            btn->setPosition(cx - b.w * 0.5f, y);
            y += rowH;
        }
    }

    [[nodiscard]] float computeVisibleBlockOffset(const float listTop, const float listBottom, const float listH,
        const std::unordered_map<uint32_t, std::unique_ptr<UI::UIButton>> &buttons)
    {
        float visTop = 0.f;
        float visBottom = 0.f;
        bool hasVisible = false;

        for (const auto &btn : buttons | std::views::values) {
            const auto b = btn->bounds();
            const float top = b.y;
            const float btm = top + b.h;
            if (top < listTop || btm > listBottom)
                continue;

            if (!hasVisible) {
                visTop = top;
                visBottom = btm;
                hasVisible = true;
            } else {
                visTop = std::min(visTop, top);
                visBottom = std::max(visBottom, btm);
            }
        }

        if (!hasVisible)
            return 0.f;

        const float visH = visBottom - visTop;
        const float targetTop = listTop + (listH - visH) * 0.5f;
        return targetTop - visTop;
    }

    void applyYOffset(const float offset, const std::unordered_map<uint32_t, std::unique_ptr<UI::UIButton>> &buttons)
    {
        if (offset == 0.f)
            return;

        for (const auto &btn : buttons | std::views::values) {
            const auto b = btn->bounds();
            btn->setPosition(b.x, b.y + offset);
        }
    }

} // namespace

namespace Engine
{
    RoomMenu::RoomMenu(
        const std::shared_ptr<Graphics::IRenderer> &renderer, const std::shared_ptr<RoomManager> &roomManager)
        : AMenu(renderer), _roomManager(roomManager)
    {
        loadBackground("sprites/bg-preview.png");

        _header.title = _renderer->texts()->createText(64, {255, 255, 255, 255});
        _header.title->setString("ROOMS");
        _header.subtitle = _renderer->texts()->createText(24, {200, 200, 200, 255});

        _root.create = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "CREATE");
        _root.join = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "JOIN");
        _root.back = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");

        _create.roomNameField = std::make_unique<UI::UITextField>(_renderer, "Room name", false);

        _create.worldPrev = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "<");
        _create.worldNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, ">");
        _create.levelPrev = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "<");
        _create.levelNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, ">");
        _create.difficultyPrev = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "<");
        _create.difficultyNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, ">");
        _create.playersPrev = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "-");
        _create.playersNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        _create.confirm = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "CREATE");
        _create.back = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");

        _create.worldLabel = _renderer->texts()->createText(32, {255, 255, 255, 255});
        _create.levelLabel = _renderer->texts()->createText(32, {255, 255, 255, 255});
        _create.difficultyLabel = _renderer->texts()->createText(32, {255, 255, 255, 255});
        _create.playersLabel = _renderer->texts()->createText(32, {255, 255, 255, 255});

        _list.back = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");

        refreshCreateCatalog();
        _layoutDirty = true;
    }

    void RoomMenu::layout()
    {
        auto centerX = [&](UI::UIButton &b, float x, float y) {
            b.setPosition(x - b.bounds().w * 0.5f, y);
        };
        const auto vp = _renderer->getViewportSize();
        const auto w = static_cast<float>(vp.width);
        const auto h = static_cast<float>(vp.height);
        const float cx = w * 0.5f;

        updateTextStrings();
        layoutBackground();

        _header.title->setPosition(cx - _header.title->getWidth() * 0.5f, h * 0.07f);
        _header.subtitle->setPosition(cx - _header.subtitle->getWidth() * 0.5f, h * 0.16f);

        if (_page == Page::Root) {
            centerX(*_root.create, cx, h * 0.35f);
            centerX(*_root.join, cx, h * 0.51f);
            centerX(*_root.back, cx, h * 0.67f);
            return;
        }
        if (_page == Page::Create)
            layoutCreate(w, h, cx);
        if (_page == Page::List)
            layoutList(h, cx);
    }

    void RoomMenu::layoutCreate(const float w, const float h, const float cx) const
    {
        auto centerX = [&](UI::UIButton &b, float x, float y) {
            b.setPosition(x - b.bounds().w * 0.5f, y);
        };

        // Room name input
        if (_create.roomNameField) {
            const float fieldW = std::min(520.f, w * 0.70f);
            const float fieldX = cx - fieldW * 0.5f;
            _create.roomNameField->setPosition(fieldX, h * 0.24f);
            _create.roomNameField->setWidth(fieldW);
        }

        // Shift rows down a bit to make space for the input
        auto row = [&](UI::UIButton &prev, UI::UIButton &next, Graphics::IText &label, int i) {
            const float y = h * 0.36f + h * 0.10f * static_cast<float>(i);
            prev.centerButtonLabel(w * 0.25f, y, label, cx);
            centerX(next, w * 0.75f, y);
        };

        row(*_create.worldPrev, *_create.worldNext, *_create.worldLabel, 0);
        row(*_create.levelPrev, *_create.levelNext, *_create.levelLabel, 1);
        row(*_create.difficultyPrev, *_create.difficultyNext, *_create.difficultyLabel, 2);
        row(*_create.playersPrev, *_create.playersNext, *_create.playersLabel, 3);

        centerX(*_create.confirm, cx, h * 0.80f);
        centerX(*_create.back, cx, h * 0.90f);
    }

    void RoomMenu::update(const InputFrame &frame)
    {
        const Page oldPage = _page;
        if (_layoutDirty) {
            layout();
            _layoutDirty = false;
        }

        handleInput(frame);

        if (_page != oldPage) {
            layout();
            _layoutDirty = false;
        }

        updateHover(frame.mouseX, frame.mouseY);

        if (_page != Page::List)
            return;

        const auto now = std::chrono::steady_clock::now();
        if (_list.lastRefresh.time_since_epoch().count() == 0)
            _list.lastRefresh = now;

        if (now - _list.lastRefresh >= _list.refreshPeriod) {
            _listRooms = true;
            updateListRooms();
            _layoutDirty = true;
            _list.lastRefresh = now;
        }

        if (frame.keyPressed) {
            if (frame.key == Key::Up) {
                _list.scroll -= _list.scrollStep;
                _layoutDirty = true;
            } else if (frame.key == Key::Down) {
                _list.scroll += _list.scrollStep;
                _layoutDirty = true;
            }
        }
    }

    void RoomMenu::updateHover(const float mx, const float my) const
    {
        auto each = [&](auto &&fn) {
            if (_page == Page::Root) {
                fn(*_root.create);
                fn(*_root.join);
                fn(*_root.back);
            } else if (_page == Page::Create) {
                fn(*_create.worldPrev);
                fn(*_create.worldNext);
                fn(*_create.levelPrev);
                fn(*_create.levelNext);
                fn(*_create.difficultyPrev);
                fn(*_create.difficultyNext);
                fn(*_create.playersPrev);
                fn(*_create.playersNext);
                fn(*_create.confirm);
                fn(*_create.back);
            } else if (_page == Page::List) {
                fn(*_list.back);
                for (const auto &btn : _list.roomButtons | std::views::values) {
                    if (isVisible(btn->bounds(), _list.listTop, _list.listBottom))
                        fn(*btn);
                }
            }
        };
        each([&](UI::UIButton &b) {
            b.update(mx, my);
        });
    }

    void RoomMenu::handleInput(const InputFrame &frame)
    {
        if (frame.mousePressed)
            handleMousePressed(frame);
        if (frame.mouseReleased)
            handleMouseReleased(frame.mouseX, frame.mouseY);
        if (frame.keyPressed)
            handleKeyPressed(frame);
        if (frame.keyReleased)
            handleKeyReleased(frame);
    }

    void RoomMenu::handleMousePressed(const InputFrame &frame) const
    {
        auto press = [&](UI::UIButton &b) {
            b.onMousePressed(frame.mouseX, frame.mouseY);
        };

        if (_page == Page::Root) {
            press(*_root.create);
            press(*_root.join);
            press(*_root.back);
            return;
        }

        if (_page == Page::Create) {
            press(*_create.worldPrev);
            press(*_create.worldNext);
            press(*_create.levelPrev);
            press(*_create.levelNext);
            press(*_create.difficultyPrev);
            press(*_create.difficultyNext);
            press(*_create.playersPrev);
            press(*_create.playersNext);
            press(*_create.confirm);
            press(*_create.back);

            if (_create.roomNameField)
                _create.roomNameField->onMousePressed(frame.mouseX, frame.mouseY);

            return;
        }

        press(*_list.back);
        for (const auto &btn : _list.roomButtons | std::views::values) {
            if (isVisible(btn->bounds(), _list.listTop, _list.listBottom))
                press(*btn);
        }
    }

    void RoomMenu::handleMouseReleased(const float mx, const float my)
    {
        if (_page == Page::Root) {
            if (_root.create->onClickReleased(mx, my, [&] {
                    _page = Page::Create;
                    refreshCreateCatalog();
                    _createRoom = false;
                    _createRoomName.clear();
                    if (_create.roomNameField) {
                        _create.roomNameField->clear();
                        _create.roomNameField->setFocused(true);
                    }
                    _layoutDirty = true;
                }))
                return;

            if (_root.join->onClickReleased(mx, my, [&] {
                    _page = Page::List;
                    _listRooms = true;
                    _list.scroll = 0.f;
                    _list.lastRefresh = {};
                    updateListRooms();
                    _layoutDirty = true;
                })) {
                return;
            }

            if (_root.back->onClickReleased(mx, my, [&] {
                    _backToMenu = true;
                }))
                return;

            return;
        }

        if (_page == Page::Create)
            handleCreateReleased(mx, my);
        if (_page == Page::List)
            handleJoinReleased(mx, my);
    }

    void RoomMenu::handleKeyPressed(const InputFrame &frame)
    {
        if (_page == Page::Create) {
            if (frame.key == Key::Escape) {
                _page = Page::Root;
                if (_create.roomNameField)
                    _create.roomNameField->setFocused(false);
                _layoutDirty = true;
                return;
            }
            if (frame.key == Key::Tab) {
                if (_create.roomNameField)
                    _create.roomNameField->setFocused(!_create.roomNameField->isFocused());
                return;
            }
            if (frame.key == Key::Enter) {
                _createRoom = true;
                _createRoomName = _create.roomNameField ? _create.roomNameField->value() : "default";
                return;
            }
        }

        if (_page == Page::List) {
            if (frame.key == Key::Escape) {
                _page = Page::Root;
                _layoutDirty = true;
                return;
            }
        }
    }

    void RoomMenu::handleKeyReleased(const InputFrame &frame) const
    {
        if (_page != Page::Create)
            return;
        if (_create.roomNameField && _create.roomNameField->isFocused())
            _create.roomNameField->onKeyPressed(frame.key);
    }

    void RoomMenu::handleCreateReleased(const float mx, const float my)
    {
        enum class Action { None, WPrev, WNext, LPrev, LNext, DPrev, DNext, PPrev, PNext, Confirm, Back };
        const auto a = pickAction<Action>(mx, my,
            {{_create.worldPrev.get(), Action::WPrev}, {_create.worldNext.get(), Action::WNext},
                {_create.levelPrev.get(), Action::LPrev}, {_create.levelNext.get(), Action::LNext},
                {_create.difficultyPrev.get(), Action::DPrev}, {_create.difficultyNext.get(), Action::DNext},
                {_create.playersPrev.get(), Action::PPrev}, {_create.playersNext.get(), Action::PNext},
                {_create.confirm.get(), Action::Confirm}, {_create.back.get(), Action::Back}});

        if (a == Action::None)
            return;

        bool refreshCatalog = false;
        switch (a) {
            case Action::WPrev: {
                if (const int wc = static_cast<int>(_worlds.size())) {
                    _selectedWorld = (_selectedWorld - 1 + wc) % wc;
                    refreshCatalog = true;
                }
                break;
            }
            case Action::WNext: {
                if (const int wc = static_cast<int>(_worlds.size())) {
                    _selectedWorld = (_selectedWorld + 1) % wc;
                    refreshCatalog = true;
                }
                break;
            }
            case Action::LPrev: {
                if (const int lc = static_cast<int>(_levels.size()))
                    _selectedLevel = (_selectedLevel - 1 + lc) % lc;
                break;
            }
            case Action::LNext: {
                if (const int lc = static_cast<int>(_levels.size()))
                    _selectedLevel = (_selectedLevel + 1) % lc;
                break;
            }
            case Action::DPrev:
                _selectedDifficulty = shiftDifficulty(_selectedDifficulty, -1);
                refreshCatalog = true;
                break;
            case Action::DNext:
                _selectedDifficulty = shiftDifficulty(_selectedDifficulty, +1);
                refreshCatalog = true;
                break;
            case Action::PPrev:
                if (_selectedMaxPlayers > 1)
                    --_selectedMaxPlayers;
                break;
            case Action::PNext:
                if (_selectedMaxPlayers < 4)
                    ++_selectedMaxPlayers;
                break;
            case Action::Back:
                _page = Page::Root;
                if (_create.roomNameField)
                    _create.roomNameField->setFocused(false);
                break;
            case Action::Confirm:
                _createRoom = true;
                _createRoomName = _create.roomNameField ? _create.roomNameField->value() : "default";
                break;
            default: break;
        }

        if (refreshCatalog)
            refreshCreateCatalog();

        _layoutDirty = true;
    }

    void RoomMenu::handleJoinReleased(const float mx, const float my)
    {
        for (const auto &[roomId, btn] : _list.roomButtons) {
            if (!isVisible(btn->bounds(), _list.listTop, _list.listBottom))
                continue;

            if (const uint32_t id = roomId; btn->onClickReleased(mx, my, [this, id] {
                    _joinRoom = true;
                    _joinRoomId = id;
                })) {
                return;
            }
        }

        (void) _list.back->onClickReleased(mx, my, [&] {
            _page = Page::Root;
            _layoutDirty = true;
        });
    }

    void RoomMenu::refreshCreateCatalog()
    {
        _worlds = _roomManager ? _roomManager->worlds() : std::vector<WorldEntry>{};
        if (_worlds.empty()) {
            _selectedWorld = 0;
            _selectedLevel = 0;
            _levels.clear();
            return;
        }

        _selectedWorld = std::clamp(_selectedWorld, 0, static_cast<int>(_worlds.size()) - 1);
        _selectedMaxPlayers = static_cast<uint8_t>(std::clamp(static_cast<int>(_selectedMaxPlayers), 1, 4));

        _levels.clear();
        _selectedLevel = 0;
        try {
            const std::string &worldId = _worlds.at(static_cast<size_t>(_selectedWorld)).id;
            const auto &ref = _roomManager->levelsFor(worldId);
            _levels.assign(ref.begin(), ref.end());
        } catch (const std::exception &) {
            _levels.clear();
        }
        _selectedLevel = std::clamp(_selectedLevel, 0, std::max(0, static_cast<int>(_levels.size()) - 1));
    }

    void RoomMenu::updateTextStrings() const
    {
        _header.subtitle->setString(_page == Page::Root ? "" : _page == Page::Create ? "Create a room" : "Join a room");

        _create.playersLabel->setString("Players: " + std::to_string(_selectedMaxPlayers));
        _create.difficultyLabel->setString("Difficulty: " + std::string(difficultyToStringUI(_selectedDifficulty)));

        if (_worlds.empty())
            _create.worldLabel->setString("World: (none)");
        else if (static_cast<size_t>(_selectedWorld) < _worlds.size())
            _create.worldLabel->setString(std::string(_worlds.at(static_cast<size_t>(_selectedWorld)).displayName));
        else
            _create.worldLabel->setString("World: (invalid)");

        if (_levels.empty())
            _create.levelLabel->setString("Level: (none)");
        else if (static_cast<size_t>(_selectedLevel) < _levels.size())
            _create.levelLabel->setString(_levels.at(static_cast<size_t>(_selectedLevel)).displayName);
        else
            _create.levelLabel->setString("Level: (invalid)");
    }

    void RoomMenu::render() const
    {
        renderBackground();
        _renderer->draw(*_header.title);
        _renderer->draw(*_header.subtitle);

        if (_page == Page::Root) {
            _root.create->render();
            _root.join->render();
            _root.back->render();
            return;
        }

        if (_page == Page::Create) {
            if (_create.roomNameField)
                _create.roomNameField->render();

            _create.worldPrev->render();
            _create.worldNext->render();
            _create.levelPrev->render();
            _create.levelNext->render();
            _create.difficultyPrev->render();
            _create.difficultyNext->render();
            _create.playersPrev->render();
            _create.playersNext->render();

            _renderer->draw(*_create.worldLabel);
            _renderer->draw(*_create.levelLabel);
            _renderer->draw(*_create.difficultyLabel);
            _renderer->draw(*_create.playersLabel);

            _create.confirm->render();
            _create.back->render();
            return;
        }

        if (_page == Page::List) {
            for (const auto &btn : _list.roomButtons | std::views::values) {
                if (isVisible(btn->bounds(), _list.listTop, _list.listBottom))
                    btn->render();
            }
            _list.back->render();
            return;
        }
    }

    void RoomMenu::updateListRooms()
    {
        if (!_roomManager)
            return;

        const auto rooms = _roomManager->rooms();

        std::unordered_set<uint32_t> seen;
        seen.reserve(rooms.size());

        for (const auto &[roomId, roomName, currentPlayers, maxPlayers, gameConfig, names] : rooms) {
            seen.insert(roomId);

            const std::string label =
                roomName + " (" + std::to_string(currentPlayers) + "/" + std::to_string(maxPlayers) + ")";

            if (auto it = _list.roomButtons.find(roomId); it == _list.roomButtons.end()) {
                auto btn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, label);
                btn->setScale(2.f, 1.f);
                _list.roomButtons.emplace(roomId, std::move(btn));
            } else {
                it->second->setLabel(label);
            }
        }

        for (auto it = _list.roomButtons.begin(); it != _list.roomButtons.end();) {
            if (!seen.contains(it->first))
                it = _list.roomButtons.erase(it);
            else
                ++it;
        }
    }

    void RoomMenu::layoutList(const float h, const float cx)
    {
        const auto m = computeListMetrics(h, _list.roomButtons);

        _list.listTop = m.listTop;
        _list.listBottom = m.listBottom;
        _list.scrollStep = m.scrollStep;
        _list.maxScroll = m.maxScroll;

        _list.scroll = snapScroll(_list.scroll, _list.scrollStep, _list.maxScroll);

        placeButtons(cx, m.listTop - _list.scroll, m.rowH, _list.roomButtons);

        const float offset = computeVisibleBlockOffset(m.listTop, m.listBottom, m.listH, _list.roomButtons);
        applyYOffset(offset, _list.roomButtons);

        _list.back->setPosition(cx - _list.back->bounds().w * 0.5f, h * 0.85f);
    }

    bool RoomMenu::wantsBackToMenu() const noexcept
    {
        return _backToMenu;
    }

    bool RoomMenu::wantsCreateRoom() const noexcept
    {
        return _createRoom;
    }

    bool RoomMenu::wantsJoinRoom() const noexcept
    {
        return _joinRoom;
    }

    bool RoomMenu::wantsListRooms() const noexcept
    {
        return _listRooms;
    }

    void RoomMenu::consumeCreateRoomState() noexcept
    {
        _createRoom = false;
        _createRoomName.clear();
    }

    void RoomMenu::consumeJoinRoomState() noexcept
    {
        _joinRoom = false;
    }

    void RoomMenu::consumeListRoomsRequest() noexcept
    {
        _listRooms = false;
    }

    uint8_t RoomMenu::maxPlayerSelected() const noexcept
    {
        return _selectedMaxPlayers;
    }

    Engine::Difficulty RoomMenu::difficultySelected() const noexcept
    {
        return _selectedDifficulty;
    }

    std::string RoomMenu::levelSelected() const noexcept
    {
        if (_levels.empty())
            return "";
        const int idx = std::clamp(_selectedLevel, 0, static_cast<int>(_levels.size()) - 1);
        return _levels.at(static_cast<size_t>(idx)).path;
    }

    uint32_t RoomMenu::roomIdSelected() const noexcept
    {
        return _joinRoomId;
    }

    const std::string &RoomMenu::roomNameSelected() const noexcept
    {
        return _createRoomName;
    }

} // namespace Engine
