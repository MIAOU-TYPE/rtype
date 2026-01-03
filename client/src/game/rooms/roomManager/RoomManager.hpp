/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomManager
*/

#pragma once
#include <optional>
#include "IResourceManager.hpp"
#include "RoomTypes.hpp"
#include <string_view>
#include <unordered_map>

namespace Engine
{
    /**
     * @class RoomManagerError
     * @brief Exception class for RoomManager-related errors.
     */
    class RoomManagerError : public std::exception {
      public:
        /**
         * @brief Constructs a RoomManagerError with a given message.
         * @param message The error message.
         */
        explicit RoomManagerError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> The error message.
    };

    /**
     * @class RoomManager
     * @brief Manages game rooms, including loading worlds and levels from embedded resources.
     */
    class RoomManager {
      public:
        /**
         * @brief Constructs a RoomManager with the given resource manager.
         * @param resources Shared pointer to the resource manager.
         * @throws RoomManagerError if resources are null or no worlds/levels are found.
         */
        explicit RoomManager(std::shared_ptr<Resources::IResourceManager> resources);

        /**
         * @brief Retrieves the list of available worlds.
         * @return A constant reference to the vector of WorldEntry objects.
         */
        [[nodiscard]] const std::vector<WorldEntry> &worlds() const noexcept;

        /**
         * @brief Retrieves the levels for a given world ID and difficulty.
         * @param worldId The ID of the world.
         * @param difficulty The difficulty level.
         * @return A constant reference to the vector of LevelInfo objects for the specified world and difficulty.
         * @throws RoomManagerError if the world ID is unknown.
         */
        [[nodiscard]] const std::vector<LevelInfo> &levelsFor(std::string_view worldId, Difficulty difficulty) const;

      private:
        /**
         * @brief Loads worlds and levels from embedded resources.
         */
        void loadFromEmbedded();

        /**
         * @brief Lists the IDs of embedded worlds.
         * @return A vector of strings containing the world IDs.
         */
        [[nodiscard]] std::vector<std::string> listEmbeddedWorldIds() const;

        /**
         * @brief Reads a text asset from the resource manager.
         * @param assetPath The path to the asset.
         * @return An optional string containing the asset content, or std::nullopt if not found.
         */
        [[nodiscard]] std::optional<std::string> readTextAsset(std::string_view assetPath) const;

        /**
         * @brief Parses a JSON string to extract a list of levels.
         * @param jsonText The JSON text to parse.
         * @return A vector of LevelInfo objects parsed from the JSON.
         * @throws RoomManagerError if parsing fails or the JSON format is incorrect.
         */
        [[nodiscard]] static std::vector<LevelInfo> parseLevelsListJson(std::string_view jsonText);

        /**
         * @brief Parses a JSON string to extract the world name.
         * @param jsonText The JSON text to parse.
         * @return An optional string containing the world name, or std::nullopt if not found.
         */
        [[nodiscard]] static std::optional<std::string> parseWorldNameJson(std::string_view jsonText);

        /**
         * @brief Constructs the resource path for a given world ID and file name.
         * @param worldId The ID of the world.
         * @param fileName The name of the file.
         * @return A string representing the full resource path.
         */
        [[nodiscard]] static std::string makePath(std::string_view worldId, std::string_view fileName);

        std::shared_ptr<Resources::IResourceManager> _resources; ///> Shared pointer to the resource manager.
        std::vector<WorldEntry> _worlds;                         ///> List of available worlds.
        std::unordered_map<std::string, WorldLevels>
            _levelsByWorldId; ///> Map of world IDs to their corresponding levels.
    };
} // namespace Engine
