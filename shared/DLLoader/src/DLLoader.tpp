/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DLLoader
*/

#pragma once
#include "DLLoader.hpp"

#ifdef _WIN32
    #include <windows.h>
    #define GET_SYMBOL(handle, symbol) GetProcAddress(static_cast<HMODULE>(handle), symbol.c_str())
#else
    #include <dlfcn.h>
    #define GET_SYMBOL(handle, symbol) dlsym(handle, symbol.c_str())
#endif

template<typename T>
T Library::DLLoader::getSymbol(const std::string &symbolName)
{
    if (!_handle)
        throw DLLoaderError("Library not loaded: " + _path);

    #ifdef __linux__
        dlerror();
    #endif
    void *symbol = GET_SYMBOL(_handle, symbolName);
    #ifndef _WIN32
        const char *err = dlerror();
        if (err)
            throw DLLoaderError("Failed to load symbol '" + symbolName + "': " + err);
    #endif
    if (!symbol)
        throw DLLoaderError("Failed to load symbol: " + symbolName + " from " + _path);

    return reinterpret_cast<T>(symbol);
}
