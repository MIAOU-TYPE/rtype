/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelSystem
*/

#include "LevelSystem.hpp"

namespace Game
{
    void LevelSystem::update(IGameWorld &world, LevelManager &lvl, const float dt, std::vector<bool> &spawned)
    {
        lvl.advance(dt);
        handleWaves(world, lvl, spawned);
    }

    void LevelSystem::handleWaves(IGameWorld &world, const LevelManager &lvl, std::vector<bool> &spawned)
    {
        const Level &level = lvl.getCurrentLevel();

        if (spawned.size() != level.waves.size())
            spawned.assign(level.waves.size(), false);

        for (size_t i = 0; i < level.waves.size(); i++) {
            const Wave &wave = level.waves.at(i);
            if (spawned.at(i))
                continue;
            if (!lvl.shouldSpawn(wave.time))
                continue;
            spawned.at(i) = true;
            spawnWave(world, level, wave);
        }
    }

    void LevelSystem::spawnWave(IGameWorld &world, const Level &level, const Wave &wave)
    {
        for (const auto &[type, count] : wave.groups) {
            if (!level.enemyTypes.contains(type))
                continue;
            const EnemyDefinition &def = level.enemyTypes.at(type);
            for (int k = 0; k < count; k++)
                spawnSingleEnemy(world, def);
        }
    }

    void LevelSystem::spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def)
    {
        auto &reg = world.registry();
        const float y = Rand::enemyY(Rand::rng);
        const Ecs::Entity mob = reg.createEntity();

        reg.emplaceComponent<Ecs::Position>(mob, Ecs::Position{900.f, y});
        reg.emplaceComponent<Ecs::Velocity>(mob, Ecs::Velocity{def.speed, 0.f});
        reg.emplaceComponent<Ecs::Health>(mob, Ecs::Health{def.hp, def.hp});
        reg.emplaceComponent<Ecs::Collision>(mob, Ecs::Collision{def.colW, def.colH});
        reg.emplaceComponent<Ecs::Damageable>(mob, Ecs::Damageable{true});
        reg.emplaceComponent<Ecs::Damage>(mob, Ecs::Damage{200});
        reg.emplaceComponent<Ecs::KillScore>(mob, Ecs::KillScore{def.killScore});

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

        Ecs::Drawable draw;
        draw.spriteId = def.sprite;
        draw.drawable = true;
        reg.emplaceComponent<Ecs::Drawable>(mob, draw);

        Ecs::AIShoot shoot;
        shoot.type = def.shoot.type == "straight" ? Ecs::AIShoot::Type::Straight
            : def.shoot.type == "diagonal"        ? Ecs::AIShoot::Type::Diagonal
                                                  : Ecs::AIShoot::Type::Spread;
        shoot.cooldown = def.shoot.cooldown;
        shoot.timer = 0.f;
        shoot.projectileSpeed = def.shoot.projectileSpeed;
        shoot.damage = def.shoot.damage;
        shoot.muzzle = {def.shoot.muzzle.first, def.shoot.muzzle.second};
        shoot.angles = def.shoot.angles;
        reg.emplaceComponent<Ecs::AIShoot>(mob, shoot);
    }

    void LevelSystem::spawnBackgrounds(IGameWorld &world, const Level &level)
    {
        if (level.backgroundLayers.empty())
            return;

        auto &reg = world.registry();

        constexpr float REFERENCE_VIEWPORT_WIDTH = 1280.0f;
        constexpr float REFERENCE_VIEWPORT_HEIGHT = 720.0f;

        for (const auto &layer : level.backgroundLayers) {
            const float scaleX = REFERENCE_VIEWPORT_WIDTH / layer.tileWidth;
            const float scaleY = REFERENCE_VIEWPORT_HEIGHT / layer.tileHeight;
            const float scale = std::max(scaleX, scaleY);

            const float scaledWidth = layer.tileWidth * scale;

            const Ecs::Entity bg0 = reg.createEntity();
            reg.emplaceComponent<Ecs::Position>(bg0, Ecs::Position{0.f, 0.f});

            Ecs::Background bgComp0;
            bgComp0.scrollSpeed = layer.scrollSpeed;
            bgComp0.tileWidth = scaledWidth;
            bgComp0.tileHeight = layer.tileHeight;
            bgComp0.screenWidth = layer.tileWidth;
            bgComp0.screenHeight = layer.tileHeight;
            bgComp0.tileIndex = 0;
            reg.emplaceComponent<Ecs::Background>(bg0, bgComp0);

            Ecs::Drawable draw0;
            draw0.spriteId = layer.spriteId;
            draw0.drawable = true;
            reg.emplaceComponent<Ecs::Drawable>(bg0, draw0);

            const Ecs::Entity bg1 = reg.createEntity();
            reg.emplaceComponent<Ecs::Position>(bg1, Ecs::Position{scaledWidth, 0.f});

            Ecs::Background bgComp1;
            bgComp1.scrollSpeed = layer.scrollSpeed;
            bgComp1.tileWidth = scaledWidth;
            bgComp1.tileHeight = layer.tileHeight;
            bgComp1.screenWidth = layer.tileWidth;
            bgComp1.screenHeight = layer.tileHeight;
            bgComp1.tileIndex = 1;
            reg.emplaceComponent<Ecs::Background>(bg1, bgComp1);

            Ecs::Drawable draw1;
            draw1.spriteId = layer.spriteId;
            draw1.drawable = true;
            reg.emplaceComponent<Ecs::Drawable>(bg1, draw1);
        }
    }
} // namespace Game
