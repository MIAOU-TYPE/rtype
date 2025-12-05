/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DLLoader
*/

#include "DLLoader.hpp"
using namespace Library;

#ifdef _WIN32
    #define LOAD_LIBRARY(path)   LoadLibraryA(path.c_str())
    #define FREE_LIBRARY(handle) FreeLibrary(static_cast<HMODULE>(handle))
#else
    #define LOAD_LIBRARY(path)   dlopen(path.c_str(), RTLD_LAZY)
    #define FREE_LIBRARY(handle) dlclose(handle)
#endif

DLLoader::DLLoader(const std::string &libPath) : _path(libPath)
{
#ifdef __linux__
    dlerror();
#endif
    auto pathWithExt = libPath + getPlatformLibraryExtension();
    _handle = LOAD_LIBRARY(pathWithExt);

#ifdef __linux__
    const char *error = dlerror();
    if (error)
        throw DLLoaderError("Failed to load library: " + libPath + " - " + std::string(error));
#endif
    if (!_handle)
        throw DLLoaderError("Failed to load library: " + libPath);
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

const std::string &DLLoader::getPlatformLibraryExtension() noexcept
{
    static const std::string ext =
#ifdef _WIN32
        ".dll";
#elif __APPLE__
        ".dylib";
#else
        ".so";
#endif
    return ext;
}
