/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelManager
*/

#include "LevelManager.hpp"

using json = nlohmann::json;

namespace Game
{
    bool parseEnemies(const json &j, Level &level)
    {
        level.enemyTypes.clear();
        if (!j.contains("ennemies") || !j["ennemies"].is_object())
            return false;
        if (j["ennemies"].empty())
            return false;

        for (auto &[name, defNode] : j["ennemies"].items()) {
            if (!defNode.is_object())
                return false;

            EnemyDefinition def;
            def.hp = defNode.value("hp", 1);
            def.speed = defNode.value("speed", -80.f);

            if (!defNode.contains("size") || !defNode["size"].is_object())
                return false;

            def.colW = defNode["size"].value("w", 20.f);
            def.colH = defNode["size"].value("h", 20.f);
            def.sprite = defNode.value("sprite", "");
            level.enemyTypes[name] = def;
        }
        return !level.enemyTypes.empty();
    }

    bool parseWaves(const json &j, Level &level)
    {
        level.waves.clear();

        if (!j.contains("waves") || !j["waves"].is_array())
            return false;
        if (j["waves"].empty())
            return false;
        for (const auto &w : j["waves"]) {
            if (!w.is_object())
                return false;
            Wave wave;
            wave.time = w.value("time", -1.f);
            if (wave.time < 0.f)
                return false;
            if (!w.contains("enemies") || !w["enemies"].is_object())
                return false;
            for (auto &[type, countValue] : w["enemies"].items()) {
                const int count = countValue.get<int>();
                if (count <= 0)
                    return false;
                wave.groups.push_back({type, count});
            }
            if (wave.groups.empty())
                return false;
            level.waves.push_back(wave);
        }
        return true;
    }

    bool parseLevelJson(const json &j, Level &level)
    {
        if (!j.contains("name") || !j["name"].is_string())
            return false;

        level.name = j["name"].get<std::string>();
        level.duration = j.value("duration", 0.f);

        if (!parseEnemies(j, level))
            return false;
        if (!parseWaves(j, level))
            return false;
        return true;
    }

    bool LevelManager::load(const std::string &content)
    {
        json j;
        try {
            j = json::parse(content);
        } catch (...) {
            return false;
        }
        if (!parseLevelJson(j, _level))
            return false;
        _time = 0.f;
        return true;
    }

    bool LevelManager::loadFromFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
            return false;

        std::stringstream ss;
        ss << file.rdbuf();
        return load(ss.str());
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
