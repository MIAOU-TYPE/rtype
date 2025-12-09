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

    Ecs::Entity enemy = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(enemy, 100.f, 100.f);
    reg.emplaceComponent<Ecs::Collision>(enemy, 20.f, 20.f);
    reg.emplaceComponent<Ecs::Health>(enemy, 50, 50);

    Ecs::Entity proj = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(proj, 110.f, 110.f);
    reg.emplaceComponent<Ecs::Collision>(proj, 5.f, 5.f);
    reg.emplaceComponent<Ecs::Damage>(proj, 10);
    Game::CollisionSystem::update(world);

    auto &hp = reg.getComponents<Ecs::Health>()[static_cast<size_t>(enemy)];
    ASSERT_TRUE(hp.has_value());
    EXPECT_EQ(hp->hp, 40);
}
