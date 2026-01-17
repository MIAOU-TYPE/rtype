/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelEditorState
*/

#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "AuthContext.hpp"
#include "EventRegistry.hpp"
#include "IGameState.hpp"
#include "IGraphics.hpp"
#include "IRenderer.hpp"
#include "IText.hpp"
#include "MenuState.hpp"
#include "MusicRegistry.hpp"
#include "RenderCommand.hpp"
#include "RoomManager.hpp"
#include "RoomTypes.hpp"
#include "ScoreboardContext.hpp"
#include "SoundRegistry.hpp"
#include "StateManager.hpp"
#include "UIButton.hpp"
#include "UITextField.hpp"
#include <unordered_map>

/**
 * @brief Type alias for nlohmann::json for convenience.
 */
using json = nlohmann::json;

namespace Engine
{
    /**
     * @class LevelEditorException
     * @brief Exception class for level editor errors.
     */
    class LevelEditorException : public std::exception {
      public:
        /**
         * @brief Constructor for LevelEditorException.
         * @param message The error message.
         */
        explicit LevelEditorException(const std::string &message) : _message("\n\t" + message)
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
     * @struct EntityType
     * @brief Represents a type of entity that can be placed in the level.
     */
    struct EntityType {
        std::string name;       ///> Name of the entity type
        unsigned int spriteId;  ///> Sprite ID for rendering
        float width;            ///> Width of the entity
        float height;           ///> Height of the entity
        std::string jsonType;   ///> Type identifier for JSON export
        std::string spriteName; ///> Sprite texture name (e.g., "enemy", "boss")
    };

    /**
     * @struct PlacedEntity
     * @brief Represents an entity placed in the level.
     */
    struct PlacedEntity {
        std::string type;      ///> Type of the entity
        float x;               ///> X position in world coordinates
        float y;               ///> Y position in world coordinates
        float spawnTime;       ///> Time when this entity spawns
        unsigned int spriteId; ///> Sprite ID for rendering
    };

    /**
     * @class LevelEditorState
     * @brief State for creating and editing game levels.
     */
    class LevelEditorState final : public IGameState {
      public:
        /**
         * @brief Construct a new Level Editor State object.
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the renderer interface.
         * @param musicRegistry Shared pointer to the music registry.
         * @param soundRegistry Shared pointer to the sound registry.
         * @param roomManager Shared pointer to the room manager.
         * @param eventBus Shared pointer to the event bus.
         * @param authCtx Shared pointer to the auth context.
         * @param scoreCtx Shared pointer to the scoreboard context.
         */
        explicit LevelEditorState(std::shared_ptr<Graphics::IGraphics> graphics,
            std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<MusicRegistry> musicRegistry,
            std::shared_ptr<SoundRegistry> soundRegistry, std::shared_ptr<RoomManager> roomManager,
            std::shared_ptr<EventBus> eventBus, std::shared_ptr<AuthContext> authCtx,
            std::shared_ptr<ScoreboardContext> scoreCtx);

        /**
         * @brief Called when entering the state.
         */
        void onEnter() override;

        /**
         * @brief Update the level editor state.
         * @param manager Reference to the state manager.
         * @param frame The current input frame.
         */
        void update(StateManager &manager, const InputFrame &frame) override;

        /**
         * @brief Render the level editor state.
         */
        void render() override;

      private:
        /**
         * @struct Background
         * @brief Represents a background option for the level.
         */
        struct Background {
            std::string name;        ///> Name of the background
            int spriteId;            ///> Sprite ID for rendering
            float scrollSpeed;       ///> Scrolling speed of the background
            float tileWidth;         ///> Width of the background tile
            float tileHeight;        ///> Height of the background tile
            std::string texturePath; ///> Path to the background texture
        };

        /**
         * @brief Initialize the entity types available for placement.
         */
        void initializeEntityTypes();

        /**
         * @brief Initialize the background options.
         */
        void initializeBackgrounds();

        /**
         * @brief Update the background display.
         */
        void updateBackgroundDisplay();

        /**
         * @brief Handle user input.
         * @param frame The current input frame.
         */
        void handleInput(const InputFrame &frame);

        /**
         * @brief Handle mouse press events.
         * @param frame The current input frame.
         */
        void handleMousePressed(const InputFrame &frame);

        /**
         * @brief Handle mouse release events.
         * @param frame The current input frame.
         */
        void handleMouseReleased(const InputFrame &frame);

        /**
         * @brief Handle key press events.
         * @param frame The current input frame.
         */
        void handleKeyPressed(const InputFrame &frame);

        /**
         * @brief Update button states based on mouse position.
         * @param mouseX Mouse X position.
         * @param mouseY Mouse Y position.
         */
        void updateButtons(float mouseX, float mouseY);

        /**
         * @brief Layout all UI elements.
         */
        void layout();

        /**
         * @brief Place an entity at the specified screen coordinates.
         * @param screenX Screen X coordinate.
         * @param screenY Screen Y coordinate.
         */
        void placeEntity(float screenX, float screenY);

        /**
         * @brief Remove entity at the specified screen coordinates.
         * @param screenX Screen X coordinate.
         * @param screenY Screen Y coordinate.
         */
        void removeEntityAt(float screenX, float screenY);

        /**
         * @brief Save the current level to a JSON file.
         * @return True if save was successful, false otherwise.
         */
        [[nodiscard]] bool saveLevel();

        /**
         * @brief Update the custom levels index file with the new level.
         * @param levelPath The path to the saved level file.
         * @param levelName The display name of the level.
         */
        void updateCustomLevelsIndex(const std::string &levelPath, const std::string &levelName);

        /**
         * @brief Convert screen coordinates to world coordinates.
         * @param screenX Screen X coordinate.
         * @param screenY Screen Y coordinate.
         * @param worldX Output world X coordinate.
         * @param worldY Output world Y coordinate.
         */
        void screenToWorld(const float screenX, const float screenY, float &worldX, float &worldY) const noexcept;

        /**
         * @brief Generate the filename for the level save.
         * @return The generated filename.
         */
        [[nodiscard]] std::string generateLevelFilename() const;

        /**
         * @brief Create the base JSON structure for the level.
         * @return The JSON object with name, background, and duration.
         */
        [[nodiscard]] json createLevelJson() const;

        /**
         * @brief Populate the enemies section of the JSON.
         * @param enemies The JSON object to populate.
         */
        void populateEnemiesJson(json &enemies) const;

        /**
         * @brief Populate the obstacles section of the JSON.
         * @param obstacles The JSON object to populate.
         */
        void populateObstaclesJson(json &obstacles) const;

        /**
         * @brief Create the waves array for the level.
         * @return The JSON array of waves.
         */
        [[nodiscard]] json createWavesJson() const;

        /**
         * @brief Write the JSON to a file.
         * @param j The JSON object to write.
         * @param filename The filename to write to.
         * @return True if successful, false otherwise.
         */
        [[nodiscard]] bool writeJsonToFile(const json &j, const std::string &filename) const;

        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Graphics interface
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Renderer interface
        std::shared_ptr<MusicRegistry> _musicRegistry;  ///> Music registry
        std::shared_ptr<SoundRegistry> _soundRegistry;  ///> Sound registry
        std::shared_ptr<RoomManager> _roomManager;      ///> Room manager
        std::shared_ptr<EventBus> _eventBus;            ///> Event bus
        std::shared_ptr<AuthContext> _authCtx;          ///> Auth context
        std::shared_ptr<ScoreboardContext> _scoreCtx;   ///> Scoreboard context

        std::vector<EntityType> _entityTypes; ///> Available entity types
        std::unordered_map<std::string, std::tuple<std::string, int, int>>
            _entitySpriteMap;                                ///> Map from jsonType to (spriteName, width, height)
        std::vector<Background> _backgrounds;                ///> Available backgrounds
        std::vector<PlacedEntity> _placedEntities;           ///> Entities placed in the level
        std::vector<std::unique_ptr<UI::UIButton>> _toolbar; ///> Toolbar buttons for entity types

        std::unique_ptr<UI::UIButton> _saveButton;        ///> Save button
        std::unique_ptr<UI::UIButton> _backButton;        ///> Back to menu button
        std::unique_ptr<UI::UIButton> _clearButton;       ///> Clear all entities button
        std::unique_ptr<UI::UIButton> _bgPrevButton;      ///> Previous background button
        std::unique_ptr<UI::UIButton> _bgNextButton;      ///> Next background button
        std::unique_ptr<Graphics::IText> _bgLabel;        ///> Background label
        std::unique_ptr<Graphics::IText> _timeLabel;      ///> Current time label
        std::unique_ptr<Graphics::IText> _helpText;       ///> Help text
        std::unique_ptr<Graphics::IText> _statusText;     ///> Status message text
        std::unique_ptr<UI::UITextField> _levelNameField; ///> Text field for level name input
        std::unique_ptr<UI::UIButton> _confirmSaveButton; ///> Confirm save button
        std::unique_ptr<UI::UIButton> _cancelSaveButton;  ///> Cancel save button

        int _selectedEntityType = 0;             ///> Index of currently selected entity type (-1 = none)
        float _currentTime = 0.0f;               ///> Current spawn time for new entities
        int _currentBackgroundIndex = 0;         ///> Index of current background
        bool _backRequested = false;             ///> Back to menu requested flag
        bool _showSaveDialog = false;            ///> Whether the save dialog is shown
        std::string _statusMessage;              ///> Current status message
        float _statusMessageTimer = 0.0f;        ///> Timer for status message fade
        std::string _levelName = "custom_level"; ///> Name of the level being created

        Graphics::TextureHandle _backgroundTexture = Graphics::InvalidTexture; ///> Current background texture
        RenderCommand _backgroundCmd;                                          ///> Background render command

        std::map<std::string, Graphics::TextureHandle> _entityTextures; ///> Preloaded entity textures by sprite name

        static constexpr float TOOLBAR_HEIGHT = 100.0f;    ///> Height of the toolbar
        static constexpr float BUTTON_SPACING = 10.0f;     ///> Spacing between toolbar buttons
        static constexpr float STATUS_DISPLAY_TIME = 3.0f; ///> Time to display status messages
    };
} // namespace Engine
