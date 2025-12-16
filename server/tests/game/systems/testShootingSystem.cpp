/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testShootingSystem
*/

#include <gtest/gtest.h>
#include "ecs/components/Collision.hpp"
#include "ecs/components/Damage.hpp"
#include "ecs/components/InputComponent.hpp"
#include "ecs/components/Position.hpp"
#include "ecs/components/Velocity.hpp"
#include "game/systems/shooting/ShootingSystem.hpp"
#include "game/world/World.hpp"

TEST(ShootingSystem, creates_projectile_when_shooting)
{
    Game::World world;
    Ecs::Entity player = world.createPlayer();

    auto &reg = world.registry();
    auto &input = reg.getComponents<Game::InputComponent>()[static_cast<size_t>(player)];
    auto &pos = reg.getComponents<Ecs::Position>()[static_cast<size_t>(player)];

    ASSERT_TRUE(input.has_value());
    ASSERT_TRUE(pos.has_value());

    input->shoot = true;

    Game::ShootingSystem::update(world);

    auto &velArr = reg.getComponents<Ecs::Velocity>();
    auto &dmgArr = reg.getComponents<Ecs::Damage>();
    auto &colArr = reg.getComponents<Ecs::Collision>();

    bool foundProjectile = false;

    for (size_t i = 0; i < velArr.size(); i++) {
        if (velArr[i].has_value() && dmgArr[i].has_value() && colArr[i].has_value()) {
            foundProjectile = true;

            EXPECT_FLOAT_EQ(velArr[i]->vx, 15.0f);
            EXPECT_FLOAT_EQ(velArr[i]->vy, 0.f);
            EXPECT_EQ(dmgArr[i]->amount, 20);
        }
    }

    EXPECT_TRUE(foundProjectile);
}

TEST(ShootingSystem, resets_shoot_flag)
{
    Game::World world;
    Ecs::Entity player = world.createPlayer();

    auto &reg = world.registry();
    auto &input = reg.getComponents<Game::InputComponent>()[static_cast<size_t>(player)];

    ASSERT_TRUE(input.has_value());

    input->shoot = true;

    Game::ShootingSystem::update(world);

    EXPECT_FALSE(input->shoot);
}
