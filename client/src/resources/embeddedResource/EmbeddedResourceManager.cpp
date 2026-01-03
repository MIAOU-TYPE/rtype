/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** EmbeddedResourceManager
*/

#include "EmbeddedResourceManager.hpp"

#include <ranges>

namespace EmbeddedResources
{
    extern const unsigned char bg_preview_png[];
    extern const unsigned int bg_preview_png_size;
    extern const unsigned char background_space_png[];
    extern const unsigned int background_space_png_size;
    extern const unsigned char menu_logo_png[];
    extern const unsigned int menu_logo_png_size;
    extern const unsigned char game_over_png[];
    extern const unsigned int game_over_png_size;
    extern const unsigned char button_s_pressed_png[];
    extern const unsigned int button_s_pressed_png_size;
    extern const unsigned char button_s_released_png[];
    extern const unsigned int button_s_released_png_size;
    extern const unsigned char button_s_hover_png[];
    extern const unsigned int button_s_hover_png_size;
    extern const unsigned char button_l_pressed_png[];
    extern const unsigned int button_l_pressed_png_size;
    extern const unsigned char button_l_released_png[];
    extern const unsigned int button_l_released_png_size;
    extern const unsigned char button_l_hover_png[];
    extern const unsigned int button_l_hover_png_size;
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
    extern const unsigned char enemy2_png[];
    extern const unsigned int enemy2_png_size;
    extern const unsigned char enemy3_png[];
    extern const unsigned int enemy3_png_size;
    extern const unsigned char missile_png[];
    extern const unsigned int missile_png_size;
    extern const unsigned char explose_png[];
    extern const unsigned int explose_png_size;
    extern const unsigned char boss_png[];
    extern const unsigned int boss_png_size;

    extern const unsigned char r_type_otf[];
    extern const unsigned int r_type_otf_size;
    extern const unsigned char font_ttf[];
    extern const unsigned int font_ttf_size;

    extern const unsigned char boss_theme_flac[];
    extern const unsigned int boss_theme_flac_size;
    extern const unsigned char explosion_wav[];
    extern const unsigned int explosion_wav_size;
    extern const unsigned char game_theme_flac[];
    extern const unsigned int game_theme_flac_size;
    extern const unsigned char hit_wav[];
    extern const unsigned int hit_wav_size;
    extern const unsigned char menu_theme_flac[];
    extern const unsigned int menu_theme_flac_size;
    extern const unsigned char powerup_wav[];
    extern const unsigned int powerup_wav_size;
    extern const unsigned char shoot_wav[];
    extern const unsigned int shoot_wav_size;

    extern const unsigned char easy_json[];
    extern const unsigned int easy_json_size;
    extern const unsigned char medium_json[];
    extern const unsigned int medium_json_size;
    extern const unsigned char hard_json[];
    extern const unsigned int hard_json_size;
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
    _resources["sprites/menu_logo.png"] = {EmbeddedResources::menu_logo_png, EmbeddedResources::menu_logo_png_size};
    _resources["sprites/game_over.png"] = {EmbeddedResources::game_over_png, EmbeddedResources::game_over_png_size};
    _resources["sprites/button_s_pressed.png"] = {
        EmbeddedResources::button_s_pressed_png, EmbeddedResources::button_s_pressed_png_size};
    _resources["sprites/button_s_released.png"] = {
        EmbeddedResources::button_s_released_png, EmbeddedResources::button_s_released_png_size};
    _resources["sprites/button_s_hover.png"] = {
        EmbeddedResources::button_s_hover_png, EmbeddedResources::button_s_hover_png_size};
    _resources["sprites/button_l_pressed.png"] = {
        EmbeddedResources::button_l_pressed_png, EmbeddedResources::button_l_pressed_png_size};
    _resources["sprites/button_l_released.png"] = {
        EmbeddedResources::button_l_released_png, EmbeddedResources::button_l_released_png_size};
    _resources["sprites/button_l_hover.png"] = {
        EmbeddedResources::button_l_hover_png, EmbeddedResources::button_l_hover_png_size};
    _resources["sprites/player.png"] = {EmbeddedResources::player_png, EmbeddedResources::player_png_size};
    _resources["sprites/player2.png"] = {EmbeddedResources::player2_png, EmbeddedResources::player2_png_size};
    _resources["sprites/player3.png"] = {EmbeddedResources::player3_png, EmbeddedResources::player3_png_size};
    _resources["sprites/player4.png"] = {EmbeddedResources::player4_png, EmbeddedResources::player4_png_size};
    _resources["sprites/enemy.png"] = {EmbeddedResources::enemy_png, EmbeddedResources::enemy_png_size};
    _resources["sprites/enemy2.png"] = {EmbeddedResources::enemy2_png, EmbeddedResources::enemy2_png_size};
    _resources["sprites/enemy3.png"] = {EmbeddedResources::enemy3_png, EmbeddedResources::enemy3_png_size};
    _resources["sprites/missile.png"] = {EmbeddedResources::missile_png, EmbeddedResources::missile_png_size};
    _resources["sprites/explose.png"] = {EmbeddedResources::explose_png, EmbeddedResources::explose_png_size};
    _resources["sprites/boss.png"] = {EmbeddedResources::boss_png, EmbeddedResources::boss_png_size};

    _resources["fonts/r-type.otf"] = {EmbeddedResources::r_type_otf, EmbeddedResources::r_type_otf_size};
    _resources["fonts/font.ttf"] = {EmbeddedResources::font_ttf, EmbeddedResources::font_ttf_size};

    _resources["sounds/boss_theme.flac"] = {
        EmbeddedResources::boss_theme_flac, EmbeddedResources::boss_theme_flac_size};
    _resources["sounds/explosion.wav"] = {EmbeddedResources::explosion_wav, EmbeddedResources::explosion_wav_size};
    _resources["sounds/game_theme.flac"] = {
        EmbeddedResources::game_theme_flac, EmbeddedResources::game_theme_flac_size};
    _resources["sounds/hit.wav"] = {EmbeddedResources::hit_wav, EmbeddedResources::hit_wav_size};
    _resources["sounds/menu_theme.flac"] = {
        EmbeddedResources::menu_theme_flac, EmbeddedResources::menu_theme_flac_size};
    _resources["sounds/powerup.wav"] = {EmbeddedResources::powerup_wav, EmbeddedResources::powerup_wav_size};
    _resources["sounds/shoot.wav"] = {EmbeddedResources::shoot_wav, EmbeddedResources::shoot_wav_size};

    _resources["levels/space/easy.json"] = {
        EmbeddedResources::easy_json, EmbeddedResources::easy_json_size};
    _resources["levels/space/medium.json"] = {
        EmbeddedResources::medium_json, EmbeddedResources::medium_json_size};
    _resources["levels/space/hard.json"] = {
        EmbeddedResources::hard_json, EmbeddedResources::hard_json_size};
}

ResourceData EmbeddedResourceManager::loadResource(const std::string &resourcePath) const noexcept
{
    if (const auto it = _resources.find(resourcePath); it != _resources.end())
        return it->second;
    return {nullptr, 0};
}

bool EmbeddedResourceManager::hasResource(const std::string &resourcePath) const noexcept
{
    return _resources.contains(resourcePath);
}

std::vector<std::string> EmbeddedResourceManager::listResources() const noexcept
{
    std::vector<std::string> resourceList;
    resourceList.reserve(_resources.size());
    for (const auto &key : _resources | std::views::keys)
        resourceList.push_back(key);
    return resourceList;
}
