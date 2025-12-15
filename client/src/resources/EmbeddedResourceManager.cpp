/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** EmbeddedResourceManager
*/

#include "EmbeddedResourceManager.hpp"
#include "ResourceError.hpp"

namespace EmbeddedResources
{
    extern const unsigned char bg_preview_png[];
    extern const unsigned int bg_preview_png_size;
    extern const unsigned char background_space_png[];
    extern const unsigned int background_space_png_size;
    extern const unsigned char play_bt_png[];
    extern const unsigned int play_bt_png_size;
    extern const unsigned char play_bt_hold_png[];
    extern const unsigned int play_bt_hold_png_size;
    extern const unsigned char play_bt_press_png[];
    extern const unsigned int play_bt_press_png_size;
    extern const unsigned char player_png[];
    extern const unsigned int player_png_size;
    extern const unsigned char player2_png[];
    extern const unsigned int player2_png_size;
    extern const unsigned char player3_png[];
    extern const unsigned int player3_png_size;
    extern const unsigned char player4_png[];
    extern const unsigned int player4_png_size;
    extern const unsigned char enemy_png[];
    extern const unsigned int enemy_png_size;
    extern const unsigned char missile_png[];
    extern const unsigned int missile_png_size;
    extern const unsigned char explose_png[];
    extern const unsigned int explose_png_size;

    extern const unsigned char r_type_otf[];
    extern const unsigned int r_type_otf_size;
} // namespace EmbeddedResources

using namespace Resources;

EmbeddedResourceManager::EmbeddedResourceManager()
{
    registerResources();
}

void EmbeddedResourceManager::registerResources()
{
    _resources["sprites/bg-preview.png"] = {EmbeddedResources::bg_preview_png, EmbeddedResources::bg_preview_png_size};
    _resources["sprites/background_space.png"] = {
        EmbeddedResources::background_space_png, EmbeddedResources::background_space_png_size};
    _resources["sprites/play_bt.png"] = {EmbeddedResources::play_bt_png, EmbeddedResources::play_bt_png_size};
    _resources["sprites/play_bt_hold.png"] = {
        EmbeddedResources::play_bt_hold_png, EmbeddedResources::play_bt_hold_png_size};
    _resources["sprites/play_bt_press.png"] = {
        EmbeddedResources::play_bt_press_png, EmbeddedResources::play_bt_press_png_size};
    _resources["sprites/player.png"] = {EmbeddedResources::player_png, EmbeddedResources::player_png_size};
    _resources["sprites/player2.png"] = {EmbeddedResources::player2_png, EmbeddedResources::player2_png_size};
    _resources["sprites/player3.png"] = {EmbeddedResources::player3_png, EmbeddedResources::player3_png_size};
    _resources["sprites/player4.png"] = {EmbeddedResources::player4_png, EmbeddedResources::player4_png_size};
    _resources["sprites/enemy.png"] = {EmbeddedResources::enemy_png, EmbeddedResources::enemy_png_size};
    _resources["sprites/missile.png"] = {EmbeddedResources::missile_png, EmbeddedResources::missile_png_size};
    _resources["sprites/explose.png"] = {EmbeddedResources::explose_png, EmbeddedResources::explose_png_size};

    _resources["fonts/r-type.otf"] = {EmbeddedResources::r_type_otf, EmbeddedResources::r_type_otf_size};
}

ResourceData EmbeddedResourceManager::loadResource(const std::string &resourcePath) const
{
    auto it = _resources.find(resourcePath);
    if (it != _resources.end()) {
        return it->second;
    }
    return {nullptr, 0};
}

bool EmbeddedResourceManager::hasResource(const std::string &resourcePath) const
{
    return _resources.find(resourcePath) != _resources.end();
}

std::vector<std::string> EmbeddedResourceManager::listResources() const
{
    std::vector<std::string> resourceList;
    resourceList.reserve(_resources.size());
    for (const auto &pair : _resources) {
        resourceList.push_back(pair.first);
    }
    return resourceList;
}
