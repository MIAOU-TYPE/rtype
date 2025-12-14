/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelManager
*/

#include "LevelManager.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Game
{
    bool LevelManager::loadFromMemory(const std::string &content)
    {
        json j;
        try {
            j = json::parse(content);
        } catch (...) {
            return false;
        }

        _level.name = j.value("name", "Unknown");
        _level.duration = j.value("duration", 0.0f);

        _level.enemyTypes.clear();
        if (j.contains("ennemies")) {
            for (auto &[name, defNode] : j["ennemies"].items()) {
                EnemyDefinition def;
                def.hp = defNode.value("hp", 1);
                def.speed = defNode.value("speed", -80.f);
                def.colW = defNode["size"].value("w", 20.f);
                def.colH = defNode["size"].value("h", 20.f);
                def.sprite = defNode.value("sprite", "");
                _level.enemyTypes[name] = def;
            }
        }

        _level.waves.clear();
        if (j.contains("waves")) {
            for (auto &w : j["waves"]) {
                Wave wave;
                wave.time = w.value("time", 0.f);
                for (auto &[type, count] : w["enemies"].items()) {
                    WaveEnemyGroup g;
                    g.type = type;
                    g.count = count;
                    wave.groups.push_back(g);
                }
                _level.waves.push_back(wave);
            }
        }

        _time = 0;
        return true;
    }

    bool LevelManager::loadFromFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
            return false;
        json j;
        file >> j;

        _level.name = j.value("name", "Unknown");
        _level.duration = j.value("duration", 0.0f);
        _level.enemyTypes.clear();
        if (j.contains("ennemies") && j["ennemies"].is_object()) {
            for (auto &[name, defNode] : j["ennemies"].items()) {
                EnemyDefinition def;
                def.hp = defNode.value("hp", 1);
                def.speed = defNode.value("speed", -80.f);

                auto sizeNode = defNode["size"];
                def.colW = sizeNode.value("w", 20.f);
                def.colH = sizeNode.value("h", 20.f);
                def.sprite = defNode.value("sprite", "");
                _level.enemyTypes[name] = def;
            }
        }

        _level.waves.clear();
        if (j.contains("waves") && j["waves"].is_array()) {
            for (auto &w : j["waves"]) {
                Wave wave;
                wave.time = w.value("time", 0.f);
                if (w.contains("enemies")) {
                    for (auto &[type, countValue] : w["enemies"].items()) {
                        WaveEnemyGroup group;
                        group.type = type;
                        group.count = countValue.get<int>();
                        wave.groups.push_back(group);
                    }
                }
                _level.waves.push_back(wave);
            }
        }
        _time = 0.f;
        return true;
    }

    void LevelManager::reset()
    {
        _time = 0.f;
    }

    void LevelManager::advance(float dt)
    {
        _time += dt;
    }

    const Level &LevelManager::getCurrentLevel() const
    {
        return _level;
    }

    float LevelManager::getTime() const
    {
        return _time;
    }

    bool LevelManager::shouldSpawn(const float waveTime) const
    {
        return _time >= waveTime;
    }
} // namespace Game
