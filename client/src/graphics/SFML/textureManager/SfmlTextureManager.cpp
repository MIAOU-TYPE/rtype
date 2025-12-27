/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlTextureManager
*/
#include "SfmlTextureManager.hpp"

namespace Graphics
{
    SfmlTextureManager::SfmlTextureManager(std::shared_ptr<Resources::IResourceManager> resources)
        : _resources(std::move(resources))
    {
    }

    TextureHandle SfmlTextureManager::load(const std::string &resourcePath)
    {
        if (const auto it = _pathToHandle.find(resourcePath); it != _pathToHandle.end()) {
            _textures[it->second].refCount++;
            return it->second;
        }

        auto [data, size] = _resources->loadResource(resourcePath);
        if (!data || size == 0)
            return InvalidTexture;

        sf::Texture texture;
        if (!texture.loadFromMemory(data, size))
            return InvalidTexture;

        TextureHandle handle = _nextHandle++;

        _textures.emplace(handle, TextureEntry{std::move(texture), 1});
        _pathToHandle.emplace(resourcePath, handle);

        return handle;
    }

    void SfmlTextureManager::unload(const TextureHandle handle)
    {
        const auto it = _textures.find(handle);
        if (it == _textures.end())
            return;

        if (--it->second.refCount == 0) {
            _textures.erase(it);

            for (auto pit = _pathToHandle.begin(); pit != _pathToHandle.end(); ++pit) {
                if (pit->second == handle) {
                    _pathToHandle.erase(pit);
                    break;
                }
            }
        }
    }

    bool SfmlTextureManager::isValid(const TextureHandle handle) const noexcept
    {
        return _textures.contains(handle);
    }

    TextureSize SfmlTextureManager::getSize(const TextureHandle handle) const
    {
        const auto &tex = get(handle);
        const auto size = tex.getSize();
        return {size.x, size.y};
    }

    void SfmlTextureManager::clear()
    {
        _textures.clear();
        _pathToHandle.clear();
        _nextHandle = 1;
    }

    const sf::Texture &SfmlTextureManager::get(const TextureHandle handle) const
    {
        const auto it = _textures.find(handle);
        if (it == _textures.end())
            throw TextureError("{SfmlTextureManager::get} Invalid texture handle");
        return it->second.texture;
    }
} // namespace Graphics
