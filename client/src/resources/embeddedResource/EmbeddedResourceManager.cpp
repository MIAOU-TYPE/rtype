/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EmbeddedResourceManager
*/

#include "EmbeddedResourceManager.hpp"

namespace EmbeddedResources
{
    extern const unsigned char sprites_popup_png[];
    extern const unsigned int sprites_popup_png_size;
    extern const unsigned char sprites_background_jungle_png[];
    extern const unsigned int sprites_background_jungle_png_size;
    extern const unsigned char sprites_background_ruin_png[];
    extern const unsigned int sprites_background_ruin_png_size;
    extern const unsigned char sprites_background_wall_png[];
    extern const unsigned int sprites_background_wall_png_size;
    extern const unsigned char sprites_bg_preview_png[];
    extern const unsigned int sprites_bg_preview_png_size;
    extern const unsigned char sprites_background_space_png[];
    extern const unsigned int sprites_background_space_png_size;
    extern const unsigned char sprites_menu_logo_png[];
    extern const unsigned int sprites_menu_logo_png_size;
    extern const unsigned char sprites_game_over_png[];
    extern const unsigned int sprites_game_over_png_size;
    extern const unsigned char sprites_button_s_pressed_png[];
    extern const unsigned int sprites_button_s_pressed_png_size;
    extern const unsigned char sprites_button_s_released_png[];
    extern const unsigned int sprites_button_s_released_png_size;
    extern const unsigned char sprites_button_s_hover_png[];
    extern const unsigned int sprites_button_s_hover_png_size;
    extern const unsigned char sprites_button_l_pressed_png[];
    extern const unsigned int sprites_button_l_pressed_png_size;
    extern const unsigned char sprites_button_l_released_png[];
    extern const unsigned int sprites_button_l_released_png_size;
    extern const unsigned char sprites_button_l_hover_png[];
    extern const unsigned int sprites_button_l_hover_png_size;
    extern const unsigned char sprites_player_png[];
    extern const unsigned int sprites_player_png_size;
    extern const unsigned char sprites_player2_png[];
    extern const unsigned int sprites_player2_png_size;
    extern const unsigned char sprites_player3_png[];
    extern const unsigned int sprites_player3_png_size;
    extern const unsigned char sprites_player4_png[];
    extern const unsigned int sprites_player4_png_size;
    extern const unsigned char sprites_enemy_png[];
    extern const unsigned int sprites_enemy_png_size;
    extern const unsigned char sprites_enemy2_png[];
    extern const unsigned int sprites_enemy2_png_size;
    extern const unsigned char sprites_enemy3_png[];
    extern const unsigned int sprites_enemy3_png_size;
    extern const unsigned char sprites_missile_png[];
    extern const unsigned int sprites_missile_png_size;
    extern const unsigned char sprites_missile_enemy_png[];
    extern const unsigned int sprites_missile_enemy_png_size;
    extern const unsigned char sprites_missile_enemy2_png[];
    extern const unsigned int sprites_missile_enemy2_png_size;
    extern const unsigned char sprites_explose_png[];
    extern const unsigned int sprites_explose_png_size;
    extern const unsigned char sprites_homingbullet_png[];
    extern const unsigned int sprites_homingbullet_png_size;
    extern const unsigned char sprites_boss_png[];
    extern const unsigned int sprites_boss_png_size;
    extern const unsigned char sprites_boss2_png[];
    extern const unsigned int sprites_boss2_png_size;
    extern const unsigned char sprites_boss3_boss3P1_png[];
    extern const unsigned int sprites_boss3_boss3P1_png_size;
    extern const unsigned char sprites_boss3_boss3P2_png[];
    extern const unsigned int sprites_boss3_boss3P2_png_size;
    extern const unsigned char sprites_boss3_boss3P3_png[];
    extern const unsigned int sprites_boss3_boss3P3_png_size;
    extern const unsigned char sprites_boss3_boss3P4_png[];
    extern const unsigned int sprites_boss3_boss3P4_png_size;
    extern const unsigned char sprites_boss3_boss3P5_png[];
    extern const unsigned int sprites_boss3_boss3P5_png_size;
    extern const unsigned char sprites_boss3_boss3P6_png[];
    extern const unsigned int sprites_boss3_boss3P6_png_size;
    extern const unsigned char sprites_boss3_boss3P7_png[];
    extern const unsigned int sprites_boss3_boss3P7_png_size;
    extern const unsigned char sprites_boss3_boss3Tail_png[];
    extern const unsigned int sprites_boss3_boss3Tail_png_size;
    extern const unsigned char sprites_obstacle_png[];
    extern const unsigned int sprites_obstacle_png_size;
    extern const unsigned char sprites_power_up_png[];
    extern const unsigned int sprites_power_up_png_size;
    extern const unsigned char sprites_power_up_attached_png[];
    extern const unsigned int sprites_power_up_attached_png_size;
    extern const unsigned char sprites_power_up2_png[];
    extern const unsigned int sprites_power_up2_png_size;
    extern const unsigned char sprites_power_up3_png[];
    extern const unsigned int sprites_power_up3_png_size;
    extern const unsigned char sprites_laser_png[];
    extern const unsigned int sprites_laser_png_size;
    extern const unsigned char sprites_bubble_png[];
    extern const unsigned int sprites_bubble_png_size;
    extern const unsigned char sprites_missile_big_png[];
    extern const unsigned int sprites_missile_big_png_size;
    extern const unsigned char sprites_bar_png[];
    extern const unsigned int sprites_bar_png_size;

    extern const unsigned char shaders_colorblind_frag[];
    extern const unsigned int shaders_colorblind_frag_size;

    extern const unsigned char fonts_font_ttf[];
    extern const unsigned int fonts_font_ttf_size;

    extern const unsigned char sounds_boss_theme_flac[];
    extern const unsigned int sounds_boss_theme_flac_size;
    extern const unsigned char sounds_explosion_wav[];
    extern const unsigned int sounds_explosion_wav_size;
    extern const unsigned char sounds_game_theme_flac[];
    extern const unsigned int sounds_game_theme_flac_size;
    extern const unsigned char sounds_hit_wav[];
    extern const unsigned int sounds_hit_wav_size;
    extern const unsigned char sounds_menu_theme_flac[];
    extern const unsigned int sounds_menu_theme_flac_size;
    extern const unsigned char sounds_powerup_wav[];
    extern const unsigned int sounds_powerup_wav_size;
    extern const unsigned char sounds_shoot_wav[];
    extern const unsigned int sounds_shoot_wav_size;

    extern const unsigned char levels_world1_levels_json[];
    extern const unsigned int levels_world1_levels_json_size;
    extern const unsigned char levels_world2_levels_json[];
    extern const unsigned int levels_world2_levels_json_size;
    extern const unsigned char levels_world3_levels_json[];
    extern const unsigned int levels_world3_levels_json_size;
} // namespace EmbeddedResources

using namespace Resources;

EmbeddedResourceManager::EmbeddedResourceManager()
{
    registerResources();
}

void EmbeddedResourceManager::registerResources()
{
    _resources["sprites/popup.png"] = {EmbeddedResources::sprites_popup_png, EmbeddedResources::sprites_popup_png_size};
    _resources["sprites/bg-preview.png"] = {
        EmbeddedResources::sprites_bg_preview_png, EmbeddedResources::sprites_bg_preview_png_size};
    _resources["sprites/background_space.png"] = {
        EmbeddedResources::sprites_background_space_png, EmbeddedResources::sprites_background_space_png_size};
    _resources["sprites/background_jungle.png"] = {
        EmbeddedResources::sprites_background_jungle_png, EmbeddedResources::sprites_background_jungle_png_size};
    _resources["sprites/background_ruin.png"] = {
        EmbeddedResources::sprites_background_ruin_png, EmbeddedResources::sprites_background_ruin_png_size};
    _resources["sprites/background_wall.png"] = {
        EmbeddedResources::sprites_background_wall_png, EmbeddedResources::sprites_background_wall_png_size};
    _resources["sprites/menu_logo.png"] = {
        EmbeddedResources::sprites_menu_logo_png, EmbeddedResources::sprites_menu_logo_png_size};
    _resources["sprites/game_over.png"] = {
        EmbeddedResources::sprites_game_over_png, EmbeddedResources::sprites_game_over_png_size};
    _resources["sprites/button_s_pressed.png"] = {
        EmbeddedResources::sprites_button_s_pressed_png, EmbeddedResources::sprites_button_s_pressed_png_size};
    _resources["sprites/button_s_released.png"] = {
        EmbeddedResources::sprites_button_s_released_png, EmbeddedResources::sprites_button_s_released_png_size};
    _resources["sprites/button_s_hover.png"] = {
        EmbeddedResources::sprites_button_s_hover_png, EmbeddedResources::sprites_button_s_hover_png_size};
    _resources["sprites/button_l_pressed.png"] = {
        EmbeddedResources::sprites_button_l_pressed_png, EmbeddedResources::sprites_button_l_pressed_png_size};
    _resources["sprites/button_l_released.png"] = {
        EmbeddedResources::sprites_button_l_released_png, EmbeddedResources::sprites_button_l_released_png_size};
    _resources["sprites/button_l_hover.png"] = {
        EmbeddedResources::sprites_button_l_hover_png, EmbeddedResources::sprites_button_l_hover_png_size};
    _resources["sprites/player.png"] = {
        EmbeddedResources::sprites_player_png, EmbeddedResources::sprites_player_png_size};
    _resources["sprites/player2.png"] = {
        EmbeddedResources::sprites_player2_png, EmbeddedResources::sprites_player2_png_size};
    _resources["sprites/player3.png"] = {
        EmbeddedResources::sprites_player3_png, EmbeddedResources::sprites_player3_png_size};
    _resources["sprites/player4.png"] = {
        EmbeddedResources::sprites_player4_png, EmbeddedResources::sprites_player4_png_size};
    _resources["sprites/enemy.png"] = {EmbeddedResources::sprites_enemy_png, EmbeddedResources::sprites_enemy_png_size};
    _resources["sprites/enemy2.png"] = {
        EmbeddedResources::sprites_enemy2_png, EmbeddedResources::sprites_enemy2_png_size};
    _resources["sprites/enemy3.png"] = {
        EmbeddedResources::sprites_enemy3_png, EmbeddedResources::sprites_enemy3_png_size};
    _resources["sprites/missile.png"] = {
        EmbeddedResources::sprites_missile_png, EmbeddedResources::sprites_missile_png_size};
    _resources["sprites/missile_enemy.png"] = {
        EmbeddedResources::sprites_missile_enemy_png, EmbeddedResources::sprites_missile_enemy_png_size};
    _resources["sprites/homingbullet.png"] = {
        EmbeddedResources::sprites_homingbullet_png, EmbeddedResources::sprites_homingbullet_png_size};
    _resources["sprites/explose.png"] = {
        EmbeddedResources::sprites_explose_png, EmbeddedResources::sprites_explose_png_size};
    _resources["sprites/boss.png"] = {EmbeddedResources::sprites_boss_png, EmbeddedResources::sprites_boss_png_size};
    _resources["sprites/boss2.png"] = {EmbeddedResources::sprites_boss2_png, EmbeddedResources::sprites_boss2_png_size};
    _resources["sprites/boss3/boss3P1.png"] = {
        EmbeddedResources::sprites_boss3_boss3P1_png, EmbeddedResources::sprites_boss3_boss3P1_png_size};
    _resources["sprites/boss3/boss3P2.png"] = {
        EmbeddedResources::sprites_boss3_boss3P2_png, EmbeddedResources::sprites_boss3_boss3P2_png_size};
    _resources["sprites/boss3/boss3P3.png"] = {
        EmbeddedResources::sprites_boss3_boss3P3_png, EmbeddedResources::sprites_boss3_boss3P3_png_size};
    _resources["sprites/boss3/boss3P4.png"] = {
        EmbeddedResources::sprites_boss3_boss3P4_png, EmbeddedResources::sprites_boss3_boss3P4_png_size};
    _resources["sprites/boss3/boss3P5.png"] = {
        EmbeddedResources::sprites_boss3_boss3P5_png, EmbeddedResources::sprites_boss3_boss3P5_png_size};
    _resources["sprites/boss3/boss3P6.png"] = {
        EmbeddedResources::sprites_boss3_boss3P6_png, EmbeddedResources::sprites_boss3_boss3P6_png_size};
    _resources["sprites/boss3/boss3P7.png"] = {
        EmbeddedResources::sprites_boss3_boss3P7_png, EmbeddedResources::sprites_boss3_boss3P7_png_size};
    _resources["sprites/boss3/boss3Tail.png"] = {
        EmbeddedResources::sprites_boss3_boss3Tail_png, EmbeddedResources::sprites_boss3_boss3Tail_png_size};
    _resources["sprites/obstacle.png"] = {
        EmbeddedResources::sprites_obstacle_png, EmbeddedResources::sprites_obstacle_png_size};
    _resources["sprites/power_up.png"] = {
        EmbeddedResources::sprites_power_up_png, EmbeddedResources::sprites_power_up_png_size};
    _resources["sprites/missile_enemy2.png"] = {
        EmbeddedResources::sprites_missile_enemy2_png, EmbeddedResources::sprites_missile_enemy2_png_size};
    _resources["sprites/explose.png"] = {
        EmbeddedResources::sprites_explose_png, EmbeddedResources::sprites_explose_png_size};
    _resources["sprites/obstacle.png"] = {
        EmbeddedResources::sprites_obstacle_png, EmbeddedResources::sprites_obstacle_png_size};
    _resources["sprites/power_up.png"] = {
        EmbeddedResources::sprites_power_up_png, EmbeddedResources::sprites_power_up_png_size};
    _resources["sprites/power_up_attached.png"] = {
        EmbeddedResources::sprites_power_up_attached_png, EmbeddedResources::sprites_power_up_attached_png_size};
    _resources["sprites/power_up2.png"] = {
        EmbeddedResources::sprites_power_up2_png, EmbeddedResources::sprites_power_up2_png_size};
    _resources["sprites/power_up3.png"] = {
        EmbeddedResources::sprites_power_up3_png, EmbeddedResources::sprites_power_up3_png_size};
    _resources["sprites/laser.png"] = {EmbeddedResources::sprites_laser_png, EmbeddedResources::sprites_laser_png_size};
    _resources["sprites/bubble.png"] = {
        EmbeddedResources::sprites_bubble_png, EmbeddedResources::sprites_bubble_png_size};
    _resources["sprites/missile_big.png"] = {
        EmbeddedResources::sprites_missile_big_png, EmbeddedResources::sprites_missile_big_png_size};
    _resources["sprites/bar.png"] = {EmbeddedResources::sprites_bar_png, EmbeddedResources::sprites_bar_png_size};

    _resources["shaders/colorblind.frag"] = {
        EmbeddedResources::shaders_colorblind_frag, EmbeddedResources::shaders_colorblind_frag_size};

    _resources["fonts/font.ttf"] = {EmbeddedResources::fonts_font_ttf, EmbeddedResources::fonts_font_ttf_size};

    _resources["sounds/boss_theme.flac"] = {
        EmbeddedResources::sounds_boss_theme_flac, EmbeddedResources::sounds_boss_theme_flac_size};
    _resources["sounds/explosion.wav"] = {
        EmbeddedResources::sounds_explosion_wav, EmbeddedResources::sounds_explosion_wav_size};
    _resources["sounds/game_theme.flac"] = {
        EmbeddedResources::sounds_game_theme_flac, EmbeddedResources::sounds_game_theme_flac_size};
    _resources["sounds/hit.wav"] = {EmbeddedResources::sounds_hit_wav, EmbeddedResources::sounds_hit_wav_size};
    _resources["sounds/menu_theme.flac"] = {
        EmbeddedResources::sounds_menu_theme_flac, EmbeddedResources::sounds_menu_theme_flac_size};
    _resources["sounds/powerup.wav"] = {
        EmbeddedResources::sounds_powerup_wav, EmbeddedResources::sounds_powerup_wav_size};
    _resources["sounds/shoot.wav"] = {EmbeddedResources::sounds_shoot_wav, EmbeddedResources::sounds_shoot_wav_size};

    _resources["levels/world1/levels.json"] = {
        EmbeddedResources::levels_world1_levels_json, EmbeddedResources::levels_world1_levels_json_size};
    _resources["levels/world2/levels.json"] = {
        EmbeddedResources::levels_world2_levels_json, EmbeddedResources::levels_world2_levels_json_size};
    _resources["levels/world3/levels.json"] = {
        EmbeddedResources::levels_world3_levels_json, EmbeddedResources::levels_world3_levels_json_size};
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