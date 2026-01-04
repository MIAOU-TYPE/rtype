/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomMenu
*/

#include "RoomMenu.hpp"

namespace Engine
{
    namespace
    {
        Difficulty shiftDifficulty(Difficulty d, const int step) noexcept
        {
            int v = static_cast<int>(d);
            v = (v + step) % 3;
            if (v < 0)
                v += 3;
            return static_cast<Difficulty>(v);
        }

        std::string_view difficultyToStringUI(Difficulty d) noexcept
        {
            switch (d) {
                case Difficulty::Easy: return "easy";
                case Difficulty::Medium: return "medium";
                case Difficulty::Hard: return "hard";
            }
            return "unknown";
        }
    } // namespace

    RoomMenu::RoomMenu(
        const std::shared_ptr<Graphics::IRenderer> &renderer, const std::shared_ptr<RoomManager> &roomManager)
        : _renderer(renderer), _roomManager(roomManager)
    {
        const auto textures = _renderer->textures();
        _backgroundTexture = textures->load("sprites/bg-preview.png");
        if (_backgroundTexture == Graphics::InvalidTexture)
            throw RoomMenuError("{RoomMenu::RoomMenu} failed to load sprites/bg-preview.png texture");
        _backgroundCmd.textureId = _backgroundTexture;

        _header.title = _renderer->texts()->createText(64, {255, 255, 255, 255});
        _header.title->setString("ROOMS");
        _header.subtitle = _renderer->texts()->createText(24, {200, 200, 200, 255});

        _root.create = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "CREATE");
        _root.join = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "JOIN");
        _root.back = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");

        _create.worldPrev = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "<");
        _create.worldNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, ">");
        _create.difficultyPrev = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "<");
        _create.difficultyNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, ">");
        _create.playersPrev = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "-");
        _create.playersNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        _create.confirm = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "CREATE");
        _create.back = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");

        _create.levelName = _renderer->texts()->createText(36, {255, 255, 255, 255});
        _create.worldLabel = _renderer->texts()->createText(32, {255, 255, 255, 255});
        _create.difficultyLabel = _renderer->texts()->createText(32, {255, 255, 255, 255});
        _create.playersLabel = _renderer->texts()->createText(32, {255, 255, 255, 255});

        refreshCreateCatalog();
        _layoutDirty = true;
    }

    void RoomMenu::layout()
    {
        const auto tex = _renderer->textures()->getSize(_backgroundTexture);
        auto centerX = [&](UI::UIButton &b, float x, float y) {
            b.setPosition(x - b.bounds().w * 0.5f, y);
        };
        const auto vp = _renderer->getViewportSize();
        const auto w = static_cast<float>(vp.width);
        const auto h = static_cast<float>(vp.height);
        const float cx = w * 0.5f;

        updateTextStrings();
        if (_backgroundTexture == Graphics::InvalidTexture)
            return;
        _backgroundCmd.frame = {0, 0, static_cast<int>(tex.width), static_cast<int>(tex.height)};
        _backgroundCmd.position = {0.f, 0.f};
        _backgroundCmd.scale = {w / static_cast<float>(tex.width), h / static_cast<float>(tex.height)};
        _header.title->setPosition(cx - _header.title->getWidth() * 0.5f, h * 0.07f);
        _header.subtitle->setPosition(cx - _header.subtitle->getWidth() * 0.5f, h * 0.16f);

        if (_page == Page::Root) {
            centerX(*_root.create, cx, h * 0.35f);
            centerX(*_root.join, cx, h * 0.51f);
            centerX(*_root.back, cx, h * 0.67f);
            return;
        }
        layoutCreate(w, h, cx);
    }

    void RoomMenu::layoutCreate(const float w, const float h, const float cx) const
    {
        auto centerX = [&](UI::UIButton &b, float x, float y) {
            b.setPosition(x - b.bounds().w * 0.5f, y);
        };
        auto row = [&](UI::UIButton &prev, UI::UIButton &next, Graphics::IText &label, int i) {
            const float y = h * 0.34f + h * 0.11f * static_cast<float>(i);
            prev.centerButtonLabel(w * 0.25f, y, label, cx);
            centerX(next, w * 0.75f, y);
        };

        row(*_create.worldPrev, *_create.worldNext, *_create.worldLabel, 0);
        row(*_create.difficultyPrev, *_create.difficultyNext, *_create.difficultyLabel, 1);
        row(*_create.playersPrev, *_create.playersNext, *_create.playersLabel, 2);
        _create.levelName->setPosition(cx - _create.levelName->getWidth() * 0.5f, h * 0.24f);
        centerX(*_create.confirm, cx, h * 0.75f);
        centerX(*_create.back, cx, h * 0.88f);
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
    }

    void RoomMenu::updateHover(const float mx, const float my) const
    {
        auto each = [&](auto &&fn) {
            if (_page == Page::Root) {
                fn(*_root.create);
                fn(*_root.join);
                fn(*_root.back);
            } else {
                fn(*_create.worldPrev);
                fn(*_create.worldNext);
                fn(*_create.difficultyPrev);
                fn(*_create.difficultyNext);
                fn(*_create.playersPrev);
                fn(*_create.playersNext);
                fn(*_create.confirm);
                fn(*_create.back);
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
        press(*_create.worldPrev);
        press(*_create.worldNext);
        press(*_create.difficultyPrev);
        press(*_create.difficultyNext);
        press(*_create.playersPrev);
        press(*_create.playersNext);
        press(*_create.confirm);
        press(*_create.back);
    }

    void RoomMenu::handleMouseReleased(const float mx, const float my)
    {
        if (_page == Page::Root) {
            if (_root.create->onClickReleased(mx, my, [&] {
                    _page = Page::Create;
                    refreshCreateCatalog();
                    _layoutDirty = true;
                }))
                return;
            if (_root.join->onClickReleased(mx, my, [&] {
                }))
                return;
            _root.back->onClickReleased(mx, my, [&] {
                _backToMenu = true;
            });
        } else
            handleCreateReleased(mx, my);
    }

    void RoomMenu::handleCreateReleased(const float mx, const float my)
    {
        enum class Action { None, WPrev, WNext, DPrev, DNext, PPrev, PNext, Confirm, Back };
        auto a = Action::None;
        bool refreshCatalog = false;
        auto pick = [&](const std::unique_ptr<UI::UIButton> &btn, const Action act) -> bool {
            return btn && btn->onClickReleased(mx, my, [&] {
                a = act;
            });
        };

        if (!(pick(_create.worldPrev, Action::WPrev) || pick(_create.worldNext, Action::WNext)
                || pick(_create.difficultyPrev, Action::DPrev) || pick(_create.difficultyNext, Action::DNext)
                || pick(_create.playersPrev, Action::PPrev) || pick(_create.playersNext, Action::PNext)
                || pick(_create.confirm, Action::Confirm) || pick(_create.back, Action::Back))) {
            return;
        }

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
            case Action::Back: _page = Page::Root; break;
            case Action::None: break;
            default:;
        }
        if (refreshCatalog)
            refreshCreateCatalog();
        _layoutDirty = true;
    }

    void RoomMenu::refreshCreateCatalog()
    {
        _worlds = _roomManager ? _roomManager->worlds() : std::vector<WorldEntry>{};
        if (_worlds.empty()) {
            _selectedWorld = 0;
            _levels.clear();
            return;
        }
        _selectedWorld = std::clamp(_selectedWorld, 0, static_cast<int>(_worlds.size()) - 1);
        _selectedMaxPlayers = static_cast<std::uint8_t>(std::clamp(static_cast<int>(_selectedMaxPlayers), 1, 4));
        _levels.clear();
        try {
            const std::string &worldId = _worlds.at(static_cast<std::size_t>(_selectedWorld)).id;
            const auto &ref = _roomManager->levelsFor(worldId, _selectedDifficulty); // retourne une ref
            _levels.assign(ref.begin(), ref.end());
        } catch (const std::exception &) {
            _levels.clear();
        }
    }

    void RoomMenu::updateTextStrings() const
    {
        _header.subtitle->setString(_page == Page::Root ? "Create a room" : "Room settings");
        _create.levelName->setString(_levels.empty() ? "(no level)" : _levels.front().displayName);
        _create.playersLabel->setString("Players: " + std::to_string(_selectedMaxPlayers));
        _create.difficultyLabel->setString("Difficulty: " + std::string(difficultyToStringUI(_selectedDifficulty)));
        if (_worlds.empty())
            _create.worldLabel->setString("World: (none)");
        else
            _create.worldLabel->setString(
                "World: " + std::string(_worlds.at(static_cast<std::size_t>(_selectedWorld)).displayName));
    }

    void RoomMenu::render() const
    {
        _renderer->draw(_backgroundCmd);
        _renderer->draw(*_header.title);
        _renderer->draw(*_header.subtitle);
        if (_page == Page::Root) {
            _root.create->render();
            _root.join->render();
            _root.back->render();
            return;
        }
        _create.worldPrev->render();
        _create.worldNext->render();
        _create.difficultyPrev->render();
        _create.difficultyNext->render();
        _create.playersPrev->render();
        _create.playersNext->render();
        _renderer->draw(*_create.levelName);
        _renderer->draw(*_create.worldLabel);
        _renderer->draw(*_create.difficultyLabel);
        _renderer->draw(*_create.playersLabel);
        _create.confirm->render();
        _create.back->render();
    }

    bool RoomMenu::wantsBackToMenu() const noexcept
    {
        return _backToMenu;
    }
} // namespace Engine
