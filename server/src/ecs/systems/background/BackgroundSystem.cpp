/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BackgroundSystem
*/

#include "BackgroundSystem.hpp"

namespace Game
{
    void BackgroundSystem::update(IGameWorld &world, float deltaTime)
    {
        auto &registry = world.registry();

        registry.view<Ecs::Background, Ecs::Position>([&](Ecs::Entity entity, Ecs::Background &bg, Ecs::Position &pos) {
            pos.x += bg.scrollSpeed * deltaTime;
        });
    }

    void BackgroundSystem::resetScroll(IGameWorld &world)
    {
        auto &registry = world.registry();

        registry.view<Ecs::Background, Ecs::Position>([&](Ecs::Entity entity, Ecs::Background &bg, Ecs::Position &pos) {
            // Si la tuile est complètement sortie à gauche de l'écran
            if (pos.x + bg.screenWidth < 0) {
                std::cout << "[BackgroundSystem] Tile at x=" << pos.x 
                          << " went offscreen, repositioning to x=" << (pos.x + bg.screenWidth * 2) << "\n";
                pos.x += bg.screenWidth * 2;
            }
        });
    }
} // namespace Game