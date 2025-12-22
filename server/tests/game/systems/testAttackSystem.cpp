/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testAttackSystem
*/

#include <gtest/gtest.h>

#include "AttackSystem.hpp"
#include "World.hpp"

#include "AIBrain.hpp"
#include "Attack.hpp"
#include "Collision.hpp"
#include "Damage.hpp"
#include "Position.hpp"
#include "Target.hpp"
#include "Velocity.hpp"

using namespace Game;

static Ecs::Entity createEnemyWithAttack(World &world)
{
    auto &reg = world.registry();
    const Ecs::Entity enemy = reg.createEntity();
    const size_t idx = static_cast<size_t>(enemy);

    reg.emplaceComponent<Ecs::Position>(enemy, 0.f, 0.f);

    Ecs::AIBrain brain;
    brain.state = Ecs::AIState::Attack;
    brain.attackCooldown = 0.f;
    reg.emplaceComponent<Ecs::AIBrain>(enemy, brain);

    Ecs::Attack atk;
    atk.damage = 10;
    atk.cooldown = 1.f;
    atk.projectileSpeed = 100.f;
    reg.emplaceComponent<Ecs::Attack>(enemy, atk);

    Ecs::Target tgt;
    tgt.targetId = SIZE_MAX;
    tgt.visionRange = 500.f;
    reg.emplaceComponent<Ecs::Target>(enemy, tgt);

    return enemy;
}

static Ecs::Entity createPlayer(World &world, float x, float y)
{
    auto &reg = world.registry();
    const Ecs::Entity player = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(player, x, y);
    return player;
}

TEST(AttackSystem, shoots_projectile_when_in_attack_state)
{
    World world;
    auto &reg = world.registry();

    const Ecs::Entity enemy = createEnemyWithAttack(world);
    const Ecs::Entity player = createPlayer(world, 100.f, 0.f);

    const size_t enemyIdx = static_cast<size_t>(enemy);
    const size_t playerIdx = static_cast<size_t>(player);

    reg.getComponents<Ecs::Target>().at(enemyIdx)->targetId = playerIdx;

    AttackSystem::update(world, 0.1f);

    auto &velArr = reg.getComponents<Ecs::Velocity>();
    auto &dmgArr = reg.getComponents<Ecs::Damage>();
    auto &colArr = reg.getComponents<Ecs::Collision>();

    bool projectileFound = false;

    for (size_t i = 0; i < velArr.size(); ++i) {
        if (velArr.at(i).has_value() && dmgArr.at(i).has_value() && colArr.at(i).has_value()) {
            projectileFound = true;
            EXPECT_FLOAT_EQ(velArr.at(i)->vx, 100.f);
            EXPECT_FLOAT_EQ(velArr.at(i)->vy, 0.f);
            EXPECT_EQ(dmgArr.at(i)->amount, 10);
        }
    }

    EXPECT_TRUE(projectileFound);
}

TEST(AttackSystem, does_not_shoot_during_cooldown)
{
    World world;
    auto &reg = world.registry();

    const Ecs::Entity enemy = createEnemyWithAttack(world);
    const auto enemyIdx = static_cast<size_t>(enemy);

    reg.getComponents<Ecs::AIBrain>().at(enemyIdx)->attackCooldown = 1.f;

    const Ecs::Entity player = createPlayer(world, 100.f, 0.f);
    reg.getComponents<Ecs::Target>().at(enemyIdx)->targetId = static_cast<size_t>(player);

    AttackSystem::update(world, 0.1f);

    auto &dmgArr = reg.getComponents<Ecs::Damage>();
    for (size_t i = 0; i < dmgArr.size(); ++i)
        EXPECT_FALSE(dmgArr.at(i).has_value());
}

TEST(AttackSystem, does_not_shoot_without_target)
{
    World world;
    auto &reg = world.registry();

    createEnemyWithAttack(world);

    AttackSystem::update(world, 0.1f);

    auto &dmgArr = reg.getComponents<Ecs::Damage>();
    for (size_t i = 0; i < dmgArr.size(); ++i)
        EXPECT_FALSE(dmgArr.at(i).has_value());
}

TEST(AttackSystem, resets_cooldown_after_shooting)
{
    World world;
    auto &reg = world.registry();

    const Ecs::Entity enemy = createEnemyWithAttack(world);
    const Ecs::Entity player = createPlayer(world, 100.f, 0.f);

    const size_t enemyIdx = static_cast<size_t>(enemy);
    const size_t playerIdx = static_cast<size_t>(player);

    reg.getComponents<Ecs::Target>().at(enemyIdx)->targetId = playerIdx;

    AttackSystem::update(world, 0.1f);

    const auto &brain = *reg.getComponents<Ecs::AIBrain>().at(enemyIdx);
    const auto &atk = *reg.getComponents<Ecs::Attack>().at(enemyIdx);

    EXPECT_FLOAT_EQ(brain.attackCooldown, atk.cooldown);
}
