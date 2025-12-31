/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelManager
*/

#include "LevelManager.hpp"

using json = nlohmann::json;

namespace
{
    [[nodiscard]] Game::ShootDefinition parseShootDefinition(const json &j)
    {
        Game::ShootDefinition shootDef;
        shootDef.type = j.value("type", "straight");
        shootDef.cooldown = j.value("cooldown", 1.f);
        shootDef.damage = j.value("damage", 10);
        shootDef.projectileSpeed = j.value("projectileSpeed", 200.f);
        if (j.contains("angles") && j.at("angles").is_array()) {
            for (const auto &angleValue : j.at("angles"))
                shootDef.angles.push_back(angleValue.get<float>());
        }
        if (j.contains("muzzle") && j.at("muzzle").is_object()) {
            const auto &muzzleNode = j.at("muzzle");
            shootDef.muzzle.first = muzzleNode.value("x", 0.f);
            shootDef.muzzle.second = muzzleNode.value("y", 0.f);
        } else {
            shootDef.muzzle = {0.f, 0.f};
        }
        return shootDef;
    }

    [[nodiscard]] bool parseEnemies(const json &j, Game::Level &level)
    {
        level.enemyTypes.clear();
        if (!j.contains("enemies") || !j.at("enemies").is_object())
            return false;
        if (j.at("enemies").empty())
            return false;

        for (auto &[name, defNode] : j.at("enemies").items()) {
            if (!defNode.is_object())
                return false;

            Game::EnemyDefinition def;
            def.hp = defNode.value("hp", 1);
            def.speed = defNode.value("speed", -80.f);

            if (!defNode.contains("size") || !defNode.at("size").is_object())
                return false;

            def.colW = defNode.at("size").value("w", 20.f);
            def.colH = defNode.at("size").value("h", 20.f);
            def.sprite = defNode.value("spriteId", static_cast<unsigned int>(0));
            def.killScore = defNode.value("killScore", 10u);
            def.shoot = parseShootDefinition(defNode.value("shoot", json::object()));

            level.enemyTypes[name] = def;
        }
        return !level.enemyTypes.empty();
    }

    bool parseWaves(const json &j, Game::Level &level)
    {
        level.waves.clear();

        if (!j.contains("waves") || !j.at("waves").is_array())
            return false;
        if (j.at("waves").empty())
            return false;
        for (const auto &w : j.at("waves")) {
            if (!w.is_object())
                return false;
            Game::Wave wave;
            wave.time = w.value("time", -1.f);
            if (wave.time < 0.f)
                return false;
            if (!w.contains("enemies") || !w.at("enemies").is_object())
                return false;
            for (auto &[type, countValue] : w.at("enemies").items()) {
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

    bool parseLevelJson(const json &j, Game::Level &level)
    {
        if (!j.contains("name") || !j.at("name").is_string())
            return false;

        level.name = j.at("name").get<std::string>();
        level.duration = j.value("duration", 0.f);

        if (!parseEnemies(j, level))
            return false;
        if (!parseWaves(j, level))
            return false;
        return true;
    }
} // namespace

namespace Game
{

    bool LevelManager::load(const std::string &jsonContent)
    {
        json j;
        try {
            j = json::parse(jsonContent);
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

    void LevelManager::advance(const float dt)
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
