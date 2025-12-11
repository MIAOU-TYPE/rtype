/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** ResourceError
*/

#pragma once

#include <exception>
#include <string>

/**
 * @namespace Resources
 * @brief Contains all resource management classes and interfaces.
 */
namespace Resources
{
    /**
     * @class ResourceError
     * @brief Exception class for resource-related errors.
     *
     * Thrown when resource loading, parsing, or access fails.
     */
    class ResourceError : public std::exception {
      public:
        /**
         * @brief Constructor for ResourceError.
         * @param message The error message to be associated with the exception.
         */
        explicit ResourceError(const std::string &message) : _message("\n\tResource Error: " + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
    };

} // namespace Resources
