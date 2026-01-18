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
#include <unordered_map>

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
    class SettingsMenuError : public std::exception {
      public:
        /**
         * @brief Constructor for SettingsMenuError.
         * @param message The error message.
         */
        explicit SettingsMenuError(const std::string &message) : _message(message)
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
         * @brief Enum representing the current state of key rebinding.
         */
        enum class RebindState {
            None,  ///> No rebinding in progress
            Up,    ///> Rebinding Up
            Down,  ///> Rebinding Down
            Left,  ///> Rebinding Left
            Right, ///> Rebinding Right
            Shoot  ///> Rebinding Shoot
        };

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

        std::shared_ptr<Graphics::IText> _title;             ///> Title text
        std::shared_ptr<Graphics::IText> _musicVolValueText; ///> Music volume value text
        std::shared_ptr<Graphics::IText> _sfxVolValueText;   ///> SFX volume value text

        std::unique_ptr<UI::UIButton> _musicVolUp;   ///> Music volume up button
        std::unique_ptr<UI::UIButton> _musicVolDown; ///> Music volume down button
        std::unique_ptr<UI::UIButton> _sfxVolUp;     ///> SFX volume up button
        std::unique_ptr<UI::UIButton> _sfxVolDown;   ///> SFX volume down button

        std::unique_ptr<UI::UIButton> _muteMusic; ///> Mute music button
        std::unique_ptr<UI::UIButton> _muteSFX;   ///> Mute SFX button

        std::unique_ptr<UI::UIButton> _colorBlindCycle; ///> Colorblind mode cycle button
        std::unique_ptr<UI::UIButton> _resolutionCycle; ///> Resolution cycle button
        std::unique_ptr<UI::UIButton> _controlsCycle;   ///> Controls preset cycle button

        std::unique_ptr<UI::UIButton> _back; ///> Back button

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

        std::shared_ptr<Graphics::IText> _colorBlindTitleText; ///> Colorblind title text
        std::shared_ptr<Graphics::IText> _resolutionTitleText; ///> Resolution title text
        std::shared_ptr<Graphics::IText> _controlsTitleText;   ///> Controls title text

        size_t _musicVolume = 50;           ///> Music volume level (0-100)
        size_t _sfxVolume = 50;             ///> SFX volume level (0-100)
        size_t _musicVolumeBeforeMute = 50; ///> Music volume before mute
        size_t _sfxVolumeBeforeMute = 50;   ///> SFX volume before mute
        bool _musicMuted = false;           ///> Music mute state
        bool _sfxMuted = false;             ///> SFX mute state
        bool _backRequested = false;        ///> Flag indicating if back navigation is requested
        bool _resolutionChanged = false;    ///> Flag indicating if resolution has been changed
        bool _controlsChanged = false;      ///> Flag indicating if controls preset has been changed

        RebindState _rebindState = RebindState::None;      ///> Current rebinding state
        RebindState _errorState = RebindState::None;       ///> State for showing error feedback
        int _errorFrameCount = 0;                          ///> Frame counter for error feedback display
        static constexpr size_t ERROR_DISPLAY_FRAMES = 30; ///> Number of frames to display error feedback

        std::unordered_map<RebindState, UI::UIButton *> _rebindButtonMap; ///> Map from rebind state to button pointer

        /**
         * @brief List of available screen resolutions.
         */
        std::vector<Graphics::Extent2u> _resolutions{{900, 600}, {1280, 720}, {1920, 1080}};
        size_t _currentResolution = 1; ///> Index of the current screen resolution
        Graphics::ColorBlindMode _currentColorBlindMode = Graphics::ColorBlindMode::NONE; ///> Current colorblind mode
    };
} // namespace Engine
