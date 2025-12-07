/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testInputSystem
*/

#include <gtest/gtest.h>
#include "game/systems/input/InputSystem.hpp"
#include "game/world/World.hpp"

TEST(InputSystem, input_sets_velocity)
{
    Game::World world;
    auto e = world.createPlayer();

    auto &reg = world.registry();

    auto &input = reg.getComponents<Game::InputComponent>()[static_cast<size_t>(e)];
    input->right = true;
    input->down  = true;

    Game::InputSystem::update(world);

    auto &vel = reg.getComponents<Ecs::Velocity>()[static_cast<size_t>(e)];

    ASSERT_EQ(vel->vx, 200.f);
    ASSERT_EQ(vel->vy, 200.f);
}

TEST(InputSystem, diagonal_movement)
{
    Game::World world;
    auto e = world.createPlayer();

    auto &reg = world.registry();

    auto &input = reg.getComponents<Game::InputComponent>()[static_cast<size_t>(e)];
    input->left = true;
    input->up = true;

    Game::InputSystem::update(world);

    auto &vel = reg.getComponents<Ecs::Velocity>()[static_cast<size_t>(e)];

    ASSERT_EQ(vel->vx, -200.f);
    ASSERT_EQ(vel->vy, -200.f);
}
