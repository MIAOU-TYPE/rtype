/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** IResourceManager
*/

#pragma once

#include <cstddef>
#include <exception>
#include <memory>
#include <string>
#include <vector>

/**
 * @namespace Resources
 * @brief Contains all resource management classes and interfaces.
 */
namespace Resources
{
    /**
     * @class ResourceManagerError
     * @brief Exception class for resource manager errors.
     */
    class ResourceManagerError : public std::exception {
      public:
        /**
         * @brief Constructor for ResourceManagerError.
         * @param message The error message.
         */
        explicit ResourceManagerError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Override of what() method from std::exception.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @struct ResourceData
     * @brief Holds raw binary data for an embedded resource.
     */
    struct ResourceData {
        const unsigned char *data; ///> Pointer to raw binary data
        std::size_t size;          ///> Size of the data in bytes
    };

    /**
     * @class IResourceManager
     * @brief Interface for accessing embedded resources.
     *
     * This interface abstracts resource loading, allowing resources to be loaded
     * from embedded binary data or filesystem as fallback.
     */
    class IResourceManager {
      public:
        /**
         * @brief Virtual destructor for IResourceManager.
         */
        virtual ~IResourceManager() = default;

        /**
         * @brief Loads a resource by its path/identifier.
         * @param resourcePath The path or identifier of the resource.
         * @return ResourceData containing pointer to data and size, or {nullptr, 0} if not found.
         */
        virtual ResourceData loadResource(const std::string &resourcePath) const = 0;

        /**
         * @brief Checks if a resource exists.
         * @param resourcePath The path or identifier of the resource.
         * @return True if the resource exists, false otherwise.
         */
        virtual bool hasResource(const std::string &resourcePath) const = 0;

        /**
         * @brief Lists all available resources.
         * @return Vector of resource paths/identifiers.
         */
        virtual std::vector<std::string> listResources() const = 0;
    };

} // namespace Resources
