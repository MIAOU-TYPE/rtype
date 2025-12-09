/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DLLoader
*/

#include "DLLoader.hpp"
using namespace Library;

#ifdef _WIN32
    #include <filesystem>
    #define LOAD_LIBRARY(path)   LoadLibraryA(path.c_str())
    #define FREE_LIBRARY(handle) FreeLibrary(static_cast<HMODULE>(handle))
#else
    #define LOAD_LIBRARY(path)   dlopen(path.c_str(), RTLD_LAZY)
    #define FREE_LIBRARY(handle) dlclose(handle)
#endif

DLLoader::DLLoader(const std::string &libName) : _path(getLibraryPath(libName))
{
#ifdef __linux__
    dlerror();
#endif
    _handle = LOAD_LIBRARY(_path);

#ifdef __linux__
    const char *error = dlerror();
    if (error)
        throw DLLoaderError("Failed to load library: " + _path + " - " + std::string(error));
#endif
    if (!_handle)
        throw DLLoaderError("Failed to load library: " + _path);
}

DLLoader::~DLLoader()
{
    if (_handle)
        FREE_LIBRARY(_handle);
    _handle = nullptr;
}

bool DLLoader::isLoaded() const noexcept
{
    return _handle != nullptr;
}

const std::string &DLLoader::path() const noexcept
{
    return _path;
}

std::string DLLoader::getLibraryPath(const std::string &name) noexcept
{
#ifdef _WIN32
    if (std::filesystem::exists(name + ".dll"))
        return name + ".dll";

    const char *configs[] = {"Debug", "Release"};

    for (auto c : configs) {
        std::string p = "../build/" + std::string(c) + "/" + name + ".dll";
        if (std::filesystem::exists(p))
            return p;
    }
    return name + ".dll";
#elif __APPLE__
    return "build/lib" + name + ".dylib";
#else
    return "build/lib" + name + ".so";
#endif
}
