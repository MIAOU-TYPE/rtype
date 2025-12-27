/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** test_ShootingSystem
*/

#include <gtest/gtest.h>
#include <vector>
#include "Events.hpp"
#include "InputComponent.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "ShootingSystem.hpp"
#include "mockTestsWorld.hpp"

class ShootingSystemTests : public ::testing::Test {
  protected:
    ::Test::TestWorld world;
    std::vector<ShootEvent> shoots;

    void SetUp() override
    {
        world.events().subscribe<ShootEvent>([this](const ShootEvent &ev) {
            shoots.push_back(ev);
        });
    }

    void run()
    {
        Game::ShootingSystem::update(world);
        world.events().process();
    }
};

TEST_F(ShootingSystemTests, DoesNotEmit_WhenShootIsFalse)
{
    auto &reg = world.registry();

    const Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Game::InputComponent>(e, Game::InputComponent{});
    reg.emplaceComponent<Ecs::Position>(e, Ecs::Position{100.f, 50.f});

    auto &inputArr = reg.getComponents<Game::InputComponent>();
    ASSERT_TRUE(inputArr.at(static_cast<size_t>(e)).has_value());
    inputArr.at(static_cast<size_t>(e))->shoot = false;

    run();

    EXPECT_TRUE(shoots.empty());
}

TEST_F(ShootingSystemTests, EmitsShootEvent_AndResetsShootFlag)
{
    auto &reg = world.registry();

    const Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Game::InputComponent>(e, Game::InputComponent{});
    reg.emplaceComponent<Ecs::Position>(e, Ecs::Position{100.f, 50.f});

    auto &inputArr = reg.getComponents<Game::InputComponent>();
    ASSERT_TRUE(inputArr.at(static_cast<size_t>(e)).has_value());
    inputArr.at(static_cast<size_t>(e))->shoot = true;

    run();

    ASSERT_EQ(shoots.size(), 1u);

    ASSERT_TRUE(inputArr.at(static_cast<size_t>(e)).has_value());
    EXPECT_FALSE(inputArr.at(static_cast<size_t>(e))->shoot);

    const auto &[x, y, vx, vy, damage, shooter, bounds, lifetime] = shoots.at(0);

    EXPECT_FLOAT_EQ(x, 130.f);
    EXPECT_FLOAT_EQ(y, 50.f);
    EXPECT_FLOAT_EQ(vx, 100.f);
    EXPECT_FLOAT_EQ(vy, 0.f);
    EXPECT_EQ(damage, 20);
    EXPECT_EQ(shooter, static_cast<size_t>(e));

    EXPECT_FLOAT_EQ(bounds.first, 8.f);
    EXPECT_FLOAT_EQ(bounds.second, 8.f);

    EXPECT_FLOAT_EQ(lifetime, 5.f);
}

TEST_F(ShootingSystemTests, MultipleEntities_EmitsForEachShooterThatHasShootTrue)
{
    auto &reg = world.registry();

    const Ecs::Entity e1 = reg.createEntity();
    reg.emplaceComponent<Game::InputComponent>(e1, Game::InputComponent{});
    reg.emplaceComponent<Ecs::Position>(e1, Ecs::Position{10.f, 10.f});

    const Ecs::Entity e2 = reg.createEntity();
    reg.emplaceComponent<Game::InputComponent>(e2, Game::InputComponent{});
    reg.emplaceComponent<Ecs::Position>(e2, Ecs::Position{20.f, 20.f});

    auto &inputArr = reg.getComponents<Game::InputComponent>();
    inputArr.at(static_cast<size_t>(e1))->shoot = true;
    inputArr.at(static_cast<size_t>(e2))->shoot = false;

    run();

    ASSERT_EQ(shoots.size(), 1u);
    EXPECT_EQ(shoots.at(0).shooter, static_cast<size_t>(e1));
    EXPECT_FLOAT_EQ(shoots.at(0).x, 40.f);
    EXPECT_FLOAT_EQ(shoots.at(0).y, 10.f);

    EXPECT_FALSE(inputArr.at(static_cast<size_t>(e1))->shoot);
    EXPECT_FALSE(inputArr.at(static_cast<size_t>(e2))->shoot);
}
