/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Menu
*/

#include "Menu.hpp"

namespace Engine
{
    Menu::Menu(const std::shared_ptr<Graphics::IRenderer> &renderer) : AMenu(renderer)
    {
        try {
            const auto textures = _renderer->textures();
            loadBackground("sprites/bg-preview.png");
            _logoTexture = textures->load("sprites/menu_logo.png");
            if (_logoTexture == Graphics::InvalidTexture)
                throw MenuError("{Menu::Menu} failed to load sprites/menu_logo.png texture");
            _logoCmd.textureId = _logoTexture;

            _login = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "LOGIN");
            _register = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "REGISTER");

            _play = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "PLAY");
            _settings = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "SETTINGS");
            _levelEditor = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "LEVEL EDITOR");
            _quit = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "QUIT");

            _userField = std::make_unique<UI::UITextField>(_renderer, "Username", false);
            _passField = std::make_unique<UI::UITextField>(_renderer, "Password", true);
            _submitBtn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "SUBMIT");
            _backBtn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");

            _authErrorText = _renderer->texts()->createText(22, {255, 80, 80, 255});
            _authErrorText->setString("");
        } catch (const std::exception &e) {
            throw MenuError(std::string("{Menu::Menu} initialization failed: ") + e.what());
        }
    }

    void Menu::onEnter()
    {
        _startRequested = false;
        _quitRequested = false;
        _settingsRequested = false;
        _levelEditorRequested = false;
        _submitted = false;
        _submittedMode = AuthMode::None;
        _submittedUser.clear();
        _submittedPass.clear();

        _authErrorMessage.clear();
        if (_authErrorText)
            _authErrorText->setString("");
        clearAuthError();
        resetButtons(_login.get(), _register.get(), _play.get(), _settings.get(), _levelEditor.get(), _quit.get(),
            _submitBtn.get(), _backBtn.get());

        if (_userField) {
            _userField->clear();
            _userField->setFocused(false);
        }
        if (_passField) {
            _passField->clear();
            _passField->setFocused(false);
        }
        _page = _authed ? Page::AuthedRoot : Page::UnauthedRoot;
        layout();
    }

    void Menu::setAuthed(const bool v) noexcept
    {
        if (_authed == v)
            return;
        _authed = v;
        _page = _authed ? Page::AuthedRoot : Page::UnauthedRoot;
        _startRequested = false;
        _quitRequested = false;
        _settingsRequested = false;
        _levelEditorRequested = false;
        _submitted = false;
        _submittedMode = AuthMode::None;
        _submittedUser.clear();
        _submittedPass.clear();

        _authErrorMessage.clear();
        if (_authErrorText)
            _authErrorText->setString("");
        clearAuthError();
        if (_userField) {
            _userField->clear();
            _userField->setFocused(false);
        }
        if (_passField) {
            _passField->clear();
            _passField->setFocused(false);
        }
        layout();
    }

    bool Menu::isAuthed() const noexcept
    {
        return _authed;
    }

    void Menu::setAuthError(std::string message)
    {
        _authErrorMessage = std::move(message);
        if (_authErrorText)
            _authErrorText->setString(_authErrorMessage);
        layout();
    }

    void Menu::clearAuthError()
    {
        if (_authErrorMessage.empty())
            return;
        _authErrorMessage.clear();
        if (_authErrorText)
            _authErrorText->setString("");
        layout();
    }

    void Menu::layout()
    {
        const auto vp = viewportF();
        const float w = vp.w;
        const float h = vp.h;
        layoutBackground();
        const auto logoSize = _renderer->textures()->getSize(_logoTexture);

        _logoCmd.frame = {0, 0, static_cast<int>(logoSize.width), static_cast<int>(logoSize.height)};

        constexpr float LOGO_SCALE = 1.0f;
        _logoCmd.scale = {LOGO_SCALE, LOGO_SCALE};
        _logoCmd.position = {(w - static_cast<float>(logoSize.width) * LOGO_SCALE) * 0.5f, h * 0.05f};

        if (_page == Page::UnauthedRoot) {
            layoutRowCentered(*_login, *_register, vp.cx, h * 0.63f, w * 0.05f);
            placeCentered(*_settings, vp.cx, h * 0.76f);
            placeCentered(*_quit, vp.cx, h * 0.89f);
            return;
        }

        if (_page == Page::AuthedRoot) {
            placeCentered(*_play, vp.cx, h * 0.55f);
            placeCentered(*_settings, vp.cx, h * 0.68f);
            placeCentered(*_levelEditor, vp.cx, h * 0.81f);
            placeCentered(*_quit, vp.cx, h * 0.94f);
            return;
        }

        const float fieldX = w * 0.5f - 220.f;
        constexpr float fieldW = 440.f;

        _userField->setPosition(fieldX, h * 0.40f);
        _userField->setWidth(fieldW);

        _passField->setPosition(fieldX, h * 0.55f);
        _passField->setWidth(fieldW);

        if (_authErrorText && !_authErrorMessage.empty())
            _authErrorText->setPosition(vp.cx - _authErrorText->getWidth() * 0.5f, h * 0.32f);
        placeCentered(*_submitBtn, vp.cx, h * 0.73f);
        placeCentered(*_backBtn, vp.cx, h * 0.87f);
    }

    void Menu::update(const InputFrame &frame)
    {
        handleInput(frame);

        if (_page == Page::UnauthedRoot) {
            updateButtons(frame.mouseX, frame.mouseY, _login.get(), _register.get(), _settings.get(), _quit.get());
        } else if (_page == Page::AuthedRoot) {
            updateButtons(frame.mouseX, frame.mouseY, _play.get(), _settings.get(), _levelEditor.get(), _quit.get());
        } else {
            updateButtons(frame.mouseX, frame.mouseY, _submitBtn.get(), _backBtn.get());
        }
    }

    void Menu::render() const
    {
        renderBackground();
        _renderer->draw(_logoCmd);
        if (_page == Page::UnauthedRoot) {
            _login->render();
            _register->render();
            _settings->render();
            _quit->render();
            return;
        }
        if (_page == Page::AuthedRoot) {
            _play->render();
            _settings->render();
            _levelEditor->render();
            _quit->render();
            return;
        }
        if (_authErrorText && !_authErrorMessage.empty())
            _renderer->draw(*_authErrorText);
        _userField->render();
        _passField->render();
        _submitBtn->render();
        _backBtn->render();
    }

    void Menu::handleInput(const InputFrame &frame)
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

    void Menu::handleMousePressed(const InputFrame &frame) const
    {
        if (_page == Page::UnauthedRoot) {
            pressButtons(frame.mouseX, frame.mouseY, _login.get(), _register.get(), _settings.get(), _quit.get());
            return;
        }
        if (_page == Page::AuthedRoot) {
            pressButtons(frame.mouseX, frame.mouseY, _play.get(), _settings.get(), _levelEditor.get(), _quit.get());
            return;
        }
        pressButtons(frame.mouseX, frame.mouseY, _submitBtn.get(), _backBtn.get());
        _userField->onMousePressed(frame.mouseX, frame.mouseY);
        _passField->onMousePressed(frame.mouseX, frame.mouseY);
        if (_userField->isFocused())
            _passField->setFocused(false);
        else if (_passField->isFocused())
            _userField->setFocused(false);
    }

    void Menu::handleMouseReleased(const InputFrame &frame)
    {
        enum class Action { None, Login, Register, Play, Settings, LevelEditor, Quit, Submit, Back };
        auto a = Action::None;

        if (_page == Page::UnauthedRoot)
            a = pickAction<Action>(frame.mouseX, frame.mouseY,
                {{_login.get(), Action::Login}, {_register.get(), Action::Register},
                    {_settings.get(), Action::Settings}, {_quit.get(), Action::Quit}});
        else if (_page == Page::AuthedRoot)
            a = pickAction<Action>(frame.mouseX, frame.mouseY,
                {{_play.get(), Action::Play}, {_settings.get(), Action::Settings},
                    {_levelEditor.get(), Action::LevelEditor}, {_quit.get(), Action::Quit}});
        else
            a = pickAction<Action>(
                frame.mouseX, frame.mouseY, {{_submitBtn.get(), Action::Submit}, {_backBtn.get(), Action::Back}});

        if (a == Action::None)
            return;
        switch (a) {
            case Action::Login: enterForm(Page::LoginForm); break;
            case Action::Register: enterForm(Page::RegisterForm); break;
            case Action::Play: _startRequested = true; break;
            case Action::Settings: _settingsRequested = true; break;
            case Action::LevelEditor: _levelEditorRequested = true; break;
            case Action::Quit: _quitRequested = true; break;
            case Action::Submit: submit(); break;
            case Action::Back: backToRoot(); break;
            case Action::None:
            default: break;
        }
    }

    void Menu::handleKeyPressed(const InputFrame &frame)
    {
        if (_page != Page::LoginForm && _page != Page::RegisterForm)
            return;
        if (frame.key == Key::Tab) {
            const bool uf = _userField->isFocused();
            if (const bool pf = _passField->isFocused(); !uf && !pf) {
                _userField->setFocused(true);
                return;
            }
            if (uf) {
                _userField->setFocused(false);
                _passField->setFocused(true);
            } else {
                _passField->setFocused(false);
                _userField->setFocused(true);
            }
            return;
        }
        if (frame.key == Key::Enter) {
            submit();
            return;
        }
        if (frame.key == Key::Escape)
            backToRoot();
    }

    void Menu::handleKeyReleased(const InputFrame &frame) const
    {
        if (_userField->isFocused())
            _userField->onKeyPressed(frame.key);
        else if (_passField->isFocused())
            _passField->onKeyPressed(frame.key);
    }

    void Menu::enterForm(const Page p)
    {
        _page = p;
        clearAuthError();
        _userField->setFocused(true);
        _passField->setFocused(false);
        layout();
    }

    void Menu::backToRoot()
    {
        _page = _authed ? Page::AuthedRoot : Page::UnauthedRoot;
        clearAuthError();
        _userField->clear();
        _passField->clear();
        _userField->setFocused(false);
        _passField->setFocused(false);
        layout();
    }

    void Menu::submit()
    {
        if (_userField->value().empty() || _passField->value().empty())
            return;
        _submitted = true;
        _submittedMode = (_page == Page::LoginForm) ? AuthMode::Login : AuthMode::Register;
        _submittedUser = _userField->value();
        _submittedPass = _passField->value();
    }

    bool Menu::hasAuthSubmission() const noexcept
    {
        return _submitted;
    }

    void Menu::consumeAuthSubmission() noexcept
    {
        _submitted = false;
        _submittedMode = AuthMode::None;
        _submittedUser.clear();
        _submittedPass.clear();
    }

    Menu::AuthMode Menu::submittedMode() const noexcept
    {
        return _submittedMode;
    }

    const std::string &Menu::submittedUsername() const noexcept
    {
        return _submittedUser;
    }

    const std::string &Menu::submittedPassword() const noexcept
    {
        return _submittedPass;
    }

    bool Menu::wantsToStart() const noexcept
    {
        return _startRequested;
    }

    bool Menu::wantsToQuit() const noexcept
    {
        return _quitRequested;
    }

    bool Menu::wantsSettings() const noexcept
    {
        return _settingsRequested;
    }

    bool Menu::wantsLevelEditor() const noexcept
    {
        return _levelEditorRequested;
    }
} // namespace Engine
