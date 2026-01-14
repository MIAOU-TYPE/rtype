/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelSystem
*/

#include "LevelSystem.hpp"

namespace Game
{
    void LevelSystem::update(IGameWorld &world, LevelManager &lvl, const float dt, std::vector<bool> &spawned, const DifficultyModifiers &modifiers)
    {
        lvl.advance(dt);
        handleWaves(world, lvl, spawned, modifiers);
    }

    void LevelSystem::handleWaves(IGameWorld &world, const LevelManager &lvl, std::vector<bool> &spawned, const DifficultyModifiers &modifiers)
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
            spawnWave(world, level, wave, modifiers);
        }
    }

    void LevelSystem::spawnWave(IGameWorld &world, const Level &level, const Wave &wave, const DifficultyModifiers &modifiers)
    {
        for (const auto &[type, count] : wave.groups) {
            if (!level.enemyTypes.contains(type))
                continue;
            const EnemyDefinition &def = level.enemyTypes.at(type);
            for (int k = 0; k < count; k++)
                spawnSingleEnemy(world, def, modifiers);
        }
    }

    void LevelSystem::spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def, const DifficultyModifiers &modifiers)
    {
        auto &reg = world.registry();
        const float y = Rand::enemyY(Rand::rng);
        const Ecs::Entity mob = world.createEntity();

        const int modifiedHp = static_cast<int>(def.hp * modifiers.enemyHpMultiplier);
        reg.emplaceComponent<Ecs::Health>(mob, Ecs::Health{modifiedHp, modifiedHp});

        const int modifiedDamage = static_cast<int>(200 * modifiers.enemyDamageMultiplier);
        reg.emplaceComponent<Ecs::Damage>(mob, Ecs::Damage{modifiedDamage});

        const unsigned int modifiedScore = static_cast<unsigned int>(def.killScore * modifiers.enemyScoreMultiplier);
        reg.emplaceComponent<Ecs::KillScore>(mob, Ecs::KillScore{modifiedScore});

        reg.emplaceComponent<Ecs::Position>(mob, Ecs::Position{1400.f, y});
        reg.emplaceComponent<Ecs::Velocity>(mob, Ecs::Velocity{def.speed * modifiers.enemySpeedMultiplier, 0.f});

        Ecs::MovementPattern pattern;
        pattern.type =
            (def.movement.type == "zigzag") ? Ecs::MovementPattern::Type::ZigZag : Ecs::MovementPattern::Type::Straight;
        pattern.baseVx = def.speed * modifiers.enemySpeedMultiplier;
        pattern.amplitude = def.movement.amplitude;
        pattern.frequency = def.movement.frequency;
        pattern.timer = 0.f;
        reg.emplaceComponent<Ecs::MovementPattern>(mob, pattern);

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
        atk.damage = static_cast<int>(10 * modifiers.enemyDamageMultiplier);
        atk.cooldown = 1.2f;
        atk.projectileSpeed = 320.f * modifiers.projectileSpeedMultiplier;
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
        shoot.projectileSpeed = def.shoot.projectileSpeed * modifiers.projectileSpeedMultiplier;
        shoot.damage = static_cast<int>(def.shoot.damage * modifiers.enemyDamageMultiplier);
        shoot.muzzle = {def.shoot.muzzle.first, def.shoot.muzzle.second};
        shoot.angles = def.shoot.angles;
        reg.emplaceComponent<Ecs::AIShoot>(mob, shoot);

        Ecs::WeaponConfig weapon;
        weapon.projectileSpriteId = def.shoot.projectileSpriteId;
        reg.emplaceComponent<Ecs::WeaponConfig>(mob, weapon);

        reg.emplaceComponent<Ecs::Collision>(mob, Ecs::Collision{def.colW, def.colH});
    }

    void LevelSystem::spawnBackgrounds(IGameWorld &world, const Level &level)
    {
        if (level.backgroundLayers.empty())
            return;

        constexpr float REFERENCE_VIEWPORT_WIDTH = 1280.0f;
        constexpr float REFERENCE_VIEWPORT_HEIGHT = 720.0f;

        for (const auto &layer : level.backgroundLayers) {
            const float scaleX = REFERENCE_VIEWPORT_WIDTH / layer.tileWidth;
            const float scaleY = REFERENCE_VIEWPORT_HEIGHT / layer.tileHeight;
            const float scale = std::max(scaleX, scaleY);

            const float scaledWidth = layer.tileWidth * scale;

            createBackgroundEntity(world, layer, 0.f, scaledWidth, 0);
            createBackgroundEntity(world, layer, scaledWidth, scaledWidth, 1);
        }
    }

    void LevelSystem::createBackgroundEntity(IGameWorld &world, const BackgroundLayer &layer, const float xPosition,
        const float scaledWidth, const int tileIndex)
    {
        auto &reg = world.registry();
        const Ecs::Entity bg = world.createEntity();

        reg.emplaceComponent<Ecs::Position>(bg, Ecs::Position{xPosition, 0.f});

        Ecs::Background bgComp;
        bgComp.scrollSpeed = layer.scrollSpeed;
        bgComp.tileWidth = scaledWidth;
        bgComp.tileHeight = layer.tileHeight;
        bgComp.originalTileWidth = layer.tileWidth;
        bgComp.originalTileHeight = layer.tileHeight;
        bgComp.tileIndex = tileIndex;
        reg.emplaceComponent<Ecs::Background>(bg, bgComp);

        Ecs::Drawable draw;
        draw.spriteId = layer.spriteId;
        draw.drawable = true;
        reg.emplaceComponent<Ecs::Drawable>(bg, draw);
    }
} // namespace Game
