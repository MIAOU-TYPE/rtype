/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelSystem
*/

#include "LevelSystem.hpp"

namespace
{
    [[nodiscard]] std::vector<float> calculateSpawnPositions(
        const std::string &pattern, const float centerY, const int count)
    {
        std::vector<float> positions;
        positions.reserve(static_cast<size_t>(count));

        if (pattern == "line") {
            constexpr float spacing = 80.f;
            float startY = centerY - (spacing * static_cast<float>(count - 1) / 2.f);
            for (int i = 0; i < count; i++)
                positions.push_back(startY + static_cast<float>(i) * spacing);
        } else if (pattern == "spread") {
            constexpr float minY = 50.f;
            constexpr float maxY = 600.f;

            if (count <= 0)
                return positions;
            if (count == 1)
                positions.push_back((minY + maxY) / 2.f);
            else {
                float step = (maxY - minY) / static_cast<float>(count - 1);
                for (int i = 0; i < count; i++)
                    positions.push_back(minY + static_cast<float>(i) * step);
            }
        } else {
            for (int i = 0; i < count; i++)
                positions.push_back(Rand::enemyY(Rand::rng));
        }

        return positions;
    }
} // namespace

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

            if (def.isGroup) {
                for (int k = 0; k < count; k++)
                    spawnEnemyGroup(world, level, def, wave.spawnPattern, wave.spawnY);
            } else {
                std::vector<float> yPositions = calculateSpawnPositions(wave.spawnPattern, wave.spawnY, count);
                for (int k = 0; k < count; k++)
                    spawnSingleEnemy(world, def, 1400.f, yPositions[static_cast<size_t>(k)]);
            }
        }
        if (!wave.obstacleType.empty() && level.obstacleTypes.contains(wave.obstacleType)) {
            const ObstacleDefinition &obsDef = level.obstacleTypes.at(wave.obstacleType);
            spawnObstacle(world, obsDef, wave.obstacleX, wave.obstacleY);
        }
    }

    void LevelSystem::spawnEnemyGroup(IGameWorld &world, const Level &level, const EnemyDefinition &groupDef,
        const std::string &pattern, const float centerY)
    {
        std::vector<float> basePositions = calculateSpawnPositions(pattern, centerY, 1);

        if (basePositions.empty())
            return;

        const float baseY = basePositions.at(0);
        const float baseX = 1400.f;

        for (const auto &member : groupDef.members) {
            if (!level.enemyTypes.contains(member.enemyType))
                continue;

            const EnemyDefinition &memberDef = level.enemyTypes.at(member.enemyType);
            const float x = baseX + member.offsetX;
            const float y = baseY + member.offsetY;

            spawnSingleEnemy(world, memberDef, x, y);
        }
    }

    void LevelSystem::spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def, const float x, const float y)
    {
        auto &reg = world.registry();
        const Ecs::Entity mob = world.createEntity();

        reg.emplaceComponent<Ecs::Position>(mob, Ecs::Position{x, y, 2});
        reg.emplaceComponent<Ecs::Velocity>(mob, Ecs::Velocity{def.speed, 0.f});

        if (def.sprite == 1 || def.sprite == 21 || def.sprite == 22)
            reg.emplaceComponent<Ecs::BossPhase>(mob, Ecs::BossPhase{});

        Ecs::MovementPattern pattern;
        pattern.type =
            (def.movement.type == "zigzag") ? Ecs::MovementPattern::Type::ZigZag : Ecs::MovementPattern::Type::Straight;
        pattern.baseVx = def.speed;
        pattern.amplitude = def.movement.amplitude;
        pattern.frequency = def.movement.frequency;
        pattern.timer = 0.f;
        reg.emplaceComponent<Ecs::MovementPattern>(mob, pattern);

        reg.emplaceComponent<Ecs::Health>(mob, Ecs::Health{def.hp, def.hp});
        reg.emplaceComponent<Ecs::Collision>(
            mob, Ecs::Collision{def.colW * COLLISION_SCALE, def.colH * COLLISION_SCALE});
        reg.emplaceComponent<Ecs::Damageable>(mob, Ecs::Damageable{true});
        reg.emplaceComponent<Ecs::Damage>(mob, Ecs::Damage{50});
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

        Ecs::Drawable draw;
        draw.spriteId = def.sprite;
        draw.drawable = true;
        reg.emplaceComponent<Ecs::Drawable>(mob, draw);

        Ecs::AIShoot shoot;
        shoot.type = def.shoot.type == "straight" ? Ecs::AIShoot::Type::Straight
            : def.shoot.type == "diagonal"        ? Ecs::AIShoot::Type::Diagonal
            : def.shoot.type == "homing"          ? Ecs::AIShoot::Type::Homing
                                                  : Ecs::AIShoot::Type::Spread;
        shoot.cooldown = def.shoot.cooldown;
        shoot.timer = 0.f;
        shoot.projectileSpeed = def.shoot.projectileSpeed;
        shoot.damage = def.shoot.damage;
        shoot.muzzle = {def.shoot.muzzle.first, def.shoot.muzzle.second};
        shoot.angles = def.shoot.angles;
        shoot.bulletsNbr = def.shoot.bulletsNbr;
        reg.emplaceComponent<Ecs::AIShoot>(mob, shoot);

        Ecs::WeaponConfig weapon;
        weapon.projectileSpriteId = def.shoot.projectileSpriteId;
        reg.emplaceComponent<Ecs::WeaponConfig>(mob, weapon);
    }

    void LevelSystem::spawnObstacle(IGameWorld &world, const ObstacleDefinition &def, const float x, const float y)
    {
        auto &reg = world.registry();
        const Ecs::Entity obstacle = world.createEntity();

        reg.emplaceComponent<Ecs::Position>(obstacle, Ecs::Position{x, y, 1});
        reg.emplaceComponent<Ecs::Velocity>(obstacle, Ecs::Velocity{0.f, 0.f});
        reg.emplaceComponent<Ecs::GravityField>(
            obstacle, Ecs::GravityField{def.pullStrength, def.damagePerSecond, def.radius, def.innerRadius});
        reg.emplaceComponent<Ecs::Drawable>(obstacle, Ecs::Drawable{def.sprite, true});
        reg.emplaceComponent<Ecs::Collision>(
            obstacle, Ecs::Collision{def.colW * COLLISION_SCALE, def.colH * COLLISION_SCALE});
    }

    void LevelSystem::spawnBackgrounds(IGameWorld &world, const Level &level)
    {
        if (level.backgroundLayers.empty())
            return;

        constexpr float REFERENCE_VIEWPORT_WIDTH = Game::Config::VIEWPORT_WIDTH;
        constexpr float REFERENCE_VIEWPORT_HEIGHT = Game::Config::VIEWPORT_HEIGHT;

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

        reg.emplaceComponent<Ecs::Position>(bg, Ecs::Position{xPosition, 0.f, 0});

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
