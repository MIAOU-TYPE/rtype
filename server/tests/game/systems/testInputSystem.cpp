/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testInputSystem
*/

#include <gtest/gtest.h>
#include <numbers>
#include "InputSystem.hpp"
#include "World.hpp"

TEST(InputSystem, input_dont_affect_velocity)
{
    Game::World world;
    auto e = world.createPlayer(1);

    auto &reg = world.registry();

    auto &input = reg.getComponents<Game::InputComponent>().at(static_cast<size_t>(e));
    input->right = true;
    input->down = true;

    Game::InputSystem::update(world);

    auto &vel = reg.getComponents<Ecs::Velocity>().at(static_cast<size_t>(e));

    ASSERT_EQ(vel->vx, 0.f);
    ASSERT_EQ(vel->vy, 0.f);
}

TEST(InputSystem, diagonal_movement)
{
    Game::World world;
    auto e = world.createPlayer(1);

    auto &reg = world.registry();

    auto beforePos = reg.getComponents<Ecs::Position>().at(static_cast<size_t>(e));

    auto &input = reg.getComponents<Game::InputComponent>().at(static_cast<size_t>(e));
    input->left = true;
    input->up = true;

    Game::InputSystem::update(world);

    auto &pos = reg.getComponents<Ecs::Position>().at(static_cast<size_t>(e));

    // With diagonal normalization: speed * (1/sqrt(2)) = 3.5 * 0.7071... ≈ 2.475
    constexpr float expectedDelta = 3.5f / std::numbers::sqrt2_v<float>;
    ASSERT_NEAR(pos->x, beforePos->x - expectedDelta, 0.001f);
    ASSERT_NEAR(pos->y, beforePos->y - expectedDelta, 0.001f);
}
