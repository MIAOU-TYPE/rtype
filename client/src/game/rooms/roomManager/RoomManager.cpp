/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomManager
*/

#include "RoomManager.hpp"

namespace Engine
{
    RoomManager::RoomManager(std::shared_ptr<Resources::IResourceManager> resources) : _resources(std::move(resources))
    {
        if (!_resources)
            throw RoomManagerError("{RoomManager} resource manager is null");

        loadFromEmbedded();

        if (_worlds.empty())
            throw RoomManagerError("{RoomManager} no worlds/levels found in embedded resources");
    }

    const std::vector<WorldEntry> &RoomManager::worlds() const noexcept
    {
        return _worlds;
    }

    const std::vector<LevelInfo> &RoomManager::levelsFor(std::string_view worldId) const
    {
        const auto it = _levelsByWorldId.find(std::string(worldId));
        if (it == _levelsByWorldId.end())
            throw RoomManagerError("{RoomManager::levelsFor} unknown worldId");
        const WorldLevels &wl = it->second;
        return wl.levels;
    }

    std::vector<RoomData> &RoomManager::rooms() noexcept
    {
        return _rooms;
    }

    std::string RoomManager::makePath(const std::string_view worldId, const std::string_view fileName)
    {
        std::string p;
        p.reserve(std::string_view("levels/").size() + worldId.size() + 1 + fileName.size());
        p.append("levels/");
        p.append(worldId);
        p.push_back('/');
        p.append(fileName);
        return p;
    }

    std::vector<std::string> RoomManager::listEmbeddedWorldIds() const
    {
        std::unordered_set<std::string> seen;
        std::vector<std::string> out;

        for (const auto &k : _resources->listResources()) {
            constexpr std::string_view prefix = "levels/";
            if (k.size() < prefix.size() || !k.starts_with(prefix))
                continue;
            const auto rest = k.substr(prefix.size());
            const auto slash = rest.find('/');
            if (slash == std::string_view::npos)
                continue;
            std::string worldId(rest.substr(0, slash));
            if (worldId.empty())
                continue;
            if (seen.insert(worldId).second)
                out.push_back(std::move(worldId));
        }
        std::ranges::sort(out, [](const std::string &a, const std::string &b) {
            auto getPriority = [](const std::string &s) -> int {
                if (s == "world1") return 1;
                if (s == "world2") return 2;
                if (s == "world3") return 3;
                return 4;
            };
            return getPriority(a) < getPriority(b);
        });
        return out;
    }

    std::optional<std::string> RoomManager::readTextAsset(const std::string_view assetPath) const
    {
        const std::string key(assetPath);
        if (!_resources->hasResource(key))
            return std::nullopt;

        const auto [data, size] = _resources->loadResource(key);
        if (!data || size == 0)
            return std::nullopt;

        return std::string(reinterpret_cast<const char *>(data), size);
    }

    std::vector<LevelInfo> RoomManager::parseLevelsListJson(const std::string_view jsonText)
    {
        json j;
        try {
            j = json::parse(jsonText.begin(), jsonText.end());
        } catch (const std::exception &e) {
            throw RoomManagerError(std::string("{RoomManager::parseLevelsListJson} parse error: ") + e.what());
        }

        if (!j.is_array())
            throw RoomManagerError("{RoomManager::parseLevelsListJson} JSON is not an array");

        std::vector<LevelInfo> out;
        out.reserve(j.size());

        for (const auto &item : j) {
            if (!item.is_object())
                continue;

            LevelInfo lvl;
            lvl.id = item.value("id", "");
            lvl.displayName = item.value("name", "");
            lvl.path = item.value("path", "");

            if (!lvl.id.empty() && !lvl.displayName.empty())
                out.push_back(std::move(lvl));
        }

        return out;
    }

    std::optional<std::string> RoomManager::parseWorldNameJson(std::string_view jsonText)
    {
        json j;
        try {
            j = json::parse(jsonText.begin(), jsonText.end());
        } catch (...) {
            return std::nullopt;
        }

        if (!j.is_object())
            return std::nullopt;

        const std::string name = j.value("name", "");
        if (name.empty())
            return std::nullopt;

        return name;
    }

    void RoomManager::loadFromEmbedded()
    {
        _worlds.clear();
        _levelsByWorldId.clear();

        for (const auto &worldId : listEmbeddedWorldIds()) {
            const auto content = readTextAsset(makePath(worldId, "levels.json"));
            if (!content)
                continue;

            auto parsed = parseLevelsListJson(*content);
            if (parsed.empty())
                continue;

            WorldLevels wl;
            wl.levels = std::move(parsed);

            std::string displayName = worldId;
            if (auto wj = readTextAsset(makePath(worldId, "world.json"))) {
                if (auto name = parseWorldNameJson(*wj))
                    displayName = *name;
            }

            _worlds.push_back(WorldEntry{worldId, displayName});
            _levelsByWorldId.emplace(worldId, std::move(wl));
        }
    }
} // namespace Engine
