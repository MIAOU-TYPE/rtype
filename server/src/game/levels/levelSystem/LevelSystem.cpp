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
    void LevelSystem::update(IGameWorld &world, LevelManager &lvl, const float dt, std::vector<bool> &spawned,
        const DifficultyModifiers &modifiers)
    {
        lvl.advance(dt);
        handleWaves(world, lvl, spawned, modifiers);
    }

    void LevelSystem::handleWaves(
        IGameWorld &world, const LevelManager &lvl, std::vector<bool> &spawned, const DifficultyModifiers &modifiers)
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

    void LevelSystem::spawnWave(
        IGameWorld &world, const Level &level, const Wave &wave, const DifficultyModifiers &modifiers)
    {
        for (const auto &[type, count] : wave.groups) {
            if (!level.enemyTypes.contains(type))
                continue;
            const EnemyDefinition &def = level.enemyTypes.at(type);

            if (def.isGroup) {
                for (int k = 0; k < count; k++)
                    spawnEnemyGroup(world, level, def, wave.spawnPattern, wave.spawnY, modifiers);
            } else {
                std::vector<float> yPositions = calculateSpawnPositions(wave.spawnPattern, wave.spawnY, count);
                for (int k = 0; k < count; k++)
                    spawnSingleEnemy(world, def, 1400.f, yPositions[static_cast<size_t>(k)], modifiers);
            }
        }
        if (!wave.obstacleType.empty() && level.obstacleTypes.contains(wave.obstacleType)) {
            const ObstacleDefinition &obsDef = level.obstacleTypes.at(wave.obstacleType);
            spawnObstacle(world, obsDef, wave.obstacleX, wave.obstacleY);
        }

        for (int i = 0; i < wave.powerUps; i++)
            spawnPowerUp(world, wave.powerUpType);
    }

    void LevelSystem::spawnEnemyGroup(IGameWorld &world, const Level &level, const EnemyDefinition &groupDef,
        const std::string &pattern, const float centerY, const DifficultyModifiers &modifiers)
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

            spawnSingleEnemy(world, memberDef, x, y, modifiers);
        }
    }

    void LevelSystem::spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def, const float x, const float y,
        const DifficultyModifiers &modifiers)
    {
        auto &reg = world.registry();
        const Ecs::Entity mob = world.createEntity();

        const int modifiedHp = static_cast<int>(static_cast<float>(def.hp) * modifiers.enemyHpMultiplier);
        reg.emplaceComponent<Ecs::Health>(mob, Ecs::Health{modifiedHp, modifiedHp});

        const int modifiedDamage = static_cast<int>(static_cast<float>(def.damage) * modifiers.enemyDamageMultiplier);
        reg.emplaceComponent<Ecs::Damage>(mob, Ecs::Damage{modifiedDamage});

        const unsigned int modifiedScore =
            static_cast<unsigned int>(static_cast<float>(def.killScore) * modifiers.enemyScoreMultiplier);
        reg.emplaceComponent<Ecs::KillScore>(mob, Ecs::KillScore{modifiedScore});

        reg.emplaceComponent<Ecs::Position>(mob, Ecs::Position{x, y, 2});
        reg.emplaceComponent<Ecs::Velocity>(mob, Ecs::Velocity{def.speed * modifiers.enemySpeedMultiplier, 0.f});

        handleBossPhases(world, def, mob, x, y);

        Ecs::MovementPattern pattern;
        pattern.type =
            (def.movement.type == "zigzag") ? Ecs::MovementPattern::Type::ZigZag : Ecs::MovementPattern::Type::Straight;
        pattern.baseVx = def.speed * modifiers.enemySpeedMultiplier;
        pattern.amplitude = def.movement.amplitude;
        pattern.frequency = def.movement.frequency;
        pattern.timer = 0.f;
        reg.emplaceComponent<Ecs::MovementPattern>(mob, pattern);

        reg.emplaceComponent<Ecs::Collision>(
            mob, Ecs::Collision{def.colW * COLLISION_SCALE, def.colH * COLLISION_SCALE});
        reg.emplaceComponent<Ecs::Damageable>(mob, Ecs::Damageable{true});

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
        shoot.projectileSpeed = def.shoot.projectileSpeed * modifiers.projectileSpeedMultiplier;
        shoot.damage = static_cast<int>(static_cast<float>(def.shoot.damage) * modifiers.enemyDamageMultiplier);
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

    void LevelSystem::handleBossPhases(
        IGameWorld &world, const EnemyDefinition &def, const Ecs::Entity mob, const float x, const float y)
    {
        auto &reg = world.registry();

        if (def.sprite == 1) {
            Ecs::BossPhase bossConfig;
            bossConfig.phases = {{2500, {-15.f, 0.f, 15.f}, 1.2f, 1, 0},
                {1500, {-30.f, -15.f, 0.f, 15.f, 30.f}, 1.5f, 2, 0},
                {500, {-45.f, -30.f, -15.f, 0.f, 15.f, 30.f, 45.f}, 1.5f, 2, 0}};
            reg.emplaceComponent<Ecs::BossPhase>(mob, bossConfig);
        } else if (def.sprite == 21) {
            Ecs::BossPhase bossConfig;
            bossConfig.phases = {{2500, {}, 1.f, 1, 0}, {1500, {}, 1.3f, 3, 0}, {500, {}, 1.5f, 3, 0}};
            reg.emplaceComponent<Ecs::BossPhase>(mob, bossConfig);
        } else if (def.sprite == 22) {
            Ecs::BossPhase bossConfig;
            bossConfig.phases = {{4998, {}, 1.f, 1, 22}, {4284, {}, 1.f, 1, 23}, {3570, {}, 1.2f, 2, 24},
                {2856, {}, 1.5f, 2, 25}, {2142, {}, 1.5f, 3, 26}, {1428, {}, 1.7f, 3, 27}, {714, {}, 2.f, 4, 28}

            };
            reg.emplaceComponent<Ecs::BossPhase>(mob, bossConfig);

            Ecs::Entity previousEntity = mob;
            const int numTailSegments = 8;
            float tailStartX = x + 90.f;
            float tailStartY = y + 300.f;
            for (int i = 0; i < numTailSegments; ++i) {
                const Ecs::Entity tailSegment = world.createEntity();
                float offsetX = -50.f * (static_cast<float>(i) + 1);
                float offsetY = 50.f * (static_cast<float>(i) + 1);
                reg.emplaceComponent<Ecs::Position>(
                    tailSegment, Ecs::Position{tailStartX + offsetX, tailStartY + offsetY, 2});
                reg.emplaceComponent<Ecs::Velocity>(tailSegment, Ecs::Velocity{def.speed, 0.f});
                Ecs::Drawable draw;
                draw.spriteId = 30;
                draw.drawable = true;
                reg.emplaceComponent<Ecs::Drawable>(tailSegment, draw);
                Ecs::TailFollower follower;
                follower.bossEntity = mob;
                follower.leaderEntity = previousEntity;
                follower.followSpeed = 80.f;
                follower.followDistance = 40.f;
                reg.emplaceComponent<Ecs::TailFollower>(tailSegment, follower);

                previousEntity = tailSegment;
            }
        }
    }

    void LevelSystem::spawnPowerUp(IGameWorld &world, const std::string &type)
    {
        auto &reg = world.registry();
        const float y = Rand::enemyY(Rand::rng);
        const Ecs::Entity powerUpEntity = world.createEntity();

        Ecs::PowerUpTypeEnum powerUpType = Ecs::PowerUpTypeEnum::Standard;
        uint32_t spriteId = 13u;

        if (type == "laser") {
            powerUpType = Ecs::PowerUpTypeEnum::Laser;
            spriteId = 18u;
        } else if (type == "shield") {
            powerUpType = Ecs::PowerUpTypeEnum::Shield;
            spriteId = 19u;
        }

        reg.emplaceComponent<Ecs::Position>(powerUpEntity, Ecs::Position{1400.f, y});
        reg.emplaceComponent<Ecs::Velocity>(powerUpEntity, Ecs::Velocity{-50.f, 0.f});
        reg.emplaceComponent<Ecs::Drawable>(powerUpEntity, Ecs::Drawable{spriteId, true});
        reg.emplaceComponent<Ecs::Collision>(powerUpEntity, Ecs::Collision{32.f, 32.f});
        reg.emplaceComponent<Ecs::PowerUp>(powerUpEntity);
        reg.emplaceComponent<Ecs::PowerUpType>(powerUpEntity, Ecs::PowerUpType{powerUpType});
    }
} // namespace Game
