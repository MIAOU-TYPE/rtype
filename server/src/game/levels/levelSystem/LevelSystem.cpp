/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelSystem
*/

#include "LevelSystem.hpp"

#include "Damage.hpp"

namespace Game
{
    std::vector<bool> LevelSystem::_spawned;

    void LevelSystem::update(IGameWorld &world, LevelManager &lvl, const float dt)
    {
        lvl.advance(dt);
        handleWaves(world, lvl);
    }

    void LevelSystem::handleWaves(IGameWorld &world, const LevelManager &lvl)
    {
        const Level &level = lvl.getCurrentLevel();

        if (_spawned.size() != level.waves.size())
            _spawned.assign(level.waves.size(), false);

        for (size_t i = 0; i < level.waves.size(); i++) {
            const Wave &wave = level.waves.at(i);
            if (_spawned.at(i))
                continue;
            if (!lvl.shouldSpawn(wave.time))
                continue;
            _spawned.at(i) = true;
            spawnWave(world, level, wave);
        }
    }

    void LevelSystem::spawnWave(IGameWorld &world, const Level &level, const Wave &wave)
    {
        for (const WaveEnemyGroup &group : wave.groups) {
            if (!level.enemyTypes.contains(group.type))
                continue;
            const EnemyDefinition &def = level.enemyTypes.at(group.type);
            for (int k = 0; k < group.count; k++)
                spawnSingleEnemy(world, def);
        }
    }

    void LevelSystem::spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def)
    {
        auto &reg = world.registry();
        float y = Rand::enemyY(Rand::rng);
        const Ecs::Entity mob = reg.createEntity();

        reg.emplaceComponent<Ecs::Position>(mob, 900.f, y);
        reg.emplaceComponent<Ecs::Velocity>(mob, def.speed, 0.f);
        reg.emplaceComponent<Ecs::Health>(mob, def.hp, def.hp);
        reg.emplaceComponent<Ecs::Collision>(mob, def.colW, def.colH);
        reg.emplaceComponent<Ecs::Damageable>(mob, true);
        reg.emplaceComponent<Ecs::Damage>(mob, 200);

        Ecs::AIBrain brain;
        brain.state = Ecs::AIState::Patrol;
        brain.timer = 0.f;
        brain.attackCooldown = 0.f;
        reg.emplaceComponent<Ecs::AIBrain>(mob, brain);

        Ecs::Target tgt;
        tgt.targetId = SIZE_MAX;
        tgt.visionRange = 350.f;
        reg.emplaceComponent<Ecs::Target>(mob, tgt);

        Ecs::Attack atk;
        atk.damage = 10;
        atk.cooldown = 1.2f;
        atk.projectileSpeed = 320.f;
        reg.emplaceComponent<Ecs::Attack>(mob, atk);

        reg.emplaceComponent<Ecs::Damageable>(mob, true);

        Ecs::Drawable draw;
        draw.spriteId = def.sprite;
        draw.drawable = true;
        reg.emplaceComponent<Ecs::Drawable>(mob, draw);
    }
} // namespace Game
