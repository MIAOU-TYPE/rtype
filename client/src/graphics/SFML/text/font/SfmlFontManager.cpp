/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlFontManager
*/

#include "SfmlFontManager.hpp"

namespace Graphics
{
    SfmlFontManager::SfmlFontManager(std::shared_ptr<Resources::IResourceManager> resources)
        : _resources(std::move(resources))
    {
    }

    FontHandle SfmlFontManager::load(const std::string &resourcePath) noexcept
    {
        if (const auto it = _pathToHandle.find(resourcePath); it != _pathToHandle.end()) {
            _fonts[it->second].refCount++;
            return it->second;
        }

        auto [data, size] = _resources->loadResource(resourcePath);
        if (!data || size == 0)
            return InvalidFont;

        sf::Font font;
        if (!font.openFromMemory(data, size))
            return InvalidFont;

        FontHandle handle = _nextHandle++;

        _fonts.emplace(handle, FontEntry{std::move(font), 1});
        _pathToHandle.emplace(resourcePath, handle);

        return handle;
    }

    void SfmlFontManager::unload(const FontHandle handle) noexcept
    {
        const auto it = _fonts.find(handle);
        if (it == _fonts.end())
            return;

        if (--it->second.refCount == 0) {
            _fonts.erase(it);

            for (auto pit = _pathToHandle.begin(); pit != _pathToHandle.end(); ++pit) {
                if (pit->second == handle) {
                    _pathToHandle.erase(pit);
                    break;
                }
            }
        }
    }

    bool SfmlFontManager::isValid(const FontHandle handle) const noexcept
    {
        return _fonts.contains(handle);
    }

    void SfmlFontManager::clear() noexcept
    {
        _fonts.clear();
        _pathToHandle.clear();
        _nextHandle = 1;
    }

    const sf::Font &SfmlFontManager::get(const FontHandle handle) const
    {
        const auto it = _fonts.find(handle);
        if (it == _fonts.end())
            throw FontManagerError("{SfmlFontManager::get} does not exist");
        return it->second.font;
    }
} // namespace Graphics
