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

    const std::vector<LevelInfo> &RoomManager::levelsFor(const std::string_view worldId) const
    {
        const auto it = _levelsByWorldId.find(std::string(worldId));
        if (it == _levelsByWorldId.end())
            throw RoomManagerError("{RoomManager::levelsFor} unknown worldId");
        const auto &[levels] = it->second;
        return levels;
    }

    std::vector<RoomData> &RoomManager::rooms() noexcept
    {
        return _rooms;
    }

    void RoomManager::setCurrentData(const RoomData &roomData) noexcept
    {
        _currentRoom = roomData;
    }

    const RoomData &RoomManager::currentRoomData() const noexcept
    {
        return _currentRoom;
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
                if (s == "world1")
                    return 1;
                if (s == "world2")
                    return 2;
                if (s == "world3")
                    return 3;
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

    std::tuple<std::string, std::string, std::vector<LevelInfo>> RoomManager::parseWorldLevelsJson(const std::string_view jsonText)
    {
        json j;
        try {
            j = json::parse(jsonText.begin(), jsonText.end());
        } catch (const std::exception &e) {
            throw RoomManagerError(std::string("{RoomManager::parseWorldLevelsJson} parse error: ") + e.what());
        }

        if (!j.is_object())
            throw RoomManagerError("{RoomManager::parseWorldLevelsJson} JSON is not an object");

        const std::string name = j.value("name", "");
        if (name.empty())
            throw RoomManagerError("{RoomManager::parseWorldLevelsJson} Missing or empty 'name' field");

        const std::string music = j.value("music", ::DEFAULT_GAME_MUSIC);

        const auto levelsJson = j.find("levels");
        if (levelsJson == j.end() || !levelsJson->is_array())
            throw RoomManagerError("{RoomManager::parseWorldLevelsJson} Missing or invalid 'levels' array");

        std::vector<LevelInfo> levels;
        levels.reserve(levelsJson->size());

        for (const auto &item : *levelsJson) {
            if (!item.is_object())
                continue;

            LevelInfo lvl;
            lvl.id = item.value("id", "");
            lvl.displayName = item.value("name", "");
            lvl.path = item.value("path", "");

            if (!lvl.id.empty() && !lvl.displayName.empty())
                levels.push_back(std::move(lvl));
        }

        return {name, music, levels};
    }

    void RoomManager::loadFromEmbedded()
    {
        _worlds.clear();
        _levelsByWorldId.clear();

        for (const auto &worldId : listEmbeddedWorldIds()) {
            if (worldId == "custom")
                continue;

            const auto content = readTextAsset(makePath(worldId, "levels.json"));
            if (!content)
                continue;

            auto [displayName, musicPath, parsed] = parseWorldLevelsJson(*content);
            if (parsed.empty())
                continue;

            WorldLevels wl;
            wl.levels = std::move(parsed);

            _worlds.push_back(WorldEntry{worldId, displayName, musicPath});
            _levelsByWorldId.emplace(worldId, std::move(wl));
        }

        loadCustomWorldFromFilesystem();
    }

    void RoomManager::loadCustomWorldFromFilesystem()
    {
        _worlds.erase(std::remove_if(_worlds.begin(), _worlds.end(),
                          [](const WorldEntry &w) {
                              return w.id == "custom";
                          }),
            _worlds.end());
        _levelsByWorldId.erase("custom");

        std::vector<LevelInfo> customLevels;

        try {
            const std::filesystem::path levelsDir("levels");
            if (!std::filesystem::exists(levelsDir) || !std::filesystem::is_directory(levelsDir)) {
                return;
            }

            for (const auto &entry : std::filesystem::directory_iterator(levelsDir)) {
                if (!entry.is_regular_file() || entry.path().extension() != ".json") {
                    continue;
                }

                const std::string filename = entry.path().filename().string();
                const std::string filepath = entry.path().string();

                std::ifstream file(filepath);
                if (!file.is_open()) {
                    continue;
                }

                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();

                if (content.empty()) {
                    continue;
                }

                try {
                    json j = json::parse(content);
                    std::string levelName = j.value("name", filename.substr(0, filename.size() - 5));

                    LevelInfo lvl;
                    lvl.id = filename.substr(0, filename.size() - 5);
                    lvl.displayName = levelName;
                    lvl.path = "levels/" + filename;

                    customLevels.push_back(std::move(lvl));
                } catch (const std::exception &e) {
                    std::cerr << "{RoomManager::loadCustomWorldFromFilesystem} Failed to parse " << filepath << ": "
                              << e.what() << std::endl;
                }
            }

            if (!customLevels.empty()) {
                WorldLevels wl;
                wl.levels = std::move(customLevels);

                _worlds.push_back(WorldEntry{"custom", "Custom Levels"});
                _levelsByWorldId.emplace("custom", std::move(wl));
            }
        } catch (const std::exception &e) {
            std::cerr << "{RoomManager::loadCustomWorldFromFilesystem} Error scanning levels directory: " << e.what()
                      << std::endl;
        }
    }
} // namespace Engine
