/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DLLoader
*/

#pragma once

#include <stdexcept>
#include <string>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace Library
{

    /**
     * @class DLLoaderError
     * @brief Exception class for DLLoader errors.
     * inherits from std::exception.
     */
    class DLLoaderError : public std::exception {
      public:
        /**
         * @brief Construct a new DLLoaderError object
         * @param message The error message
         */
        explicit DLLoaderError(const std::string &message) : _msg(message)
        {
        }

        /**
         * @brief Get the error message
         * @return const char* The error message
         */
        virtual const char *what() const noexcept override
        {
            return _msg.c_str();
        }

      private:
        std::string _msg; ///> The error message
    };

    /**
     * @class DLLoader
     * @brief Class for dynamic library loading and symbol retrieval.
     * Handles loading and unloading of dynamic libraries.
     * Provides methods to get symbols from the loaded library.
     * Uses platform-specific APIs for dynamic loading.
     * Throws DLLoaderError on errors.
     */
    class DLLoader {
      public:
        /**
         * @brief Construct a new DLLoader object and load the library
         * @param libPath The path to the dynamic library
         * @throws DLLoaderError if the library fails to load
         */
        explicit DLLoader(const std::string &libPath);

        /**
         * @brief Destroy the DLLoader object and unload the library
         */
        ~DLLoader();

        /**
         * @brief Get a symbol from the loaded library
         * @tparam T The type of the symbol (e.g., function pointer)
         * @param symbolName The name of the symbol to retrieve
         * @return T The symbol retrieved from the library
         * @throws DLLoaderError if the symbol cannot be found
         */
        template <typename T>
        T getSymbol(const std::string &symbolName);

        /**
         * @brief Check if the library is loaded
         * @return true if the library is loaded, false otherwise
         */
        bool isLoaded() const noexcept;

        /**
         * @brief Get the path of the loaded library
         * @return const std::string& The path of the loaded library
         */
        const std::string &path() const noexcept;

        /**
         * @brief Get the platform-specific library extension
         * @return const std::string & The platform-specific library extension
         */
        static const std::string &getPlatformLibraryExtension() noexcept;

      private:
        void *_handle = nullptr; ///> Handle to the loaded library
        std::string _path = "";  ///> Path to the loaded library
    };

} // namespace Library

#include "DLLoader.tpp"
