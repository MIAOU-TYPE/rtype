/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemySpawnSystem
*/

#include "EnemySpawnSystem.hpp"

float Game::EnemySpawnSystem::_timer = 0.f;

namespace Game
{
    void EnemySpawnSystem::update(IGameWorld &world, float dt)
    {
        _timer += dt;
        if (_timer < 2.f)
            return;
        _timer = 0.f;
        auto &reg = world.registry();
        const Ecs::Entity mob = reg.createEntity();

        float y = 50 + (rand() % 400);
        std::cout << "Spawned enemy: " << static_cast<size_t>(mob) << "\n";

        reg.emplaceComponent<Ecs::Position>(mob, 900.f, y);
        reg.emplaceComponent<Ecs::Velocity>(mob, -80.f, 0.f);
        reg.emplaceComponent<Ecs::Health>(mob, 40, 40);
        reg.emplaceComponent<Ecs::Collision>(mob, 20.f, 20.f);
        reg.emplaceComponent<Ecs::Damageable>(mob, true);
    }
} // namespace Game
