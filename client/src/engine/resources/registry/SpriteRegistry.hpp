/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteRegistry
*/

#pragma once

#include <string>
#include <unordered_map>

#include <exception>
#include "Animation.hpp"
#include "Position.hpp"
#include "SpriteDefinition.hpp"

namespace Engine
{
    /**
     * @class SpriteRegistryError
     * @brief Exception class for errors related to the SpriteRegistry.
     */
    class SpriteRegistryError : public std::exception {
      public:
        /**
         * @brief Constructs a SpriteRegistryError with a given message.
         * @param message Error message describing the issue.
         */
        explicit SpriteRegistryError(const std::string &message) : _message("\n\t" + message)
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
     * @class SpriteRegistry
     * @brief Manages a registry of sprite definitions identified by unique string IDs.
     * Allows adding, retrieving, and checking existence of sprite definitions.
     */
    class SpriteRegistry {
      public:
        /**
         * @brief Adds a sprite definition to the registry.
         * @param spriteId Unique ID for the sprite.
         * @param def The SpriteDefinition to be added.
         */
        void add(int spriteId, SpriteDefinition def);

        /**
         * @brief Retrieves a sprite definition by its ID.
         * @param spriteId Unique ID for the sprite.
         * @return Reference to the corresponding SpriteDefinition.
         */
        const SpriteDefinition &get(int spriteId) const;

        /**
         * @brief Checks if a sprite definition exists in the registry.
         * @param spriteId Unique ID for the sprite.
         * @return True if the sprite definition exists, false otherwise.
         */
        bool exists(int spriteId) const;

        /**
         * @brief Iterates over all sprite definitions in the registry.
         * @tparam Fn Callable type that takes (int id, SpriteDefinition &).
         * @param fn Function to be called for each sprite definition.
         */
        template <typename Fn>
        void forEach(Fn &&fn);

      private:
        std::unordered_map<int, SpriteDefinition> _sprites; ///> Map of sprite IDs to their definitions
    };

} // namespace Engine

#include "SpriteRegistry.tpp"