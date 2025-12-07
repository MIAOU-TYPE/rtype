/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testWorld
*/

#include <gtest/gtest.h>
#include "game/world/World.hpp"
#include "ecs/components/Position.hpp"
#include "ecs/components/Velocity.hpp"
#include "ecs/components/Health.hpp"
#include "ecs/components/InputComponent.hpp"

TEST(World, create_player_adds_all_components)
{
    Game::World world;

    Ecs::Entity e = world.createPlayer();

    auto &reg = world.registry();

    ASSERT_TRUE(reg.hasComponent<Ecs::Position>(e));
    ASSERT_TRUE(reg.hasComponent<Ecs::Velocity>(e));
    ASSERT_TRUE(reg.hasComponent<Ecs::Health>(e));
    ASSERT_TRUE(reg.hasComponent<Game::InputComponent>(e));

    auto &pos = reg.getComponents<Ecs::Position>();
    ASSERT_EQ(pos[(size_t)e]->x, 100.f);
    ASSERT_EQ(pos[(size_t)e]->y, 100.f);
}

TEST(World, destroy_player_removes_components)
{
    Game::World world;
    auto e = world.createPlayer();
    auto &reg = world.registry();

    ASSERT_TRUE(reg.hasComponent<Ecs::Health>(e));
    world.destroyEntity(e);
    ASSERT_FALSE(reg.hasComponent<Ecs::Health>(e));
}
