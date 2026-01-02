/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteLoader
*/

#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include "SpriteRegistry.hpp"

namespace Engine
{
    class SpriteLoaderError : public std::exception {
      public:
        /**
         * @brief Constructs a SpriteLoaderError with a given message.
         * @param message Error message describing the issue.
         */
        explicit SpriteLoaderError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Returns the error message.
         * @return C-string representing the error message.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message = ""; ///> Error message
    };

    /**
     * @class SpriteLoader
     * @brief Responsible for loading sprite definitions from JSON files into a SpriteRegistry.
     * The JSON file should contain the texture path, default animation, and a list of animations with their frames.
     */
    class SpriteLoader {
      public:
        /**
         * @brief Loads sprite definitions from a JSON file and populates the provided SpriteRegistry.
         * @param path Path to the JSON file containing sprite definitions.
         * @param registry Reference to the SpriteRegistry to populate.
         */
        static void loadFromFile(const std::string &path, SpriteRegistry &registry);
    };

} // namespace Engine
