/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** SettingsConfig
*/

#include "SettingsConfig.hpp"

using json = nlohmann::json;

namespace {
Engine::Key stringToKey(const std::string &str) {
    if (str == "Up") return Engine::Key::Up;
    if (str == "Down") return Engine::Key::Down;
    if (str == "Left") return Engine::Key::Left;
    if (str == "Right") return Engine::Key::Right;
    if (str == "Z") return Engine::Key::Z;
    if (str == "S") return Engine::Key::S;
    if (str == "Q") return Engine::Key::Q;
    if (str == "D") return Engine::Key::D;
    return Engine::Key::Up;
}

std::string keyToString(Engine::Key key) {
    switch (key) {
        case Engine::Key::Up: return "Up";
        case Engine::Key::Down: return "Down";
        case Engine::Key::Left: return "Left";
        case Engine::Key::Right: return "Right";
        case Engine::Key::Z: return "Z";
        case Engine::Key::S: return "S";
        case Engine::Key::Q: return "Q";
        case Engine::Key::D: return "D";
        default: return "Up";
    }
}
}

namespace Utils
{
    SettingsConfig::SettingsConfig()
    {
        _presets[KeyPreset::Arrows] = {Engine::Key::Up, Engine::Key::Down, Engine::Key::Left, Engine::Key::Right};
        _presets[KeyPreset::ZQSD] = {Engine::Key::Z, Engine::Key::S, Engine::Key::Q, Engine::Key::D};
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
                _musicVolume = audio.value("music_volume", 50);
                _sfxVolume = audio.value("sfx_volume", 50);
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

            j["audio"] = {
                {"music_volume", _musicVolume},
                {"sfx_volume", _sfxVolume},
                {"music_muted", _musicMuted},
                {"sfx_muted", _sfxMuted}
            };

            j["video"] = {
                {"resolution", {
                    {"width", _resolution.width},
                    {"height", _resolution.height}
                }},
                {"colorblind_mode", 
                    _colorBlindMode == Graphics::ColorBlindMode::DEUTERANOPIA ? "deuteranopia" :
                    _colorBlindMode == Graphics::ColorBlindMode::PROTANOPIA ? "protanopia" :
                    _colorBlindMode == Graphics::ColorBlindMode::TRITANOPIA ? "tritanopia" : "none"
                }
            };

            std::string presetStr;
            switch (_currentPreset) {
                case KeyPreset::Arrows: presetStr = "arrows"; break;
                case KeyPreset::ZQSD: presetStr = "zqsd"; break;
                case KeyPreset::Custom: presetStr = "custom"; break;
                default: presetStr = "arrows"; break;
            }
            j["input"] = {{"preset", presetStr}};
            if (_currentPreset == KeyPreset::Custom) {
                j["input"]["keys"] = {
                    {"up", keyToString(_customKeys.up)},
                    {"down", keyToString(_customKeys.down)},
                    {"left", keyToString(_customKeys.left)},
                    {"right", keyToString(_customKeys.right)}
                };
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

    void SettingsConfig::clearRebindFlag() const noexcept
    {
        _needsRebind = false;
    }

    void SettingsConfig::setMusicVolume(std::size_t volume)
    {
        _musicVolume = volume;
        save();
    }

    void SettingsConfig::setSfxVolume(std::size_t volume)
    {
        _sfxVolume = volume;
        save();
    }

    void SettingsConfig::setMusicMuted(bool muted)
    {
        _musicMuted = muted;
        save();
    }

    void SettingsConfig::setSfxMuted(bool muted)
    {
        _sfxMuted = muted;
        save();
    }

    void SettingsConfig::setResolution(Graphics::Extent2u resolution)
    {
        _resolution = resolution;
        save();
    }

    void SettingsConfig::setColorBlindMode(Graphics::ColorBlindMode mode)
    {
        _colorBlindMode = mode;
        save();
    }

    void SettingsConfig::setPreset(KeyPreset preset)
    {
        _currentPreset = preset;
        _needsRebind = true;
        save();
    }

    void SettingsConfig::setCustomMovementKeys(MovementKeys keys)
    {
        _customKeys = keys;
        _currentPreset = KeyPreset::Custom;
        _needsRebind = true;
        save();
    }
} // namespace Utils

