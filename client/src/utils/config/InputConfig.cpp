/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** InputConfig
*/

#include "InputConfig.hpp"
#include <fstream>
#include <iostream>

namespace Utils
{
    InputConfig::InputConfig()
    {
        _presets[KeyPreset::Arrows] = {Engine::Key::Up, Engine::Key::Down, Engine::Key::Left, Engine::Key::Right};
        _presets[KeyPreset::ZQSD] = {Engine::Key::Z, Engine::Key::S, Engine::Key::Q, Engine::Key::D};

        load();
    }

    InputConfig &InputConfig::getInstance()
    {
        static InputConfig instance;
        return instance;
    }

    KeyPreset InputConfig::getCurrentPreset() const noexcept
    {
        return _currentPreset;
    }

    void InputConfig::setPreset(const KeyPreset preset)
    {
        _currentPreset = preset;
        _needsRebind = true;
        save();
    }

    MovementKeys InputConfig::getMovementKeys() const noexcept
    {
        const auto it = _presets.find(_currentPreset);
        if (it != _presets.end())
            return it->second;
        return _presets.at(KeyPreset::Arrows);
    }

    std::string InputConfig::getPresetName(const KeyPreset preset)
    {
        switch (preset) {
            case KeyPreset::Arrows: return "Arrow Keys";
            case KeyPreset::ZQSD: return "ZQSD";
            default: return "Unknown";
        }
    }

    bool InputConfig::load()
    {
        std::ifstream file(CONFIG_FILE);
        if (!file.is_open())
            return false;

        std::string line;
        while (std::getline(file, line)) {
            if (line.find("preset=") == 0) {
                const std::string value = line.substr(7);
                if (value == "arrows")
                    _currentPreset = KeyPreset::Arrows;
                else if (value == "zqsd")
                    _currentPreset = KeyPreset::ZQSD;
            }
        }
        return true;
    }

    bool InputConfig::save() const
    {
        std::ofstream file(CONFIG_FILE);
        if (!file.is_open())
            return false;

        std::string presetName;
        switch (_currentPreset) {
            case KeyPreset::Arrows: presetName = "arrows"; break;
            case KeyPreset::ZQSD: presetName = "zqsd"; break;
            default: presetName = "arrows"; break;
        }

        file << "preset=" << presetName << "\n";
        return true;
    }

    bool InputConfig::needsRebind() const noexcept
    {
        return _needsRebind;
    }

    void InputConfig::clearRebindFlag() noexcept
    {
        _needsRebind = false;
    }
} // namespace Utils
