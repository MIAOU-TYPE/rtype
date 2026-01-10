/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Menu
*/

#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include "IRenderer.hpp"
#include "IText.hpp"
#include "ITextureManager.hpp"
#include "InputState.hpp"
#include "RenderCommand.hpp"
#include "UIButton.hpp"
#include "UITextField.hpp"

namespace Engine
{
    /**
     * @brief Exception class for menu errors.
     */
    class MenuError : public std::exception {
      public:
        /**
         * @brief Constructor for MenuError.
         * @param message The error message.
         */
        explicit MenuError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Override of the what() method from std::exception.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief Class representing the main menu of the game.
     */
    class Menu {
      public:
        enum class Page { UnauthedRoot, AuthedRoot, LoginForm, RegisterForm };
        enum class AuthMode { None, Login, Register };
        /**
         * @brief Construct a new Menu object.
         *
         * @param renderer Shared pointer to the renderer used for rendering the menu.
         */
        explicit Menu(const std::shared_ptr<Graphics::IRenderer> &renderer);

        /**
         * @brief Update the menu state.
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame);

        /**
         * @brief Render the menu.
         */
        void render() const;

        /**
         * @brief Called when entering the menu state.
         */
        void onEnter();

        void setAuthed(bool v) noexcept;
        bool isAuthed() const noexcept;

        bool hasAuthSubmission() const noexcept;
        void consumeAuthSubmission() noexcept;
        AuthMode submittedMode() const noexcept;
        const std::string &submittedUsername() const noexcept;
        const std::string &submittedPassword() const noexcept;

        /**
         * @brief Check if the user wants to start the game.
         *
         * @return true if the user wants to start the game, false otherwise.
         */
        [[nodiscard]] bool wantsToStart() const noexcept;

        /**
         * @brief Check if the user wants to quit the game.
         *
         * @return true if the user wants to quit the game, false otherwise.
         */
        [[nodiscard]] bool wantsToQuit() const noexcept;

        /**
         * @brief Check if the user wants to access the settings.
         *
         * @return true if the user wants to access the settings, false otherwise.
         */
        [[nodiscard]] bool wantsSettings() const noexcept;

        /**
         * @brief Handle resizing of the menu.
         */
        void layout();

      private:
        /**
         * @brief Handle user input for the menu.
         * @param frame The current input frame.
         */
        void handleInput(const InputFrame &frame);

        void handleMousePressed(const InputFrame &frame) const;
        void handleMouseReleased(const InputFrame &frame);
        void handleKeyPressed(const InputFrame &frame);

        void enterForm(Page p);
        void backToRoot();
        void submit();

        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the renderer.

        Graphics::TextureHandle _backgroundTexture; ///> Handle to the background texture.
        Graphics::TextureHandle _logoTexture;       ///> Handle to the logo texture.

        RenderCommand _backgroundCmd; ///> Render command for the background.
        RenderCommand _logoCmd;       ///> Render command for the logo.

        Page _page = Page::UnauthedRoot;
        bool _authed = false;

        std::unique_ptr<UI::UIButton> _login;
        std::unique_ptr<UI::UIButton> _register;

        std::unique_ptr<UI::UIButton> _play;     ///> Button to start the game.
        std::unique_ptr<UI::UIButton> _settings; ///> Button to access settings.
        std::unique_ptr<UI::UIButton> _quit;     ///> Button to quit the game.

        std::unique_ptr<UI::UIButton> _submitBtn;
        std::unique_ptr<UI::UIButton> _backBtn;
        std::unique_ptr<UI::UITextField> _userField;
        std::unique_ptr<UI::UITextField> _passField;

        bool _startRequested = false;    ///> Flag indicating if the user requested to start the game.
        bool _quitRequested = false;     ///> Flag indicating if the user requested to quit.
        bool _settingsRequested = false; ///> Flag indicating if the user requested to access settings.

        bool _submitted = false;
        AuthMode _submittedMode = AuthMode::None;
        std::string _submittedUser;
        std::string _submittedPass;
    };
} // namespace Engine
