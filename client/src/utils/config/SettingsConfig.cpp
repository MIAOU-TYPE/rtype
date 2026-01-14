/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** SettingsConfig
*/

#include "SettingsConfig.hpp"

using json = nlohmann::json;

namespace
{
    Engine::Key stringToKey(const std::string &str)
    {
        static const std::unordered_map<std::string, Engine::Key> keyMap = {{"A", Engine::Key::A},
            {"B", Engine::Key::B}, {"C", Engine::Key::C}, {"D", Engine::Key::D}, {"E", Engine::Key::E},
            {"F", Engine::Key::F}, {"G", Engine::Key::G}, {"H", Engine::Key::H}, {"I", Engine::Key::I},
            {"J", Engine::Key::J}, {"K", Engine::Key::K}, {"L", Engine::Key::L}, {"M", Engine::Key::M},
            {"N", Engine::Key::N}, {"O", Engine::Key::O}, {"P", Engine::Key::P}, {"Q", Engine::Key::Q},
            {"R", Engine::Key::R}, {"S", Engine::Key::S}, {"T", Engine::Key::T}, {"U", Engine::Key::U},
            {"V", Engine::Key::V}, {"W", Engine::Key::W}, {"X", Engine::Key::X}, {"Y", Engine::Key::Y},
            {"Z", Engine::Key::Z}, {"Num0", Engine::Key::Num0}, {"Num1", Engine::Key::Num1},
            {"Num2", Engine::Key::Num2}, {"Num3", Engine::Key::Num3}, {"Num4", Engine::Key::Num4},
            {"Num5", Engine::Key::Num5}, {"Num6", Engine::Key::Num6}, {"Num7", Engine::Key::Num7},
            {"Num8", Engine::Key::Num8}, {"Num9", Engine::Key::Num9}, {"Up", Engine::Key::Up},
            {"Down", Engine::Key::Down}, {"Left", Engine::Key::Left}, {"Right", Engine::Key::Right},
            {"Space", Engine::Key::Space}, {"Enter", Engine::Key::Enter}, {"Escape", Engine::Key::Escape},
            {"Tab", Engine::Key::Tab}, {"Backspace", Engine::Key::Backspace}, {"LShift", Engine::Key::LShift},
            {"RShift", Engine::Key::RShift}, {"LCtrl", Engine::Key::LCtrl}, {"RCtrl", Engine::Key::RCtrl}};
        auto it = keyMap.find(str);
        if (it == keyMap.end()) {
            std::cerr << "Warning: Unrecognized key string '" << str << "', defaulting to Up" << std::endl;
            return Engine::Key::Up;
        }
        return it->second;
    }

    std::string keyToString(Engine::Key key)
    {
        switch (key) {
            case Engine::Key::A: return "A";
            case Engine::Key::B: return "B";
            case Engine::Key::C: return "C";
            case Engine::Key::D: return "D";
            case Engine::Key::E: return "E";
            case Engine::Key::F: return "F";
            case Engine::Key::G: return "G";
            case Engine::Key::H: return "H";
            case Engine::Key::I: return "I";
            case Engine::Key::J: return "J";
            case Engine::Key::K: return "K";
            case Engine::Key::L: return "L";
            case Engine::Key::M: return "M";
            case Engine::Key::N: return "N";
            case Engine::Key::O: return "O";
            case Engine::Key::P: return "P";
            case Engine::Key::Q: return "Q";
            case Engine::Key::R: return "R";
            case Engine::Key::S: return "S";
            case Engine::Key::T: return "T";
            case Engine::Key::U: return "U";
            case Engine::Key::V: return "V";
            case Engine::Key::W: return "W";
            case Engine::Key::X: return "X";
            case Engine::Key::Y: return "Y";
            case Engine::Key::Z: return "Z";

            case Engine::Key::Num0: return "Num0";
            case Engine::Key::Num1: return "Num1";
            case Engine::Key::Num2: return "Num2";
            case Engine::Key::Num3: return "Num3";
            case Engine::Key::Num4: return "Num4";
            case Engine::Key::Num5: return "Num5";
            case Engine::Key::Num6: return "Num6";
            case Engine::Key::Num7: return "Num7";
            case Engine::Key::Num8: return "Num8";
            case Engine::Key::Num9: return "Num9";

            case Engine::Key::Up: return "Up";
            case Engine::Key::Down: return "Down";
            case Engine::Key::Left: return "Left";
            case Engine::Key::Right: return "Right";

            case Engine::Key::Space: return "Space";
            case Engine::Key::Enter: return "Enter";
            case Engine::Key::Escape: return "Escape";
            case Engine::Key::Tab: return "Tab";
            case Engine::Key::Backspace: return "Backspace";
            case Engine::Key::LShift: return "LShift";
            case Engine::Key::RShift: return "RShift";
            case Engine::Key::LCtrl: return "LCtrl";
            case Engine::Key::RCtrl: return "RCtrl";

            default: return "Unknown";
        }
    }
} // namespace

namespace Utils
{
    SettingsConfig::SettingsConfig()
    {
        _presets[KeyPreset::Arrows] = {
            Engine::Key::Up, Engine::Key::Down, Engine::Key::Left, Engine::Key::Right, Engine::Key::Space};
        _presets[KeyPreset::ZQSD] = {
            Engine::Key::Z, Engine::Key::S, Engine::Key::Q, Engine::Key::D, Engine::Key::Space};
        _customKeys = _presets[KeyPreset::Arrows];

        if (!load())
            std::cerr << "Warning: Failed to load settings configuration, using defaults" << std::endl;
    }

    SettingsConfig &SettingsConfig::getInstance()
    {
        static SettingsConfig instance;
        return instance;
    }

    bool SettingsConfig::load() noexcept
    {
        try {
            if (!std::filesystem::exists(CONFIG_FILE))
                return false;

            std::ifstream file(CONFIG_FILE);
            if (!file.is_open())
                return false;

            json j;
            file >> j;

            if (j.contains("audio") && j["audio"].is_object()) {
                const auto &audio = j["audio"];
                _musicVolume = audio.value("music_volume", 50U);
                _sfxVolume = audio.value("sfx_volume", 50U);
                if (_musicVolume > 100U)
                    _musicVolume = 100U;
                if (_sfxVolume > 100U)
                    _sfxVolume = 100U;
                _musicMuted = audio.value("music_muted", false);
                _sfxMuted = audio.value("sfx_muted", false);
            }

            if (j.contains("video") && j["video"].is_object()) {
                const auto &video = j["video"];
                if (video.contains("resolution") && video["resolution"].is_object()) {
                    const auto &res = video["resolution"];
                    _resolution.width = res.value("width", 1280u);
                    _resolution.height = res.value("height", 720u);
                }
                std::string colorBlindStr = video.value("colorblind_mode", "none");
                if (colorBlindStr == "deuteranopia")
                    _colorBlindMode = Graphics::ColorBlindMode::DEUTERANOPIA;
                else if (colorBlindStr == "protanopia")
                    _colorBlindMode = Graphics::ColorBlindMode::PROTANOPIA;
                else if (colorBlindStr == "tritanopia")
                    _colorBlindMode = Graphics::ColorBlindMode::TRITANOPIA;
                else
                    _colorBlindMode = Graphics::ColorBlindMode::NONE;
            }

            if (j.contains("input") && j["input"].is_object()) {
                const auto &input = j["input"];
                std::string presetStr = input.value("preset", "arrows");
                if (presetStr == "arrows")
                    _currentPreset = KeyPreset::Arrows;
                else if (presetStr == "zqsd")
                    _currentPreset = KeyPreset::ZQSD;
                else if (presetStr == "custom")
                    _currentPreset = KeyPreset::Custom;
                else
                    _currentPreset = KeyPreset::Arrows;

                if (_currentPreset == KeyPreset::Custom && input.contains("keys") && input["keys"].is_object()) {
                    const auto &keys = input["keys"];
                    _customKeys.up = stringToKey(keys.value("up", "Up"));
                    _customKeys.down = stringToKey(keys.value("down", "Down"));
                    _customKeys.left = stringToKey(keys.value("left", "Left"));
                    _customKeys.right = stringToKey(keys.value("right", "Right"));
                    _customKeys.shoot = stringToKey(keys.value("shoot", "Space"));
                }
            }
            return true;
        } catch (const std::exception &e) {
            std::cerr << "Error loading settings: " << e.what() << std::endl;
            return false;
        }
    }

    bool SettingsConfig::save() const noexcept
    {
        try {
            std::filesystem::path configPath(CONFIG_FILE);
            std::filesystem::create_directories(configPath.parent_path());

            json j;

            j["audio"] = {{"music_volume", _musicVolume}, {"sfx_volume", _sfxVolume}, {"music_muted", _musicMuted},
                {"sfx_muted", _sfxMuted}};

            j["video"] = {{"resolution", {{"width", _resolution.width}, {"height", _resolution.height}}},
                {"colorblind_mode",
                    _colorBlindMode == Graphics::ColorBlindMode::DEUTERANOPIA     ? "deuteranopia"
                        : _colorBlindMode == Graphics::ColorBlindMode::PROTANOPIA ? "protanopia"
                        : _colorBlindMode == Graphics::ColorBlindMode::TRITANOPIA ? "tritanopia"
                                                                                  : "none"}};

            std::string presetStr;
            switch (_currentPreset) {
                case KeyPreset::Arrows: presetStr = "arrows"; break;
                case KeyPreset::ZQSD: presetStr = "zqsd"; break;
                case KeyPreset::Custom: presetStr = "custom"; break;
                default: presetStr = "arrows"; break;
            }
            j["input"] = {{"preset", presetStr}};
            if (_currentPreset == KeyPreset::Custom) {
                j["input"]["keys"] = {{"up", keyToString(_customKeys.up)}, {"down", keyToString(_customKeys.down)},
                    {"left", keyToString(_customKeys.left)}, {"right", keyToString(_customKeys.right)},
                    {"shoot", keyToString(_customKeys.shoot)}};
            }

            std::ofstream file(CONFIG_FILE);
            if (!file.is_open())
                return false;

            file << j.dump(4);
            return true;
        } catch (const std::exception &e) {
            std::cerr << "Error saving settings: " << e.what() << std::endl;
            return false;
        }
    }

    std::size_t SettingsConfig::getMusicVolume() const noexcept
    {
        return _musicVolume;
    }

    std::size_t SettingsConfig::getSfxVolume() const noexcept
    {
        return _sfxVolume;
    }

    bool SettingsConfig::isMusicMuted() const noexcept
    {
        return _musicMuted;
    }

    bool SettingsConfig::isSfxMuted() const noexcept
    {
        return _sfxMuted;
    }

    Graphics::Extent2u SettingsConfig::getResolution() const noexcept
    {
        return _resolution;
    }

    Graphics::ColorBlindMode SettingsConfig::getColorBlindMode() const noexcept
    {
        return _colorBlindMode;
    }

    KeyPreset SettingsConfig::getCurrentPreset() const noexcept
    {
        return _currentPreset;
    }

    MovementKeys SettingsConfig::getMovementKeys() const noexcept
    {
        if (_currentPreset == KeyPreset::Custom)
            return _customKeys;

        if (const auto it = _presets.find(_currentPreset); it != _presets.end())
            return it->second;
        return _presets.at(KeyPreset::Arrows);
    }

    std::string SettingsConfig::getPresetName(KeyPreset preset)
    {
        switch (preset) {
            case KeyPreset::Arrows: return "Arrow Keys";
            case KeyPreset::ZQSD: return "ZQSD";
            case KeyPreset::Custom: return "Custom";
            default: return "Unknown";
        }
    }

    bool SettingsConfig::needsRebind() const noexcept
    {
        return _needsRebind;
    }

    void SettingsConfig::clearRebindFlag() noexcept
    {
        _needsRebind = false;
    }

    bool SettingsConfig::isKeyAlreadyAssigned(Engine::Key key, Engine::Key excludeCommand) const noexcept
    {
        const MovementKeys keys = getMovementKeys();
        return (key == keys.up && excludeCommand != keys.up) || (key == keys.down && excludeCommand != keys.down)
            || (key == keys.left && excludeCommand != keys.left) || (key == keys.right && excludeCommand != keys.right)
            || (key == keys.shoot && excludeCommand != keys.shoot);
    }

    void SettingsConfig::setMusicVolume(std::size_t volume)
    {
        _musicVolume = (volume > 100U) ? 100U : volume;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setSfxVolume(std::size_t volume)
    {
        _sfxVolume = (volume > 100U) ? 100U : volume;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setMusicMuted(bool muted)
    {
        _musicMuted = muted;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setSfxMuted(bool muted)
    {
        _sfxMuted = muted;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setResolution(Graphics::Extent2u resolution)
    {
        _resolution = resolution;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setColorBlindMode(Graphics::ColorBlindMode mode)
    {
        _colorBlindMode = mode;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setPreset(KeyPreset preset)
    {
        _currentPreset = preset;
        _needsRebind = true;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setCustomMovementKeys(MovementKeys keys)
    {
        _customKeys = keys;
        _currentPreset = KeyPreset::Custom;
        _needsRebind = true;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    std::string SettingsConfig::keyToString(Engine::Key key)
    {
        return ::keyToString(key);
    }

    Engine::Key SettingsConfig::getUpKey() const noexcept
    {
        return getMovementKeys().up;
    }

    Engine::Key SettingsConfig::getDownKey() const noexcept
    {
        return getMovementKeys().down;
    }

    Engine::Key SettingsConfig::getLeftKey() const noexcept
    {
        return getMovementKeys().left;
    }

    Engine::Key SettingsConfig::getRightKey() const noexcept
    {
        return getMovementKeys().right;
    }

    Engine::Key SettingsConfig::getShootKey() const noexcept
    {
        return getMovementKeys().shoot;
    }

    void SettingsConfig::setUpKey(Engine::Key key)
    {
        _customKeys.up = key;
        _currentPreset = KeyPreset::Custom;
        _needsRebind = true;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setDownKey(Engine::Key key)
    {
        _customKeys.down = key;
        _currentPreset = KeyPreset::Custom;
        _needsRebind = true;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setLeftKey(Engine::Key key)
    {
        _customKeys.left = key;
        _currentPreset = KeyPreset::Custom;
        _needsRebind = true;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setRightKey(Engine::Key key)
    {
        _customKeys.right = key;
        _currentPreset = KeyPreset::Custom;
        _needsRebind = true;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }

    void SettingsConfig::setShootKey(Engine::Key key)
    {
        _customKeys.shoot = key;
        _currentPreset = KeyPreset::Custom;
        _needsRebind = true;
        if (!save())
            std::cerr << "Warning: Failed to save settings" << std::endl;
    }
} // namespace Utils
