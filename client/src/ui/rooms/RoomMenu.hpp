/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomMenu
*/

#pragma once

#include <algorithm>
#include <cstdint>
#include <exception>
#include <iostream>
#include "AMenu.hpp"
#include "IRenderer.hpp"
#include "IText.hpp"
#include "InputState.hpp"
#include "RenderCommand.hpp"
#include "RoomManager.hpp"
#include "UIButton.hpp"

/**
 * @struct ListMetrics
 * @brief Struct to hold metrics for the room list UI.
 */
struct ListMetrics {
    float listTop = 0.f;    ///> Top boundary of the room list.
    float listBottom = 0.f; ///> Bottom boundary of the room list.
    float listH = 0.f;      ///> Height of the room list.

    float rowH = 0.f;     ///> Height of each row in the room list.
    float contentH = 0.f; ///> Total height of the content in the room list.

    float maxScroll = 0.f;  ///> Maximum scroll position.
    float scrollStep = 0.f; ///> Scroll step size.
};

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
        explicit RoomMenuError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> The error message.
    };

    /**
     * @class RoomMenu
     * @brief Class representing the room menu UI.
     * This class handles the layout, input, and rendering of the room menu,
     * allowing users to create or join game rooms.
     */
    class RoomMenu final : public AMenu {
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
        void layout() override;

        /**
         * @brief Updates the room menu based on input.
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame) override;

        /**
         * @brief Renders the room menu UI elements.
         */
        void render() const override;

        /**
         * @brief Checks if the user wants to go back to the main menu.
         * @return True if the user wants to go back, false otherwise.
         */
        [[nodiscard]] bool wantsBackToMenu() const noexcept;

        /**
         * @brief Checks if the user wants to create a room.
         * @return True if the user wants to create a room, false otherwise.
         */
        [[nodiscard]] bool wantsCreateRoom() const noexcept;

        /**
         * @brief Checks if the user wants to join a room.
         * @return True if the user wants to join a room, false otherwise.
         */
        [[nodiscard]] bool wantsJoinRoom() const noexcept;

        /**
         * @brief Checks if the user wants to list available rooms.
         * @return True if the user wants to list rooms, false otherwise.
         */
        [[nodiscard]] bool wantsListRooms() const noexcept;

        /**
         * @brief Consumes the create room state.
         */
        void consumeCreateRoomState() noexcept;

        /**
         * @brief Consumes the join room state.
         */
        void consumeJoinRoomState() noexcept;

        /**
         * @brief Consumes the list rooms request state.
         */
        void consumeListRoomsRequest() noexcept;

        /**
         * @brief Gets the maximum number of players selected for room creation.
         * @return The maximum number of players.
         */
        [[nodiscard]] uint8_t maxPlayerSelected() const noexcept;

        /**
         * @brief Gets the selected difficulty level.
         * @return The selected difficulty.
         */
        [[nodiscard]] Engine::Difficulty difficultySelected() const noexcept;

        /**
         * @brief Gets the selected level ID (path to the level file).
         * @return The level ID, or empty string if no level is selected.
         */
        [[nodiscard]] std::string levelSelected() const noexcept;

        /**
         * @brief Gets the selected room ID to join.
         * @return The room ID.
         */
        [[nodiscard]] uint32_t roomIdSelected() const noexcept;

      private:
        /**
         * @brief Lays out the creation room UI elements.
         * @param w Width of the layout area.
         * @param h Height of the layout area.
         * @param cx Center x-coordinate of the layout area.
         */
        void layoutCreate(float w, float h, float cx) const;

        /**
         * @brief Lays out the list rooms UI elements.
         * @param h Height of the layout area.
         * @param cx Center x-coordinate of the layout area.
         */
        void layoutList(float h, float cx);

        /**
         * @brief Update the list of available rooms from the room manager.
         */
        void updateListRooms();

        /**
         * @enum Page
         * @brief Enum representing the current page of the room menu.
         */
        enum class Page { Root, Create, List };

        /**
         * @struct HeaderUI
         * @brief Struct representing the header UI elements.
         * @brief title Pointer to the title text element.
         * @brief subtitle Pointer to the subtitle text element.
         */
        struct HeaderUI {
            std::unique_ptr<Graphics::IText> title;
            std::unique_ptr<Graphics::IText> subtitle;
        };

        /**
         * @struct RootUI
         * @brief Struct representing the root UI elements.
         * @brief create Pointer to the create button.
         * @brief join Pointer to the join button.
         * @brief back Pointer to the back button.
         */
        struct RootUI {
            std::unique_ptr<UI::UIButton> create;
            std::unique_ptr<UI::UIButton> join;
            std::unique_ptr<UI::UIButton> back;
        };

        /**
         * @struct CreateUI
         * @brief Struct representing the create room UI elements.
         */
        struct CreateUI {
            std::unique_ptr<UI::UIButton> worldPrev; ///> Previous world button.
            std::unique_ptr<UI::UIButton> worldNext; ///> Next world button.

            std::unique_ptr<UI::UIButton> levelPrev; ///> Previous level button.
            std::unique_ptr<UI::UIButton> levelNext; ///> Next level button.

            std::unique_ptr<UI::UIButton> difficultyPrev; ///> Previous difficulty button.
            std::unique_ptr<UI::UIButton> difficultyNext; ///> Next difficulty button.

            std::unique_ptr<UI::UIButton> playersPrev; ///> Previous players button.
            std::unique_ptr<UI::UIButton> playersNext; ///> Next players button.

            std::unique_ptr<UI::UIButton> confirm; ///> Confirm button.
            std::unique_ptr<UI::UIButton> back;    ///> Back button.

            std::unique_ptr<Graphics::IText> worldLabel;      ///> World label text.
            std::unique_ptr<Graphics::IText> levelLabel;      ///> Level label text.
            std::unique_ptr<Graphics::IText> difficultyLabel; ///> Difficulty label text.
            std::unique_ptr<Graphics::IText> playersLabel;    ///> Players label text.
        };

        /**
         * @struct ListUI
         * @brief Struct representing the list room UI elements.
         */
        struct ListUI {
            std::unordered_map<uint32_t, std::unique_ptr<UI::UIButton>> roomButtons; ///> Map of room ID to room button.
            std::unique_ptr<UI::UIButton> back;                                      ///> Back button.

            float listTop = 0.f;    ///> Top boundary of the room list.
            float listBottom = 0.f; ///> Bottom boundary of the room list.

            float scroll = 0.f;      ///> Current scroll position.
            float maxScroll = 0.f;   ///> Maximum scroll position.
            float scrollStep = 80.f; ///> Scroll step size.

            std::chrono::steady_clock::time_point lastRefresh{};                  ///> Last refresh time point.
            static constexpr auto refreshPeriod = std::chrono::milliseconds(500); ///> Refresh period.
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
         * @brief Handles mouse release events on the join room page.
         * @param mx The x-coordinate of the mouse.
         * @param my The y-coordinate of the mouse.
         */
        void handleJoinReleased(float mx, float my);

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

        std::shared_ptr<RoomManager> _roomManager; ///> Shared pointer to the room manager.

        HeaderUI _header;        ///> Header UI elements.
        RootUI _root;            ///> Root UI elements.
        CreateUI _create;        ///> Create room UI elements.
        Page _page = Page::Root; ///> Current page of the room menu.
        ListUI _list;            ///> List room UI elements.

        std::vector<WorldEntry> _worlds; ///> List of available worlds.
        std::vector<LevelInfo> _levels;  ///> List of levels for the selected world and difficulty.

        int _selectedWorld = 0;                                            ///> Index of the selected world.
        int _selectedLevel = 0;                                            ///> Index of the selected level.
        Engine::Difficulty _selectedDifficulty = Engine::Difficulty::Easy; ///> Selected difficulty level.
        uint8_t _selectedMaxPlayers = 4;                                   ///> Selected maximum number of players.
        uint32_t _joinRoomId = 0;                                          ///> ID of the room to join.

        bool _backToMenu = false; ///> Flag indicating if the user wants to go back to the main menu.
        bool _createRoom = false; ///> Flag indicating if the user wants to create a room.
        bool _listRooms = false;  ///> Flag indicating if the user wants to list available rooms.
        bool _joinRoom = false;   ///> Flag indicating if the user wants to join a room.
        bool _layoutDirty = true; ///> Flag indicating if the layout needs to be updated.
    };
} // namespace Engine
