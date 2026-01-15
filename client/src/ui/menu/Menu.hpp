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
#include "AMenu.hpp"
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
    class Menu final : public AMenu {
      public:
        /**
         * @brief Enumeration of the different pages in the menu.
         * UnauthedRoot: The root page for unauthenticated users.
         * AuthedRoot: The root page for authenticated users.
         * LoginForm: The login form page.
         * RegisterForm: The registration form page.
         */
        enum class Page { UnauthedRoot, AuthedRoot, LoginForm, RegisterForm };

        /**
         * @brief Enumeration of authentication modes.
         * None: No authentication mode.
         * Login: Login mode.
         * Register: Registration mode.
         */
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
        void update(const InputFrame &frame) override;

        /**
         * @brief Render the menu.
         */
        void render() const override;

        /**
         * @brief Called when entering the menu state.
         */
        void onEnter();

        /**
         * @brief Set an authentication error message.
         * @param message The error message to set.
         */
        void setAuthError(std::string message);

        /**
         * @brief Clear the authentication error message.
         */
        void clearAuthError();

        /**
         * @brief Set the authentication status of the user.
         *
         * @param v true if the user is authenticated, false otherwise.
         */
        void setAuthed(bool v) noexcept;

        /**
         * @brief Check if the user is authenticated.
         *
         * @return true if the user is authenticated, false otherwise.
         */
        [[nodiscard]]
        bool isAuthed() const noexcept;

        /**
         * @brief Check if there is an authentication submission.
         *
         * @return true if there is an authentication submission, false otherwise.
         */
        [[nodiscard]]
        bool hasAuthSubmission() const noexcept;

        /**
         * @brief Consume the authentication submission.
         */
        void consumeAuthSubmission() noexcept;

        /**
         * @brief Get the submitted authentication mode.
         *
         * @return The submitted authentication mode.
         */
        [[nodiscard]] AuthMode submittedMode() const noexcept;

        /**
         * @brief Get the submitted username.
         *
         * @return The submitted username.
         */
        [[nodiscard]] const std::string &submittedUsername() const noexcept;

        /**
         * @brief Get the submitted password.
         *
         * @return The submitted password.
         */
        [[nodiscard]] const std::string &submittedPassword() const noexcept;

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
         * @brief Check if the user wants to access the level editor.
         *
         * @return true if the user wants to access the level editor, false otherwise.
         */
        [[nodiscard]] bool wantsLevelEditor() const noexcept;

        /**
         * @brief Handle resizing of the menu.
         */
        void layout() override;

      private:
        /**
         * @brief Handle user input for the menu.
         * @param frame The current input frame.
         */
        void handleInput(const InputFrame &frame);

        /**
         * @brief Handle mouse movement input.
         * @param frame The current input frame.
         */
        void handleMousePressed(const InputFrame &frame) const;

        /**
         * @brief Handle mouse release input.
         * @param frame The current input frame.
         */
        void handleMouseReleased(const InputFrame &frame);

        /**
         * @brief Handle key press input.
         * @param frame The current input frame.
         */
        void handleKeyPressed(const InputFrame &frame);

        /**
         * @brief Handle key release input.
         * @param frame The current input frame.
         */
        void handleKeyReleased(const InputFrame &frame) const;

        /**
         * @brief Enter the specified form page.
         * @param p The page to enter (LoginForm or RegisterForm).
         */
        void enterForm(Page p);

        /**
         * @brief Return to the root page.
         */
        void backToRoot();

        /**
         * @brief Submit the authentication form.
         */
        void submit();

        Graphics::TextureHandle _logoTexture = Graphics::InvalidTexture; ///> Handle to the logo texture.
        RenderCommand _logoCmd;                                          ///> Render command for the logo.

        Page _page = Page::UnauthedRoot; ///> Current page in the menu.
        bool _authed = false;            ///> Flag indicating if the user is authenticated.

        std::unique_ptr<UI::UIButton> _login;    ///> Button to access the login form.
        std::unique_ptr<UI::UIButton> _register; ///> Button to access the registration form.

        std::unique_ptr<UI::UIButton> _play;        ///> Button to start the game.
        std::unique_ptr<UI::UIButton> _settings;    ///> Button to access settings.
        std::unique_ptr<UI::UIButton> _levelEditor; ///> Button to access level editor.
        std::unique_ptr<UI::UIButton> _quit;        ///> Button to quit the game.

        std::unique_ptr<UI::UIButton> _submitBtn;    ///> Button to submit the authentication form.
        std::unique_ptr<UI::UIButton> _backBtn;      ///> Button to go back to the root page.
        std::unique_ptr<UI::UITextField> _userField; ///> Text field for entering the username.
        std::unique_ptr<UI::UITextField> _passField; ///> Text field for entering the password.

        std::unique_ptr<Graphics::IText> _authErrorText; ///> Text object for displaying authentication error messages.
        std::string _authErrorMessage;                   ///> Authentication error message.

        bool _startRequested = false;       ///> Flag indicating if the user requested to start the game.
        bool _quitRequested = false;        ///> Flag indicating if the user requested to quit.
        bool _settingsRequested = false;    ///> Flag indicating if the user requested to access settings.
        bool _levelEditorRequested = false; ///> Flag indicating if the user requested to access level editor.

        bool _submitted = false;                  ///> Flag indicating if there is a submitted authentication form.
        AuthMode _submittedMode = AuthMode::None; ///> Submitted authentication mode.
        std::string _submittedUser;               ///> Submitted username.
        std::string _submittedPass;               ///> Submitted password.
    };
} // namespace Engine
