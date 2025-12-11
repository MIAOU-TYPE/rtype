/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** EmbeddedResourceManager
*/

#pragma once

#include <string>
#include <vector>
#include "IResourceManager.hpp"
#include <unordered_map>

/**
 * @namespace Resources
 * @brief Contains all resource management classes and interfaces.
 */
namespace Resources
{
    /**
     * @class EmbeddedResourceManager
     * @brief Manages access to resources embedded in the executable.
     *
     * This class provides access to binary data that has been compiled
     * directly into the executable, eliminating the need for external files.
     */
    class EmbeddedResourceManager : public IResourceManager {
      public:
        /**
         * @brief Constructor for EmbeddedResourceManager.
         */
        EmbeddedResourceManager();

        /**
         * @brief Destructor for EmbeddedResourceManager.
         */
        ~EmbeddedResourceManager() override = default;

        /**
         * @brief Loads a resource by its path/identifier.
         * @param resourcePath The path or identifier of the resource (e.g., "sprites/player.png").
         * @return ResourceData containing pointer to data and size, or {nullptr, 0} if not found.
         */
        ResourceData loadResource(const std::string &resourcePath) const override;

        /**
         * @brief Checks if a resource exists.
         * @param resourcePath The path or identifier of the resource.
         * @return True if the resource exists, false otherwise.
         */
        bool hasResource(const std::string &resourcePath) const override;

        /**
         * @brief Lists all available resources.
         * @return Vector of resource paths/identifiers.
         */
        std::vector<std::string> listResources() const override;

      private:
        /**
         * @brief Registers all embedded resources.
         *
         * This method is called by the constructor to populate the resource map
         * with all embedded binary data.
         */
        void registerResources();

        std::unordered_map<std::string, ResourceData> _resources; ///> Map of resource paths to data
    };

} // namespace Resources
