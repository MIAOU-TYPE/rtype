/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Menu
*/

#include "Menu.hpp"

#include <iostream>

namespace Engine
{
    Menu::Menu(const std::shared_ptr<Graphics::IRenderer> &renderer) : _renderer(renderer)
    {
        try {
            const auto textures = renderer->textures();
            _backgroundTexture = textures->load("sprites/bg-preview.png");
            _logoTexture = textures->load("sprites/menu_logo.png");
            if (_backgroundTexture == Graphics::InvalidTexture)
                throw MenuError("{Menu::Menu} failed to load sprites/bg-preview.png texture");
            if (_logoTexture == Graphics::InvalidTexture)
                throw MenuError("{Menu::Menu} failed to load sprites/menu_logo.png texture");

            _backgroundCmd.textureId = _backgroundTexture;
            _logoCmd.textureId = _logoTexture;

            _login = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "LOGIN");
            _register = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "REGISTER");

            _play = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "PLAY");
            _settings = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "SETTINGS");
            _quit = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "QUIT");

            _userField = std::make_unique<UI::UITextField>(_renderer, "Username", false);
            _passField = std::make_unique<UI::UITextField>(_renderer, "Password", true);
            _submitBtn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "SUBMIT");
            _backBtn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");
        } catch (const std::exception &e) {
            throw MenuError(std::string("{Menu::Menu} initialization failed: ") + e.what());
        }
    }

    void Menu::onEnter()
    {
        _startRequested = false;
        _quitRequested = false;
        _settingsRequested = false;
        _submitted = false;
        _submittedMode = AuthMode::None;
        _submittedUser.clear();
        _submittedPass.clear();

        if (_login)
            _login->reset();
        if (_register)
            _register->reset();
        if (_play)
            _play->reset();
        if (_settings)
            _settings->reset();
        if (_quit)
            _quit->reset();
        if (_submitBtn)
            _submitBtn->reset();
        if (_backBtn)
            _backBtn->reset();

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
        _submitted = false;
        _submittedMode = AuthMode::None;
        _submittedUser.clear();
        _submittedPass.clear();
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

    void Menu::layout()
    {
        const auto vp = _renderer->getViewportSize();
        const auto w = static_cast<float>(vp.width);
        const auto h = static_cast<float>(vp.height);
        const auto [width, height] = _renderer->textures()->getSize(_backgroundTexture);

        _backgroundCmd.frame = {0, 0, static_cast<int>(width), static_cast<int>(height)};
        _backgroundCmd.position = {0.f, 0.f};
        _backgroundCmd.scale = {static_cast<float>(vp.width) / static_cast<float>(width),
            static_cast<float>(vp.height) / static_cast<float>(height)};

        const auto logoSize = _renderer->textures()->getSize(_logoTexture);

        _logoCmd.frame = {0, 0, static_cast<int>(logoSize.width), static_cast<int>(logoSize.height)};

        constexpr float LOGO_SCALE = 1.0f;
        _logoCmd.scale = {LOGO_SCALE, LOGO_SCALE};
        _logoCmd.position = {(w - static_cast<float>(logoSize.width) * LOGO_SCALE) * 0.5f, h * 0.05f};

        if (_page == Page::UnauthedRoot) {
            const float x = (w - _login->bounds().w) / 2.f;
            _login->setPosition(x * 0.80f, h * 0.63f);
            _register->setPosition(x * 1.20f, h * 0.63f);
            _settings->setPosition(x, h * 0.76f);
            _quit->setPosition(x, h * 0.89f);
            return;
        }

        if (_page == Page::AuthedRoot) {
            const float x = (w - _play->bounds().w) / 2.f;
            _play->setPosition(x, h * 0.63f);
            _settings->setPosition(x, h * 0.76f);
            _quit->setPosition(x, h * 0.89f);
            return;
        }

        const float fieldX = w * 0.5f - 220.f;
        constexpr float fieldW = 440.f;

        _userField->setPosition(fieldX, h * 0.40f);
        _userField->setWidth(fieldW);

        _passField->setPosition(fieldX, h * 0.55f);
        _passField->setWidth(fieldW);

        const float btnX = (w - _submitBtn->bounds().w) / 2.f;
        _submitBtn->setPosition(btnX, h * 0.73f);
        _backBtn->setPosition(btnX, h * 0.87f);
    }

    void Menu::update(const InputFrame &frame)
    {
        handleInput(frame);

        if (_page == Page::UnauthedRoot) {
            _login->update(frame.mouseX, frame.mouseY);
            _register->update(frame.mouseX, frame.mouseY);
            _settings->update(frame.mouseX, frame.mouseY);
            _quit->update(frame.mouseX, frame.mouseY);
        } else if (_page == Page::AuthedRoot) {
            _play->update(frame.mouseX, frame.mouseY);
            _settings->update(frame.mouseX, frame.mouseY);
            _quit->update(frame.mouseX, frame.mouseY);
        } else {
            _submitBtn->update(frame.mouseX, frame.mouseY);
            _backBtn->update(frame.mouseX, frame.mouseY);
        }
    }

    void Menu::render() const
    {
        _renderer->draw(_backgroundCmd);
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
            _quit->render();
            return;
        }
        _userField->render();
        _passField->render();
        _submitBtn->render();
        _backBtn->render();
    }

    void Menu::handleInput(const InputFrame &frame)
    {
        if (frame.mousePressed)
            handleMousePressed(frame);
        handleMouseReleased(frame);
        if (frame.keyPressed)
            handleKeyPressed(frame);
    }

    void Menu::handleMousePressed(const InputFrame &frame) const
    {
        if (_page == Page::UnauthedRoot) {
            _login->onMousePressed(frame.mouseX, frame.mouseY);
            _register->onMousePressed(frame.mouseX, frame.mouseY);
            _settings->onMousePressed(frame.mouseX, frame.mouseY);
            _quit->onMousePressed(frame.mouseX, frame.mouseY);
            return;
        }
        if (_page == Page::AuthedRoot) {
            _play->onMousePressed(frame.mouseX, frame.mouseY);
            _settings->onMousePressed(frame.mouseX, frame.mouseY);
            _quit->onMousePressed(frame.mouseX, frame.mouseY);
            return;
        }
        _submitBtn->onMousePressed(frame.mouseX, frame.mouseY);
        _backBtn->onMousePressed(frame.mouseX, frame.mouseY);
        _userField->onMousePressed(frame.mouseX, frame.mouseY);
        _passField->onMousePressed(frame.mouseX, frame.mouseY);
        if (_userField->isFocused())
            _passField->setFocused(false);
        else if (_passField->isFocused())
            _userField->setFocused(false);
    }

    void Menu::handleMouseReleased(const InputFrame &frame)
    {
        if (_page == Page::UnauthedRoot) {
            if (_login->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                    enterForm(Page::LoginForm);
                }))
                return;
            if (_register->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                    enterForm(Page::RegisterForm);
                }))
                return;
            if (_settings->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                    _settingsRequested = true;
                }))
                return;
            (void) _quit->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                _quitRequested = true;
            });
            return;
        }
        if (_page == Page::AuthedRoot) {
            if (_play->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                    _startRequested = true;
                }))
                return;
            if (_settings->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                    _settingsRequested = true;
                }))
                return;
            (void) _quit->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                _quitRequested = true;
            });
            return;
        }
        if (_submitBtn->onClickReleased(frame.mouseX, frame.mouseY, [&] {
                submit();
            }))
            return;
        (void) _backBtn->onClickReleased(frame.mouseX, frame.mouseY, [&] {
            backToRoot();
        });
    }

    void Menu::handleKeyPressed(const InputFrame &frame)
    {
        if (_page != Page::LoginForm && _page != Page::RegisterForm)
            return;
        if (frame.key == Key::Tab) {
            const bool uf = _userField->isFocused();
            const bool pf = _passField->isFocused();
            if (!uf && !pf) {
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
        if (frame.key == Key::Escape) {
            backToRoot();
            return;
        }
        if (_userField->isFocused())
            _userField->onKeyPressed(frame.key);
        else if (_passField->isFocused())
            _passField->onKeyPressed(frame.key);
    }

    void Menu::enterForm(const Page p)
    {
        _page = p;
        _userField->setFocused(true);
        _passField->setFocused(false);
        layout();
    }

    void Menu::backToRoot()
    {
        _page = _authed ? Page::AuthedRoot : Page::UnauthedRoot;
        _userField->clear();
        _passField->clear();
        _userField->setFocused(false);
        _passField->setFocused(false);
        layout();
    }

    void Menu::submit()
    {
        std::cout << "Submitting form..." << std::endl;
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
} // namespace Engine
