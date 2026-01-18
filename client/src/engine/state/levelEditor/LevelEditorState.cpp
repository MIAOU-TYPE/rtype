/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelEditorState
*/

#include "LevelEditorState.hpp"

namespace Engine
{
    LevelEditorState::LevelEditorState(std::shared_ptr<Graphics::IGraphics> graphics,
        std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<MusicRegistry> musicRegistry,
        std::shared_ptr<SoundRegistry> soundRegistry, std::shared_ptr<RoomManager> roomManager,
        std::shared_ptr<EventBus> eventBus, std::shared_ptr<AuthContext> authCtx,
        std::shared_ptr<ScoreboardContext> scoreCtx)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _roomManager(std::move(roomManager)),
          _eventBus(std::move(eventBus)), _authCtx(std::move(authCtx)), _scoreCtx(std::move(scoreCtx))
    {
    }

    void LevelEditorState::onEnter()
    {
        try {
            initializeEntityTypes();
            initializeBackgrounds();

            _toolbar.clear();
            _entityTextures.clear();
            _backRequested = false;
            _showSaveDialog = false;
            _statusMessage.clear();
            _statusMessageTimer = 0.0f;

            _saveButton = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "SAVE");
            _backButton = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");
            _clearButton = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "CLEAR");
            _bgPrevButton = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "<");
            _bgNextButton = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, ">");

            _bgLabel = _renderer->texts()->createText(20, {255, 255, 255, 255});
            _bgLabel->setString("Background: Space");

            updateBackgroundDisplay();

            _timeLabel = _renderer->texts()->createText(18, {255, 255, 255, 255});
            _timeLabel->setString("Time: 0s (left) - 120s (right)");

            _helpText = _renderer->texts()->createText(16, {200, 200, 200, 255});
            _helpText->setString("X position = spawn time (0-120s) | Backspace: remove last | S: save | C: clear");

            _statusText = _renderer->texts()->createText(18, {100, 255, 100, 255});
            _statusText->setString("");

            _levelNameField = std::make_unique<UI::UITextField>(_renderer, "Level Name", false);
            _levelNameField->setValue("custom_level");
            _confirmSaveButton = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "CONFIRM");
            _cancelSaveButton = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "CANCEL");

            for (size_t i = 0; i < _entityTypes.size(); ++i) {
                auto btn = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "");
                _toolbar.push_back(std::move(btn));
            }

            const auto textures = _renderer->textures();
            _entityTextures["enemy"] = textures->load("sprites/enemy.png");
            _entityTextures["enemy2"] = textures->load("sprites/enemy2.png");
            _entityTextures["enemy3"] = textures->load("sprites/enemy3.png");
            _entityTextures["boss"] = textures->load("sprites/boss.png");
            _entityTextures["boss2"] = textures->load("sprites/boss2.png");
            _entityTextures["boss3P1"] = textures->load("sprites/boss3/boss3P1.png");
            _entityTextures["obstacle"] = textures->load("sprites/obstacle.png");
            _entityTextures["power_up"] = textures->load("sprites/power_up.png");
            _entityTextures["power_up2"] = textures->load("sprites/power_up2.png");
            _entityTextures["power_up3"] = textures->load("sprites/power_up3.png");

            layout();
        } catch (const std::exception &e) {
            throw LevelEditorException(std::string("{LevelEditorState::onEnter} ") + e.what());
        }
    }

    void LevelEditorState::initializeEntityTypes()
    {
        _entityTypes.clear();
        _entitySpriteMap.clear();
        _entityTypes.reserve(10);

        _entityTypes.push_back({"Small Enemy", 2, 65.0f, 66.0f, "smallEnemy", "enemy"});
        _entityTypes.push_back({"Medium Enemy", 3, 65.0f, 49.0f, "mediumEnemy", "enemy2"});
        _entityTypes.push_back({"Fast Enemy", 4, 33.0f, 22.0f, "fastEnemy", "enemy3"});

        _entityTypes.push_back({"Boss", 1, 177.0f, 144.0f, "boss", "boss"});
        _entityTypes.push_back({"Boss 2", 31, 130.0f, 50.0f, "boss2", "boss2"});
        _entityTypes.push_back({"Boss 3", 22, 160.0f, 213.0f, "boss3", "boss3P1"});

        _entityTypes.push_back({"Obstacle", 15, 34.0f, 34.0f, "gravityWell", "obstacle"});

        _entityTypes.push_back({"Laser Power-Up", 13, 33.0f, 28.0f, "laser", "power_up"});
        _entityTypes.push_back({"Shield Power-Up", 18, 33.0f, 28.0f, "shield", "power_up2"});
        _entityTypes.push_back({"Bubble Power-Up", 19, 33.0f, 28.0f, "bubble", "power_up3"});

        _entitySpriteMap["smallEnemy"] = std::make_tuple("enemy", 65, 66);
        _entitySpriteMap["mediumEnemy"] = std::make_tuple("enemy2", 65, 49);
        _entitySpriteMap["mediumEnemyTriple"] = std::make_tuple("enemy2", 65, 49);
        _entitySpriteMap["mediumEnemyWide"] = std::make_tuple("enemy2", 65, 49);
        _entitySpriteMap["mediumEnemyFive"] = std::make_tuple("enemy2", 65, 49);
        _entitySpriteMap["groupEnemy"] = std::make_tuple("enemy2", 65, 49);
        _entitySpriteMap["fastEnemy"] = std::make_tuple("enemy3", 33, 22);
        _entitySpriteMap["boss"] = std::make_tuple("boss", 177, 144);
        _entitySpriteMap["boss2"] = std::make_tuple("boss2", 130, 50);
        _entitySpriteMap["boss3"] = std::make_tuple("boss3P1", 160, 213);
        _entitySpriteMap["gravityWell"] = std::make_tuple("obstacle", 34, 34);
        _entitySpriteMap["laser"] = std::make_tuple("power_up", 33, 28);
        _entitySpriteMap["shield"] = std::make_tuple("power_up2", 33, 28);
        _entitySpriteMap["bubble"] = std::make_tuple("power_up3", 33, 28);
    }

    void LevelEditorState::initializeBackgrounds()
    {
        _backgrounds.clear();
        _backgrounds.reserve(3);

        _backgrounds.push_back({"Space", 100, -50.0f, 1140.0f, 207.0f, "sprites/background_space.png"});
        _backgrounds.push_back({"Jungle", 101, -50.0f, 2584.0f, 720.0f, "sprites/background_jungle.png"});
        _backgrounds.push_back({"Ruin", 102, -50.0f, 1148.0f, 223.0f, "sprites/background_ruin.png"});
    }

    void LevelEditorState::updateBackgroundDisplay()
    {
        const auto &bg = _backgrounds[static_cast<size_t>(_currentBackgroundIndex)];
        _backgroundTexture = _renderer->textures()->load(bg.texturePath);
        if (_backgroundTexture != Graphics::InvalidTexture) {
            const auto size = _renderer->textures()->getSize(_backgroundTexture);
            _backgroundCmd.textureId = _backgroundTexture;
            _backgroundCmd.frame = {0, 0, static_cast<int>(size.width), static_cast<int>(size.height)};
            _backgroundCmd.position = {0, 0};
            const auto vp = _renderer->getViewportSize();
            const float scale = static_cast<float>(vp.height) / static_cast<float>(size.height);
            _backgroundCmd.scale = {scale, scale};
        }
        _bgLabel->setString("Background: " + bg.name);
    }

    void LevelEditorState::layout()
    {
        const auto vp = _renderer->getViewportSize();
        const float w = static_cast<float>(vp.width);
        const float h = static_cast<float>(vp.height);

        float toolbarX = BUTTON_SPACING;
        const float toolbarY = BUTTON_SPACING;

        for (auto &btn : _toolbar) {
            btn->setPosition(toolbarX, toolbarY);
            toolbarX += 120.0f + BUTTON_SPACING;
        }

        const float bottomY = h - 70.0f;
        _backButton->setPosition(BUTTON_SPACING, bottomY);
        _saveButton->setPosition(w - 220.0f, bottomY);
        _clearButton->setPosition(w * 0.5f - 100.0f, bottomY);

        const float bgY = TOOLBAR_HEIGHT + 20.0f;
        _bgPrevButton->setPosition(BUTTON_SPACING, bgY);
        _bgNextButton->setPosition(BUTTON_SPACING + 120.0f, bgY);
        _bgLabel->setPosition(BUTTON_SPACING + 240.0f, bgY + 10.0f);

        _timeLabel->setPosition(w - 150.0f, bgY + 10.0f);

        if (_helpText)
            _helpText->setPosition(w * 0.5f - _helpText->getWidth() * 0.5f, h - 100.0f);

        if (_statusText && !_statusMessage.empty())
            _statusText->setPosition(w * 0.5f - _statusText->getWidth() * 0.5f, h * 0.5f);

        if (_levelNameField) {
            _levelNameField->setPosition(w * 0.5f - 200.0f, h * 0.5f - 50.0f);
            _levelNameField->setWidth(400.0f);
            _levelNameField->layout();
        }
        if (_confirmSaveButton)
            _confirmSaveButton->setPosition(w * 0.5f - 220.0f, h * 0.5f + 20.0f);
        if (_cancelSaveButton)
            _cancelSaveButton->setPosition(w * 0.5f + 20.0f, h * 0.5f + 20.0f);
    }

    void LevelEditorState::update(StateManager &manager, const InputFrame &frame)
    {
        handleInput(frame);
        updateButtons(frame.mouseX, frame.mouseY);

        if (_statusMessageTimer > 0.0f) {
            _statusMessageTimer -= 0.016f;
            if (_statusMessageTimer <= 0.0f) {
                _statusMessage.clear();
                _statusText->setString("");
            }
        }

        if (_backRequested) {
            manager.queueState(std::make_unique<MenuState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx, _scoreCtx));
        }
    }

    void LevelEditorState::render()
    {
        if (_showSaveDialog) {
            if (_levelNameField)
                _levelNameField->render();
            if (_confirmSaveButton)
                _confirmSaveButton->render();
            if (_cancelSaveButton)
                _cancelSaveButton->render();
            return;
        }

        if (_backgroundTexture != Graphics::InvalidTexture)
            _renderer->draw(_backgroundCmd);

        for (const auto &entity : _placedEntities) {
            RenderCommand cmd;

            auto spriteIt = _entitySpriteMap.find(entity.type);
            if (spriteIt == _entitySpriteMap.end())
                continue;

            auto [spriteName, frameWidth, frameHeight] = spriteIt->second;

            auto it = _entityTextures.find(spriteName);
            if (it != _entityTextures.end() && it->second != Graphics::InvalidTexture) {
                cmd.textureId = it->second;
                cmd.frame = {0, 0, frameWidth, frameHeight};
                cmd.position = {entity.x, entity.y};
                cmd.scale = {1.0f, 1.0f};
                _renderer->draw(cmd);
            }
        }

        for (size_t i = 0; i < _toolbar.size(); ++i) {
            _toolbar[i]->render();

            if (i < _entityTypes.size()) {
                const auto &entityType = _entityTypes[i];
                auto it = _entityTextures.find(entityType.spriteName);
                if (it != _entityTextures.end() && it->second != Graphics::InvalidTexture) {
                    RenderCommand cmd;
                    cmd.textureId = it->second;
                    cmd.frame = {0, 0, static_cast<int>(entityType.width), static_cast<int>(entityType.height)};

                    const float buttonX = BUTTON_SPACING + static_cast<float>(i) * (120.0f + BUTTON_SPACING);
                    const float buttonY = BUTTON_SPACING;
                    const float buttonWidth = 90.0f;
                    const float buttonHeight = 60.0f;
                    const float buttonPaddingX = 5.0f;
                    const float buttonPaddingY = 5.0f;

                    float scale = 0.5f;
                    const float availableWidth = buttonWidth - 2 * buttonPaddingX;
                    const float availableHeight = buttonHeight - 2 * buttonPaddingY;

                    if (entityType.width > availableWidth || entityType.height > availableHeight) {
                        const float scaleX = availableWidth / entityType.width;
                        const float scaleY = availableHeight / entityType.height;
                        scale = std::min(scaleX, scaleY);
                    }

                    cmd.position = {buttonX + buttonPaddingX + (availableWidth - entityType.width * scale) * 0.5f,
                        buttonY + buttonPaddingY + (availableHeight - entityType.height * scale) * 0.5f};
                    cmd.scale = {scale, scale};
                    _renderer->draw(cmd);
                }
            }

            if (static_cast<int>(i) == _selectedEntityType) {}
        }

        _backButton->render();
        _saveButton->render();
        _clearButton->render();
        _bgPrevButton->render();
        _bgNextButton->render();

        _renderer->draw(*_bgLabel);
        _renderer->draw(*_timeLabel);
        if (_helpText)
            _renderer->draw(*_helpText);
        if (_statusText && !_statusMessage.empty())
            _renderer->draw(*_statusText);
    }

    void LevelEditorState::handleInput(const InputFrame &frame)
    {
        if (_showSaveDialog) {
            if (frame.mousePressed && _levelNameField)
                _levelNameField->onMousePressed(frame.mouseX, frame.mouseY);
            if (frame.keyPressed && _levelNameField && _levelNameField->isFocused())
                _levelNameField->onKeyPressed(frame.key);
            if (frame.mousePressed) {
                if (_confirmSaveButton)
                    _confirmSaveButton->onMousePressed(frame.mouseX, frame.mouseY);
                if (_cancelSaveButton)
                    _cancelSaveButton->onMousePressed(frame.mouseX, frame.mouseY);
            }
            if (frame.mouseReleased) {
                if (_confirmSaveButton && _confirmSaveButton->onMouseReleased(frame.mouseX, frame.mouseY)) {
                    _levelName = _levelNameField->value();
                    if (saveLevel()) {
                        _statusMessage = "Level saved successfully!";
                        _statusText->setString(_statusMessage);
                        _statusText->setColor({100, 255, 100, 255});
                        _statusMessageTimer = STATUS_DISPLAY_TIME;
                    } else {
                        _statusMessage = "Failed to save level!";
                        _statusText->setString(_statusMessage);
                        _statusText->setColor({255, 100, 100, 255});
                        _statusMessageTimer = STATUS_DISPLAY_TIME;
                    }
                    _showSaveDialog = false;
                    _confirmSaveButton->reset();
                    layout();
                    return;
                }
                if (_cancelSaveButton && _cancelSaveButton->onMouseReleased(frame.mouseX, frame.mouseY)) {
                    _showSaveDialog = false;
                    _cancelSaveButton->reset();
                    return;
                }
            }
            return;
        }

        if (frame.mousePressed)
            handleMousePressed(frame);
        if (frame.mouseReleased)
            handleMouseReleased(frame);
        if (frame.keyPressed)
            handleKeyPressed(frame);
    }

    void LevelEditorState::handleMousePressed(const InputFrame &frame)
    {
        for (auto &btn : _toolbar) {
            btn->onMousePressed(frame.mouseX, frame.mouseY);
        }
        _backButton->onMousePressed(frame.mouseX, frame.mouseY);
        _saveButton->onMousePressed(frame.mouseX, frame.mouseY);
        _clearButton->onMousePressed(frame.mouseX, frame.mouseY);
        _bgPrevButton->onMousePressed(frame.mouseX, frame.mouseY);
        _bgNextButton->onMousePressed(frame.mouseX, frame.mouseY);
    }

    void LevelEditorState::handleMouseReleased(const InputFrame &frame)
    {
        for (size_t i = 0; i < _toolbar.size(); ++i) {
            if (_toolbar[i]->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _selectedEntityType = static_cast<int>(i);
                _toolbar[i]->reset();
                return;
            }
        }

        if (_backButton->onMouseReleased(frame.mouseX, frame.mouseY)) {
            _backRequested = true;
            _backButton->reset();
            return;
        }

        if (_saveButton->onMouseReleased(frame.mouseX, frame.mouseY)) {
            _showSaveDialog = true;
            _saveButton->reset();
            return;
        }

        if (_clearButton->onMouseReleased(frame.mouseX, frame.mouseY)) {
            _placedEntities.clear();
            _statusMessage = "All entities cleared!";
            _statusText->setString(_statusMessage);
            _statusText->setColor({255, 255, 100, 255});
            _statusMessageTimer = STATUS_DISPLAY_TIME;
            _clearButton->reset();
            layout();
            return;
        }

        if (_bgPrevButton->onMouseReleased(frame.mouseX, frame.mouseY)) {
            _currentBackgroundIndex = (_currentBackgroundIndex - 1 + static_cast<int>(_backgrounds.size()))
                % static_cast<int>(_backgrounds.size());
            updateBackgroundDisplay();
            _bgPrevButton->reset();
            return;
        }

        if (_bgNextButton->onMouseReleased(frame.mouseX, frame.mouseY)) {
            _currentBackgroundIndex = (_currentBackgroundIndex + 1) % static_cast<int>(_backgrounds.size());
            updateBackgroundDisplay();
            _bgNextButton->reset();
            return;
        }

        const auto vp = _renderer->getViewportSize();
        if (frame.mouseY > TOOLBAR_HEIGHT && frame.mouseY < static_cast<float>(vp.height) - 120.0f) {
            if (_selectedEntityType >= 0 && _selectedEntityType < static_cast<int>(_entityTypes.size()))
                placeEntity(frame.mouseX, frame.mouseY);
        }
    }

    void LevelEditorState::handleKeyPressed(const InputFrame &frame)
    {
        switch (frame.key) {
            case Key::Escape: _backRequested = true; break;
            case Key::S: _showSaveDialog = true; break;
            case Key::C:
                _placedEntities.clear();
                _statusMessage = "All entities cleared!";
                _statusText->setString(_statusMessage);
                _statusText->setColor({255, 255, 100, 255});
                _statusMessageTimer = STATUS_DISPLAY_TIME;
                layout();
                break;
            case Key::Backspace:
                if (!_placedEntities.empty()) {
                    _placedEntities.pop_back();
                    _statusMessage = "Last entity removed";
                    _statusText->setString(_statusMessage);
                    _statusText->setColor({255, 200, 100, 255});
                    _statusMessageTimer = STATUS_DISPLAY_TIME;
                    layout();
                }
                break;
            default: break;
        }
    }

    void LevelEditorState::updateButtons(const float mouseX, const float mouseY)
    {
        for (auto &btn : _toolbar) {
            btn->update(mouseX, mouseY);
        }
        _backButton->update(mouseX, mouseY);
        _saveButton->update(mouseX, mouseY);
        _clearButton->update(mouseX, mouseY);
        _bgPrevButton->update(mouseX, mouseY);
        _bgNextButton->update(mouseX, mouseY);
    }

    void LevelEditorState::placeEntity(const float screenX, const float screenY)
    {
        if (_selectedEntityType < 0 || _selectedEntityType >= static_cast<int>(_entityTypes.size()))
            return;

        float worldX, worldY;
        screenToWorld(screenX, screenY, worldX, worldY);

        const auto &entityType = _entityTypes[static_cast<size_t>(_selectedEntityType)];

        const float centeredX = worldX - (entityType.width * 0.5f);
        const float centeredY = worldY - (entityType.height * 0.5f);

        const auto vp = _renderer->getViewportSize();
        const float spawnTime = (worldX / static_cast<float>(vp.width)) * 120.0f;

        PlacedEntity entity;
        entity.type = entityType.jsonType;
        entity.x = centeredX;
        entity.y = centeredY;
        entity.spawnTime = spawnTime;
        entity.spriteId = entityType.spriteId;

        _placedEntities.push_back(entity);

        std::ostringstream oss;
        oss << "Placed " << entityType.name << " at (" << static_cast<int>(worldX) << ", " << static_cast<int>(worldY)
            << ") - Time: " << std::fixed << std::setprecision(1) << spawnTime << "s";
        _statusMessage = oss.str();
        _statusText->setString(_statusMessage);
        _statusText->setColor({100, 255, 100, 255});
        _statusMessageTimer = STATUS_DISPLAY_TIME;
        layout();
    }

    void LevelEditorState::removeEntityAt(const float screenX, const float screenY)
    {
        float worldX, worldY;
        screenToWorld(screenX, screenY, worldX, worldY);

        const float REMOVE_THRESHOLD = 50.0f;
        for (auto it = _placedEntities.begin(); it != _placedEntities.end(); ++it) {
            const float dx = it->x - worldX;
            const float dy = it->y - worldY;
            const float distSq = dx * dx + dy * dy;

            if (distSq < REMOVE_THRESHOLD * REMOVE_THRESHOLD) {
                _placedEntities.erase(it);
                _statusMessage = "Entity removed";
                _statusText->setString(_statusMessage);
                _statusText->setColor({255, 200, 100, 255});
                _statusMessageTimer = STATUS_DISPLAY_TIME;
                layout();
                return;
            }
        }
    }

    void LevelEditorState::screenToWorld(
        const float screenX, const float screenY, float &worldX, float &worldY) const noexcept
    {
        worldX = screenX;
        worldY = screenY;
    }

    std::string LevelEditorState::generateLevelFilename() const
    {
        const auto now = std::time(nullptr);
        const auto tm = std::localtime(&now);
        std::ostringstream filename;
        filename << "levels/" << _levelName << "_" << std::setfill('0') << std::setw(2) << tm->tm_mday << "-"
                 << std::setfill('0') << std::setw(2) << (tm->tm_mon + 1) << "-" << std::setfill('0') << std::setw(2)
                 << (tm->tm_year % 100) << ".json";
        return filename.str();
    }

    json LevelEditorState::createLevelJson() const
    {
        json j;
        j["name"] = _levelName;

        const auto &bg = _backgrounds[static_cast<size_t>(_currentBackgroundIndex)];
        j["background"] = {{"spriteId", bg.spriteId}, {"scrollSpeed", bg.scrollSpeed}, {"tileWidth", bg.tileWidth},
            {"tileHeight", bg.tileHeight}};

        float maxTime = 0.0f;
        for (const auto &entity : _placedEntities) {
            if (entity.spawnTime > maxTime)
                maxTime = entity.spawnTime;
        }
        j["duration"] = static_cast<int>(maxTime + 10.0f);

        return j;
    }

    void LevelEditorState::populateEnemiesJson(json &enemies) const
    {
        enemies = json::object();

        auto addEnemy = [&](const std::string &key, int hp, int speed, int w, int h, int killScore,
                            const std::string &sprite, int spriteId, const json &shoot,
                            const json &movement = json::object(), const json &phases = json::array()) {
            json e;
            e["hp"] = hp;
            e["speed"] = speed;
            e["size"] = {{"w", w}, {"h", h}};
            e["killScore"] = killScore;
            e["sprite"] = sprite;
            e["spriteId"] = spriteId;

            if (!shoot.is_null() && !shoot.empty())
                e["shoot"] = shoot;
            if (!movement.is_null() && !movement.empty())
                e["movement"] = movement;
            if (!phases.is_null() && phases.is_array() && !phases.empty())
                e["phases"] = phases;

            enemies[key] = std::move(e);
        };

        addEnemy("smallEnemy", 15, -100, 65, 66, 30, "enemy", 2,
            {
                {"type", "straight"},
                {"cooldown", 2.0},
                {"projectileSpeed", 130},
                {"damage", 50},
                {"muzzle", {{"x", -20}, {"y", 50}}},
                {"projectileSpriteId", 9},
            });

        addEnemy("mediumEnemy", 40, -70, 65, 49, 50, "enemy2", 3,
            {
                {"type", "diagonal"},
                {"angles", json::array({-15, 15})},
                {"cooldown", 2.0},
                {"projectileSpeed", 100},
                {"damage", 50},
                {"muzzle", {{"x", -20}, {"y", 35}}},
                {"projectileSpriteId", 9},
            });

        addEnemy("mediumEnemyTriple", 40, -70, 65, 49, 65, "enemy2", 3,
            {
                {"type", "diagonal"},
                {"angles", json::array({-25, 0, 25})},
                {"cooldown", 2.2},
                {"projectileSpeed", 110},
                {"damage", 50},
                {"muzzle", {{"x", -20}, {"y", 35}}},
                {"projectileSpriteId", 12},
            });

        addEnemy("mediumEnemyWide", 45, -70, 65, 49, 80, "enemy2", 3,
            {
                {"type", "diagonal"},
                {"angles", json::array({-35, -15, 15, 35})},
                {"cooldown", 2.6},
                {"projectileSpeed", 105},
                {"damage", 45},
                {"muzzle", {{"x", -20}, {"y", 35}}},
                {"projectileSpriteId", 12},
            });

        addEnemy("mediumEnemyFive", 50, -70, 65, 49, 110, "enemy2", 3,
            {
                {"type", "diagonal"},
                {"angles", json::array({-40, -20, 0, 20, 40})},
                {"cooldown", 3.0},
                {"projectileSpeed", 110},
                {"damage", 40},
                {"muzzle", {{"x", -20}, {"y", 35}}},
                {"projectileSpriteId", 12},
            });

        addEnemy("fastEnemy", 10, -125, 33, 22, 10, "enemy3", 4,
            {
                {"type", "straight"},
                {"cooldown", 0.8},
                {"projectileSpeed", 160},
                {"damage", 50},
                {"muzzle", {{"x", -20}, {"y", 15}}},
                {"projectileSpriteId", 9},
            },
            {
                {"type", "zigzag"},
                {"params", {{"amplitude", 50.0}, {"frequency", 0.5}}},
            });

        addEnemy("boss", 3000, -30, 177, 144, 300, "boss", 1,
            {
                {"type", "diagonal"},
                {"angles", json::array({-90, -45, 0, -315, -270})},
                {"cooldown", 1.6},
                {"projectileSpeed", 140},
                {"damage", 35},
                {"muzzle", {{"x", 105}, {"y", 112}}},
                {"projectileSpriteId", 12},
            },
            json::object(),
            json::array({
                {{"hpThreshold", 2500}, {"angles", json::array({-15, 0, 15})}, {"fireSpeedMultiplier", 1.2},
                    {"damageMultiplier", 1}, {"spriteId", 1}},
                {{"hpThreshold", 1500}, {"angles", json::array({-30, 30})}, {"fireSpeedMultiplier", 1.5},
                    {"damageMultiplier", 2}, {"spriteId", 1}},
                {{"hpThreshold", 500}, {"angles", json::array({-45, 45})}, {"fireSpeedMultiplier", 2.0},
                    {"damageMultiplier", 2}, {"spriteId", 1}},
            }));

        addEnemy("boss2", 3000, -30, 130, 50, 300, "boss2", 31,
            {
                {"type", "spread"},
                {"bulletsNbr", 5},
                {"cooldown", 0.5},
                {"projectileSpeed", 150},
                {"damage", 50},
                {"muzzle", {{"x", 65}, {"y", 25}}},
                {"projectileSpriteId", 12},
            },
            json::object(),
            json::array({
                {{"hpThreshold", 2500}, {"angles", json::array()}, {"fireSpeedMultiplier", 1.0},
                    {"damageMultiplier", 1}, {"spriteId", 31}},
                {{"hpThreshold", 1500}, {"angles", json::array()}, {"fireSpeedMultiplier", 1.3},
                    {"damageMultiplier", 3}, {"spriteId", 31}},
                {{"hpThreshold", 500}, {"angles", json::array()}, {"fireSpeedMultiplier", 1.5}, {"damageMultiplier", 1},
                    {"spriteId", 31}},
            }));

        addEnemy("boss3", 4998, -30, 160, 213, 300, "boss3P1", 22,
            {
                {"type", "homing"},
                {"cooldown", 4.0},
                {"projectileSpeed", 130},
                {"damage", 50},
                {"muzzle", {{"x", 60}, {"y", 70}}},
                {"projectileSpriteId", 29},
            },
            json::object(),
            json::array({
                {{"hpThreshold", 4998}, {"angles", json::array()}, {"fireSpeedMultiplier", 1.0},
                    {"damageMultiplier", 1}, {"spriteId", 22}},
                {{"hpThreshold", 4284}, {"angles", json::array()}, {"fireSpeedMultiplier", 1.0},
                    {"damageMultiplier", 1}, {"spriteId", 23}},
                {{"hpThreshold", 3570}, {"angles", json::array()}, {"fireSpeedMultiplier", 1.2},
                    {"damageMultiplier", 2}, {"spriteId", 24}},
                {{"hpThreshold", 2856}, {"angles", json::array()}, {"fireSpeedMultiplier", 1.5},
                    {"damageMultiplier", 2}, {"spriteId", 25}},
                {{"hpThreshold", 2142}, {"angles", json::array()}, {"fireSpeedMultiplier", 1.5},
                    {"damageMultiplier", 3}, {"spriteId", 26}},
                {{"hpThreshold", 1428}, {"angles", json::array()}, {"fireSpeedMultiplier", 2.0},
                    {"damageMultiplier", 3}, {"spriteId", 27}},
                {{"hpThreshold", 714}, {"angles", json::array()}, {"fireSpeedMultiplier", 2.0}, {"damageMultiplier", 4},
                    {"spriteId", 28}},
            }));

        enemies["groupEnemy"] = {{"type", "group"},
            {"members",
                json::array({
                    {{"enemyType", "mediumEnemy"}, {"offset", {{"x", 0}, {"y", 0}}}},
                    {{"enemyType", "smallEnemy"}, {"offset", {{"x", 0}, {"y", -130}}}},
                    {{"enemyType", "smallEnemy"}, {"offset", {{"x", 0}, {"y", 130}}}},
                })}};
    }

    void LevelEditorState::populateObstaclesJson(json &obstacles) const
    {
        obstacles["gravityWell"] = {{"spriteId", 15}, {"size", {{"w", 34}, {"h", 34}}}, {"pullStrength", 200},
            {"damagePerSecond", 10}, {"radius", 130}, {"innerRadius", 50}};
    }

    json LevelEditorState::createWavesJson() const
    {
        json waves = json::array();
        auto sortedEntities = _placedEntities;
        std::sort(sortedEntities.begin(), sortedEntities.end(), [](const PlacedEntity &a, const PlacedEntity &b) {
            return a.spawnTime < b.spawnTime;
        });

        for (const auto &entity : sortedEntities) {
            json wave;
            wave["time"] = entity.spawnTime;

            if (entity.type == "gravityWell") {
                wave["obstacleType"] = entity.type;
                wave["obstacleX"] = entity.x;
                wave["obstacleY"] = entity.y;
                wave["enemies"] = json::object();
            } else if (entity.type == "laser" || entity.type == "shield" || entity.type == "bubble") {
                wave["powerUp"] = json{{"type", entity.type}};
            } else {
                wave["enemies"] = json{{entity.type, 1}};
                wave["spawnPattern"] = "line";
                wave["spawnY"] = entity.y;
            }

            waves.push_back(wave);
        }

        return waves;
    }

    bool LevelEditorState::writeJsonToFile(const json &j, const std::string &filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open())
            return false;

        file << j.dump(2);
        file.close();
        return true;
    }

    void LevelEditorState::updateCustomLevelsIndex(const std::string &levelPath, const std::string &levelName)
    {
        try {
            const std::string indexPath = "client/assets/levels/custom/levels.json";

            json indexJson;
            std::ifstream indexFile(indexPath);
            if (indexFile.is_open()) {
                indexFile >> indexJson;
                indexFile.close();

                if (!indexJson.contains("name"))
                    indexJson["name"] = "Custom World";
                if (!indexJson.contains("levels") || !indexJson["levels"].is_array())
                    indexJson["levels"] = json::array();
            } else {
                indexJson = {{"name", "Custom World"}, {"levels", json::array()}};
            }

            std::string levelId = levelPath;
            size_t lastSlash = levelId.find_last_of('/');
            if (lastSlash != std::string::npos)
                levelId = levelId.substr(lastSlash + 1);
            size_t dotPos = levelId.find_last_of('.');
            if (dotPos != std::string::npos)
                levelId = levelId.substr(0, dotPos);

            bool levelExists = false;
            for (auto &level : indexJson["levels"]) {
                if (level["id"] == levelId) {
                    level["name"] = levelName;
                    level["path"] = levelPath;
                    levelExists = true;
                    break;
                }
            }

            if (!levelExists) {
                json newLevel = {{"id", levelId}, {"name", levelName}, {"path", levelPath}};
                indexJson["levels"].push_back(newLevel);
            }

            std::ofstream outIndexFile(indexPath);
            if (outIndexFile.is_open()) {
                outIndexFile << indexJson.dump(2);
                outIndexFile.close();
            }
        } catch (const std::exception &e) {
            std::cerr << "Warning: Failed to update custom levels index: " << e.what() << std::endl;
        }
    }

    bool LevelEditorState::saveLevel()
    {
        try {
            std::string filename = generateLevelFilename();
            json j = createLevelJson();

            populateEnemiesJson(j["enemies"]);
            populateObstaclesJson(j["obstacles"]);
            j["waves"] = createWavesJson();

            if (!writeJsonToFile(j, filename))
                return false;

            updateCustomLevelsIndex(filename, _levelName);
            return true;
        } catch (const std::exception &) {
            return false;
        }
    }
} // namespace Engine
