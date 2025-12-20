/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SnapshotSystem tests
*/

#include <gtest/gtest.h>
#include "Drawable.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "SnapshotSystem.hpp"
#include "World.hpp"

using namespace Game;

class SnapshotSystemTest : public ::testing::Test {
  protected:
    std::unique_ptr<Game::World> world;
    std::vector<SnapshotEntity> snapshot;

    void SetUp() override
    {
        world = std::make_unique<Game::World>();
        snapshot.clear();
    }
};

TEST_F(SnapshotSystemTest, EmptyWorldProducesNoSnapshot)
{
    SnapshotSystem::update(*world, snapshot);
    EXPECT_TRUE(snapshot.empty());
}

TEST_F(SnapshotSystemTest, SingleEntityWithDrawableAndPosition)
{
    auto &reg = world->registry();

    auto e = reg.createEntity();
    reg.registerComponent<Ecs::Drawable>();
    reg.registerComponent<Ecs::Position>();

    reg.emplaceComponent<Ecs::Drawable>(e, Ecs::Drawable{"ship.png"});
    reg.emplaceComponent<Ecs::Position>(e, Ecs::Position{42.0f, 84.0f});

    SnapshotSystem::update(*world, snapshot);

    ASSERT_EQ(snapshot.size(), 1u);
    const auto &s = snapshot.front();

    EXPECT_EQ(s.id, static_cast<std::size_t>(e));
    EXPECT_FLOAT_EQ(s.x, 42.0f);
    EXPECT_FLOAT_EQ(s.y, 84.0f);
    EXPECT_EQ(s.sprite, "ship.png");
}

TEST_F(SnapshotSystemTest, EntityWithoutDrawableIsIgnored)
{
    auto &reg = world->registry();

    reg.registerComponent<Ecs::Drawable>();
    reg.registerComponent<Ecs::Position>();

    auto e1 = reg.createEntity();
    reg.emplaceComponent<Ecs::Position>(e1, Ecs::Position{10.0f, 20.0f});

    SnapshotSystem::update(*world, snapshot);
    EXPECT_TRUE(snapshot.empty());
}

TEST_F(SnapshotSystemTest, EntityWithoutPositionIsIgnored)
{
    auto &reg = world->registry();

    reg.registerComponent<Ecs::Drawable>();
    reg.registerComponent<Ecs::Position>();

    auto e1 = reg.createEntity();
    reg.emplaceComponent<Ecs::Drawable>(e1, Ecs::Drawable{"enemy.png"});

    SnapshotSystem::update(*world, snapshot);
    EXPECT_TRUE(snapshot.empty());
}

TEST_F(SnapshotSystemTest, MultipleEntitiesSnapshotCorrectly)
{
    auto &reg = world->registry();

    reg.registerComponent<Ecs::Drawable>();
    reg.registerComponent<Ecs::Position>();

    auto e1 = reg.createEntity();
    reg.emplaceComponent<Ecs::Drawable>(e1, Ecs::Drawable{"enemy.png"});
    reg.emplaceComponent<Ecs::Position>(e1, Ecs::Position{10.0f, 20.0f});

    auto e2 = reg.createEntity();
    reg.emplaceComponent<Ecs::Drawable>(e2, Ecs::Drawable{"bullet.png"});
    reg.emplaceComponent<Ecs::Position>(e2, Ecs::Position{50.0f, 60.0f});

    auto e3 = reg.createEntity(); // incomplete entity
    reg.emplaceComponent<Ecs::Position>(e3, Ecs::Position{0.0f, 0.0f});

    SnapshotSystem::update(*world, snapshot);

    ASSERT_EQ(snapshot.size(), 2u);

    // order is deterministic since Registry stores by index
    EXPECT_EQ(snapshot[0].sprite, "enemy.png");
    EXPECT_FLOAT_EQ(snapshot[0].x, 10.0f);
    EXPECT_FLOAT_EQ(snapshot[0].y, 20.0f);

    EXPECT_EQ(snapshot[1].sprite, "bullet.png");
    EXPECT_FLOAT_EQ(snapshot[1].x, 50.0f);
    EXPECT_FLOAT_EQ(snapshot[1].y, 60.0f);
}
