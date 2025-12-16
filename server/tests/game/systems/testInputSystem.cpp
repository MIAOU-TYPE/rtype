/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testInputSystem
*/

#include <gtest/gtest.h>
#include "InputSystem.hpp"
#include "World.hpp"

TEST(InputSystem, input_dont_affect_velocity)
{
    Game::World world;
    auto e = world.createPlayer();

    auto &reg = world.registry();

    auto &input = reg.getComponents<Game::InputComponent>()[static_cast<size_t>(e)];
    input->right = true;
    input->down = true;

    Game::InputSystem::update(world);

    auto &vel = reg.getComponents<Ecs::Velocity>()[static_cast<size_t>(e)];

    ASSERT_EQ(vel->vx, 0.f);
    ASSERT_EQ(vel->vy, 0.f);
}

TEST(InputSystem, diagonal_movement)
{
    Game::World world;
    auto e = world.createPlayer();

    auto &reg = world.registry();

    auto beforePos = reg.getComponents<Ecs::Position>()[static_cast<size_t>(e)];

    auto &input = reg.getComponents<Game::InputComponent>()[static_cast<size_t>(e)];
    input->left = true;
    input->up = true;

    Game::InputSystem::update(world);

    auto &pos = reg.getComponents<Ecs::Position>()[static_cast<size_t>(e)];

    ASSERT_EQ(pos->x, (beforePos->x - 7.f));
    ASSERT_EQ(pos->y, (beforePos->y - 7.f));
}
