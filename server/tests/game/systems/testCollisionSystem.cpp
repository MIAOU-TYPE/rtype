/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testCollisionSystem
*/

#include <gtest/gtest.h>
#include "game/systems/collision/CollisionSystem.hpp"
#include "game/world/World.hpp"
#include "ecs/components/Position.hpp"
#include "ecs/components/Collision.hpp"
#include "ecs/components/Damage.hpp"
#include "ecs/components/Health.hpp"

TEST(CollisionSystem, projectile_hits_enemy_and_deals_damage)
{
    Game::World world;
    auto &reg = world.registry();

    const Ecs::Entity enemy = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(enemy, 100.f, 100.f);
    reg.emplaceComponent<Ecs::Collision>(enemy, 20.f, 20.f);
    reg.emplaceComponent<Ecs::Health>(enemy, 50, 50);

    const Ecs::Entity proj = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(proj, 110.f, 110.f);
    reg.emplaceComponent<Ecs::Collision>(proj, 5.f, 5.f);
    reg.emplaceComponent<Ecs::Damage>(proj, 10);
    Game::CollisionSystem::update(world);

    auto &hp = reg.getComponents<Ecs::Health>()[static_cast<size_t>(enemy)];
    ASSERT_TRUE(hp.has_value());
    EXPECT_EQ(hp->hp, 40);
}

TEST(CollisionSystem, collision_works_even_if_projectile_has_lower_id)
{
    Game::World world;
    auto &reg = world.registry();

    const Ecs::Entity proj = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(proj, 110.f, 110.f);
    reg.emplaceComponent<Ecs::Collision>(proj, 5.f, 5.f);
    reg.emplaceComponent<Ecs::Damage>(proj, 10);

    const Ecs::Entity enemy = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(enemy, 100.f, 100.f);
    reg.emplaceComponent<Ecs::Collision>(enemy, 20.f, 20.f);
    reg.emplaceComponent<Ecs::Health>(enemy, 50, 50);

    Game::CollisionSystem::update(world);

    auto &hp = reg.getComponents<Ecs::Health>()[static_cast<size_t>(enemy)];
    ASSERT_TRUE(hp.has_value());
    EXPECT_EQ(hp->hp, 40);
}

TEST(CollisionSystem, projectile_is_destroyed_after_collision)
{
    Game::World world;
    auto &reg = world.registry();

    const Ecs::Entity enemy = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(enemy, 100.f, 100.f);
    reg.emplaceComponent<Ecs::Collision>(enemy, 20.f, 20.f);
    reg.emplaceComponent<Ecs::Health>(enemy, 50, 50);

    const Ecs::Entity proj = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(proj, 110.f, 110.f);
    reg.emplaceComponent<Ecs::Collision>(proj, 5.f, 5.f);
    reg.emplaceComponent<Ecs::Damage>(proj, 10);

    Game::CollisionSystem::update(world);

    auto &dmg = reg.getComponents<Ecs::Damage>()[static_cast<size_t>(proj)];
    EXPECT_FALSE(dmg.has_value());
}

TEST(CollisionSystem, no_collision_no_damage)
{
    Game::World world;
    auto &reg = world.registry();

    const Ecs::Entity enemy = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(enemy, 100.f, 100.f);
    reg.emplaceComponent<Ecs::Collision>(enemy, 20.f, 20.f);
    reg.emplaceComponent<Ecs::Health>(enemy, 50, 50);

    const Ecs::Entity proj = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(proj, 500.f, 500.f);
    reg.emplaceComponent<Ecs::Collision>(proj, 5.f, 5.f);
    reg.emplaceComponent<Ecs::Damage>(proj, 10);

    Game::CollisionSystem::update(world);

    auto &hp = reg.getComponents<Ecs::Health>()[static_cast<size_t>(enemy)];
    ASSERT_TRUE(hp.has_value());
    EXPECT_EQ(hp->hp, 50);
}

TEST(CollisionSystem, projectile_can_hit_multiple_enemies_same_frame)
{
    Game::World world;
    auto &reg = world.registry();

    const Ecs::Entity e1 = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(e1, 100.f, 100.f);
    reg.emplaceComponent<Ecs::Collision>(e1, 20.f, 20.f);
    reg.emplaceComponent<Ecs::Health>(e1, 50, 50);

    const Ecs::Entity e2 = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(e2, 110.f, 110.f);
    reg.emplaceComponent<Ecs::Collision>(e2, 20.f, 20.f);
    reg.emplaceComponent<Ecs::Health>(e2, 50, 50);

    const Ecs::Entity proj = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(proj, 115.f, 115.f);
    reg.emplaceComponent<Ecs::Collision>(proj, 20.f, 20.f);
    reg.emplaceComponent<Ecs::Damage>(proj, 10);

    Game::CollisionSystem::update(world);

    EXPECT_EQ(reg.getComponents<Ecs::Health>()[static_cast<size_t>(e1)]->hp, 40);
    EXPECT_EQ(reg.getComponents<Ecs::Health>()[static_cast<size_t>(e2)]->hp, 40);
}
