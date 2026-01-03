/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomMenu
*/

#pragma once

#include <cstdint>
#include <exception>

#include "IRenderer.hpp"
#include "IText.hpp"
#include "InputState.hpp"
#include "RenderCommand.hpp"
#include "RoomManager.hpp"
#include "UIButton.hpp"

namespace Engine
{
    /**
     * @class RoomMenuError
     * @brief Exception class for RoomMenu-related errors.
     */
    class RoomMenuError : public std::exception {
      public:
        /**
         * @brief Constructs a RoomMenuError with a given message.
         * @param message The error message.
         */
        explicit RoomMenuError(const std::string &message) : _message("\n\t" + message) {}

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override { return _message.c_str(); }

      private:
        std::string _message;   ///> The error message.
    };

    /**
     * @class RoomMenu
     * @brief Class representing the room menu UI.
     * This class handles the layout, input, and rendering of the room menu,
     * allowing users to create or join game rooms.
     */
    class RoomMenu {
      public:
        /**
         * @brief Constructs a RoomMenu with the given renderer and room manager.
         * @param renderer Shared pointer to the graphics renderer.
         * @param roomManager Shared pointer to the room manager.
         * @throws RoomMenuError if resources fail to load.
         */
        RoomMenu(const std::shared_ptr<Graphics::IRenderer> &renderer, const std::shared_ptr<RoomManager> &roomManager);

        /**
         * @brief Lays out the room menu UI elements.
         */
        void layout();

        /**
         * @brief Lays out the creation room UI elements.
         * @param w Width of the layout area.
         * @param h Height of the layout area.
         * @param cx Center x-coordinate of the layout area.
         */
        void layoutCreate(float w, float h, float cx) const;

        /**
         * @brief Updates the room menu based on input.
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame);

        /**
         * @brief Renders the room menu UI elements.
         */
        void render() const;

        /**
         * @brief Checks if the user wants to go back to the main menu.
         * @return True if the user wants to go back, false otherwise.
         */
        [[nodiscard]] bool wantsBackToMenu() const noexcept;

      private:
        /**
         * @enum Page
         * @brief Enum representing the current page of the room menu.
         */
        enum class Page { Root, Create };

        /**
         * @struct HeaderUI
         * @brief Struct representing the header UI elements.
         * @param title Pointer to the title text element.
         * @param subtitle Pointer to the subtitle text element.
         */
        struct HeaderUI {
            std::unique_ptr<Graphics::IText> title;
            std::unique_ptr<Graphics::IText> subtitle;
        };

        /**
         * @struct RootUI
         * @brief Struct representing the root UI elements.
         * @param create Pointer to the create button.
         * @param join Pointer to the join button.
         * @param back Pointer to the back button.
         */
        struct RootUI {
            std::unique_ptr<UI::UIButton> create;
            std::unique_ptr<UI::UIButton> join;
            std::unique_ptr<UI::UIButton> back;
        };

        /**
         * @struct CreateUI
         * @brief Struct representing the create room UI elements.
         * @param worldPrev Pointer to the previous world button.
         * @param worldNext Pointer to the next world button.
         * @param difficultyPrev Pointer to the previous difficulty button.
         * @param difficultyNext Pointer to the next difficulty button.
         * @param playersPrev Pointer to the previous players button.
         * @param playersNext Pointer to the next players button.
         * @param confirm Pointer to the confirm button.
         * @param back Pointer to the back button.
         * @param levelName Pointer to the level name text element.
         * @param worldLabel Pointer to the world label text element.
         * @param difficultyLabel Pointer to the difficulty label text element.
         * @param playersLabel Pointer to the players label text element.
         */
        struct CreateUI {
            std::unique_ptr<UI::UIButton> worldPrev;
            std::unique_ptr<UI::UIButton> worldNext;

            std::unique_ptr<UI::UIButton> difficultyPrev;
            std::unique_ptr<UI::UIButton> difficultyNext;

            std::unique_ptr<UI::UIButton> playersPrev;
            std::unique_ptr<UI::UIButton> playersNext;

            std::unique_ptr<UI::UIButton> confirm;
            std::unique_ptr<UI::UIButton> back;

            std::unique_ptr<Graphics::IText> levelName;
            std::unique_ptr<Graphics::IText> worldLabel;
            std::unique_ptr<Graphics::IText> difficultyLabel;
            std::unique_ptr<Graphics::IText> playersLabel;
        };

        /**
         * @brief Handles input for the room menu.
         * @param frame The current input frame.
         */
        void handleInput(const InputFrame &frame);

        /**
         * @brief Handles mouse press events.
         * @param frame The current input frame.
         */
        void handleMousePressed(const InputFrame &frame) const;

        /**
         * @brief Handles mouse release events.
         * @param mx The x-coordinate of the mouse.
         * @param my The y-coordinate of the mouse.
         */
        void handleMouseReleased(float mx, float my);

        /**
         * @brief Handles mouse release events on the create room page.
         * @param mx The x-coordinate of the mouse.
         * @param my The y-coordinate of the mouse.
         */
        void handleCreateReleased(float mx, float my);

        /**
         * @brief Updates hover states for UI elements.
         * @param mx The x-coordinate of the mouse.
         * @param my The y-coordinate of the mouse.
         */
        void updateHover(float mx, float my) const;

        /**
         * @brief Refreshes the creation room catalog based on current selections.
         */
        void refreshCreateCatalog();

        /**
         * @brief Updates text strings in the UI elements.
         */
        void updateTextStrings() const;

        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the graphics renderer.
        std::shared_ptr<RoomManager> _roomManager;  ///> Shared pointer to the room manager.

        Graphics::TextureHandle _backgroundTexture = Graphics::InvalidTexture; ///> Background texture handle.
        RenderCommand _backgroundCmd; ///> Render command for the background.

        HeaderUI _header;   ///> Header UI elements.
        RootUI _root;   ///> Root UI elements.
        CreateUI _create;   ///> Create room UI elements.
        Page _page = Page::Root;    ///> Current page of the room menu.

        std::vector<WorldEntry> _worlds;    ///> List of available worlds.
        std::vector<LevelInfo> _levels; ///> List of levels for the selected world and difficulty.

        int _selectedWorld = 0; ///> Index of the selected world.
        Difficulty _selectedDifficulty = Difficulty::Easy;  ///> Selected difficulty level.
        std::uint8_t _selectedMaxPlayers = 4;   ///> Selected maximum number of players.

        bool _backToMenu = false;   ///> Flag indicating if the user wants to go back to the main menu.
        bool _layoutDirty = true;   ///> Flag indicating if the layout needs to be updated.
    };
} // namespace Engine
