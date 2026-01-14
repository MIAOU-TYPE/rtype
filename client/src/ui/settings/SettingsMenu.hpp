/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#pragma once

#include <algorithm>
#include <fstream>
#include <memory>
#include <stdexcept>
#include "AMenu.hpp"
#include "IRenderer.hpp"
#include "IText.hpp"
#include "InputState.hpp"
#include "MusicRegistry.hpp"
#include "RenderCommand.hpp"
#include "SettingsConfig.hpp"
#include "SoundRegistry.hpp"
#include "UIButton.hpp"

/**
 * @brief Using declaration to bring BindAction into scope for convenience in the settings menu.
 * This allows direct use of BindAction enum values without full qualification.
 */
using Utils::BindAction;

namespace Engine
{
    /**
     * @brief Exception class for settings menu errors.
     */
    class SettingsMenuError : public std::runtime_error {
      public:
        /**
         * @brief Constructor for SettingsMenuError.
         * @param message The error message.
         */
        explicit SettingsMenuError(const std::string &message) : std::runtime_error("\n\t" + message)
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
     * @brief Class representing the settings menu of the game.
     */
    class SettingsMenu final : public AMenu {
      public:
        /**
         * @brief Construct a new Settings Menu object.
         *
         * @param renderer Shared pointer to the renderer used for rendering the settings menu.
         * @param musicRegistry Shared pointer to the music registry.
         * @param soundRegistry Shared pointer to the sound registry.
         */
        explicit SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer,
            std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry);

        /**
         * @brief Update the settings menu state.
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame) override;

        /**
         * @brief Render the settings menu.
         */
        void render() const override;

        /**
         * @brief Called when entering the settings menu.
         */
        void onEnter();

        /**
         * @brief Check if the user wants to go back from the settings menu.
         *
         * @return true if the user wants to go back, false otherwise.
         */
        [[nodiscard]] bool wantsBack() const noexcept;

        /**
         * @brief Check if the screen resolution has been changed.
         *
         * @return true if the resolution has been changed, false otherwise.
         */
        [[nodiscard]] bool resolutionChanged() noexcept;

        /**
         * @brief Get the current screen resolution.
         *
         * @return The current screen resolution.
         */
        [[nodiscard]] Graphics::Extent2u currentResolution() const noexcept;

        /**
         * @brief Check if the controls preset has been changed.
         *
         * @return true if the controls preset has been changed, false otherwise.
         */
        [[nodiscard]] bool controlsChanged() const noexcept;

        /**
         * @brief Handle resizing of the settings menu.
         */
        void layout() override;

      private:
        static constexpr int ERROR_DISPLAY_FRAMES = 60; ///> Number of frames to display error feedback

        /**
         * @brief Handle input for the settings menu.
         * @param frame The current input frame.
         */
        void handleInput(const InputFrame &frame);

        /**
         * @brief Handle mouse pressed events.
         * @param frame The current input frame.
         */
        void handleMousePressed(const InputFrame &frame) const;

        /**
         * @brief Handle mouse released events.
         * @param frame The current input frame.
         */
        void handleMouseReleased(const InputFrame &frame);

        /**
         * @brief Handle key pressed events.
         * @param frame The current input frame.
         */
        void handleKeyPressed(const InputFrame &frame);

        /**
         * @brief Handle video settings button releases.
         * @param mx Mouse x-coordinate.
         * @param my Mouse y-coordinate.
         * @return true if a video settings button was released, false otherwise.
         */
        [[nodiscard]] bool handleVideoReleased(float mx, float my);

        /**
         * @brief Handle controls settings button releases.
         * @param mx Mouse x-coordinate.
         * @param my Mouse y-coordinate.
         * @return true if a controls settings button was released, false otherwise.
         */
        [[nodiscard]] bool handleControlsReleased(float mx, float my);

        /**
         * @brief Handle rebind button releases.
         * @param mx Mouse x-coordinate.
         * @param my Mouse y-coordinate.
         * @return true if a rebind button was released, false otherwise.
         */
        [[nodiscard]] bool handleRebindReleased(float mx, float my);

        /**
         * @brief Update the labels of the rebind buttons based on the current preset.
         */
        void updateRebindLabels();

        /**
         * @brief Handle audio settings button releases.
         * @param mx Mouse x-coordinate.
         * @param my Mouse y-coordinate.
         * @return true if an audio settings button was released, false otherwise.
         */
        [[nodiscard]] bool handleAudioReleased(float mx, float my);

        /**
         * @brief Handle navigation button releases.
         * @param mx Mouse x-coordinate.
         * @param my Mouse y-coordinate.
         * @return true if a navigation button was released, false otherwise.
         */
        [[nodiscard]] bool handleNavigationReleased(float mx, float my);

        /**
         * @brief Apply volume change to music registry, handling mute state.
         * @param volume The new volume value.
         * @param isMuted Whether the audio is currently muted.
         */
        void applyMusicVolumeChange(size_t volume, bool isMuted) noexcept;

        /**
         * @brief Apply volume change to sound registry, handling mute state.
         * @param volume The new volume value.
         * @param isMuted Whether the audio is currently muted.
         */
        void applySoundVolumeChange(size_t volume, bool isMuted) noexcept;

        std::shared_ptr<MusicRegistry> _musicRegistry; ///> Music registry
        std::shared_ptr<SoundRegistry> _soundRegistry; ///> Sound registry

        std::unique_ptr<UI::UIButton> _colorBlindMode; ///> Colorblind mode button
        std::unique_ptr<UI::UIButton> _colorBlindNext; ///> Next colorblind mode button
        std::unique_ptr<UI::UIButton> _left;           ///> Decrease resolution button
        std::unique_ptr<UI::UIButton> _right;          ///> Increase resolution button
        std::unique_ptr<UI::UIButton> _back;           ///> Back button
        std::unique_ptr<UI::UIButton> _resolution;     ///> Current resolution display button
        std::unique_ptr<UI::UIButton> _resolutionNext; ///> Next resolution button

        std::size_t _musicVolume = 50;           ///> Music volume level (0-100)
        std::size_t _sfxVolume = 50;             ///> SFX volume level (0-100)
        std::size_t _musicVolumeBeforeMute = 50; ///> Music volume before mute
        std::size_t _sfxVolumeBeforeMute = 50;   ///> SFX volume before mute
        bool _musicMuted = false;                ///> Music mute state
        bool _sfxMuted = false;                  ///> SFX mute state

        std::unique_ptr<UI::UIButton> _musicVolLabel; ///> Music volume display
        std::unique_ptr<UI::UIButton> _musicVolUp;    ///> Increase music volume
        std::unique_ptr<UI::UIButton> _musicVolDown;  ///> Decrease music volume
        std::unique_ptr<UI::UIButton> _sfxVolLabel;   ///> SFX volume display
        std::unique_ptr<UI::UIButton> _sfxVolUp;      ///> Increase SFX volume
        std::unique_ptr<UI::UIButton> _sfxVolDown;    ///> Decrease SFX volume
        std::unique_ptr<UI::UIButton> _muteMusic;     ///> Mute/unmute music
        std::unique_ptr<UI::UIButton> _muteSFX;       ///> Mute/unmute SFX
        std::unique_ptr<UI::UIButton> _controls;      ///> Current controls preset display button
        std::unique_ptr<UI::UIButton> _controlsNext;  ///> Next controls preset button

        std::unique_ptr<UI::UIButton> _rebindUp;    ///> Rebind up key button
        std::unique_ptr<UI::UIButton> _rebindDown;  ///> Rebind down key button
        std::unique_ptr<UI::UIButton> _rebindLeft;  ///> Rebind left key button
        std::unique_ptr<UI::UIButton> _rebindRight; ///> Rebind right key button
        std::unique_ptr<UI::UIButton> _rebindShoot; ///> Rebind shoot key button

        std::unique_ptr<Graphics::IText> _rebindUpLabel;    ///> Label for up key button
        std::unique_ptr<Graphics::IText> _rebindDownLabel;  ///> Label for down key button
        std::unique_ptr<Graphics::IText> _rebindLeftLabel;  ///> Label for left key button
        std::unique_ptr<Graphics::IText> _rebindRightLabel; ///> Label for right key button
        std::unique_ptr<Graphics::IText> _rebindShootLabel; ///> Label for shoot key button

        enum class RebindState { None, Up, Down, Left, Right, Shoot };
        RebindState _rebindState = RebindState::None; ///> Current rebinding state
        RebindState _errorState = RebindState::None;  ///> State for showing error feedback
        int _errorFrameCount = 0;                     ///> Frame counter for error feedback display
        bool _controlsChanged = false;                ///> Flag indicating if the controls preset has been changed
        bool _backRequested = false;                  ///> Flag indicating if the user wants to go back

        /**
         * @brief List of available screen resolutions.
         */
        std::vector<Graphics::Extent2u> _resolutions{{900, 600}, {1280, 720}, {1920, 1080}};
        std::size_t _currentResolution = 1; ///> Index of the current screen resolution
        bool _resolutionChanged = false;    ///> Flag indicating if the resolution has been changed

        Graphics::ColorBlindMode _currentColorBlindMode = Graphics::ColorBlindMode::NONE; ///> Current colorblind mode
    };
} // namespace Engine
