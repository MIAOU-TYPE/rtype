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

DLLoader::DLLoader(const std::string &libName, const std::string &baseDir) : _path(getLibraryPath(libName, baseDir))
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

std::string DLLoader::getLibraryPath(const std::string &name, const std::string &baseDir) noexcept
{
#if defined(_WIN32)
    namespace fs = std::filesystem;
    fs::path dllName = name + ".dll";

    std::vector<fs::path> searchPaths = {
        fs::current_path(),
        fs::current_path() / "Debug",
        fs::current_path() / "Release",
        fs::current_path() / baseDir,
        fs::current_path().parent_path() / "build/Debug",
        fs::current_path().parent_path() / "build/Release",
        fs::current_path().parent_path() / "build",
    };

    for (auto &folder : searchPaths) {
        fs::path candidate = folder / dllName;
        if (fs::exists(candidate)) {
            return candidate.string();
        }
    }

    return dllName.string();
#else
    return baseDir + "lib" + name +
#if defined(__APPLE__)
           ".dylib";
#else
           ".so";
#endif
#endif
}
