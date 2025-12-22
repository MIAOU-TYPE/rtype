/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testSnapshotSystem
*/

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../../server/src/ecs/systems/snapshot/SnapshotSystem.hpp"

TEST(SnapshotSystemTests, EmptyWorldProducesEmptySnapshot)
{
    Game::SnapshotSystem sys;
    Game::World world;
    std::vector<SnapshotEntity> snapshot;

    sys.update(world, snapshot);

    EXPECT_TRUE(snapshot.empty());
}

TEST(SnapshotSystemTests, SingleEntitySnapshotIsCorrect)
{
    Game::SnapshotSystem sys;
    Game::World world;
    std::vector<SnapshotEntity> snapshot;

    auto drawable = world.registry().registerComponent<Ecs::Drawable>();
    auto position = world.registry().registerComponent<Ecs::Position>();

    const auto entity = world.registry().createEntity();
    auto fakeEntity = world.registry().createEntity();
    world.registry().emplaceComponent<Ecs::Drawable>(entity, Ecs::Drawable{1});
    world.registry().emplaceComponent<Ecs::Position>(entity, Ecs::Position{100.f, 200.f});

    sys.update(world, snapshot);
    ASSERT_EQ(snapshot.size(), 1);
    EXPECT_EQ(snapshot[0].spriteId, 1);
    EXPECT_EQ(snapshot[0].x, 100.f);
    EXPECT_EQ(snapshot[0].y, 200.f);
}

TEST(SnapshotSystemTests, MultipleEntitiesAreCapturedInOrder)
{
    Game::SnapshotSystem sys;
    Game::World world;
    std::vector<SnapshotEntity> snapshot;

    auto drawable = world.registry().registerComponent<Ecs::Drawable>();
    auto position = world.registry().registerComponent<Ecs::Position>();

    const auto entity1 = world.registry().createEntity();
    world.registry().emplaceComponent<Ecs::Drawable>(entity1, Ecs::Drawable{3});
    world.registry().emplaceComponent<Ecs::Position>(entity1, Ecs::Position{10.f, 20.f});

    const auto entityUnused = world.registry().createEntity();

    const auto entity2 = world.registry().createEntity();
    world.registry().emplaceComponent<Ecs::Drawable>(entity2, Ecs::Drawable{5});
    world.registry().emplaceComponent<Ecs::Position>(entity2, Ecs::Position{30.f, 40.f});

    sys.update(world, snapshot);

    ASSERT_EQ(snapshot.size(), 2);
    EXPECT_EQ(snapshot[0].id, static_cast<size_t>(entity1));
    EXPECT_EQ(snapshot[0].spriteId, 3);
    EXPECT_EQ(snapshot[0].x, 10.f);
    EXPECT_EQ(snapshot[0].y, 20.f);

    EXPECT_EQ(snapshot[1].id, static_cast<size_t>(entity2));
    EXPECT_EQ(snapshot[1].spriteId, 5);
    EXPECT_EQ(snapshot[1].x, 30.f);
    EXPECT_EQ(snapshot[1].y, 40.f);
}

TEST(SnapshotSystemTests, SnapshotIsClearedBeforeWriting)
{
    Game::SnapshotSystem sys;
    Game::World world;
    std::vector<SnapshotEntity> snapshot;

    auto drawable = world.registry().registerComponent<Ecs::Drawable>();
    auto position = world.registry().registerComponent<Ecs::Position>();
    const auto entity = world.registry().createEntity();
    world.registry().emplaceComponent<Ecs::Drawable>(entity, Ecs::Drawable{2});
    world.registry().emplaceComponent<Ecs::Position>(entity, Ecs::Position{50.f, 60.f});
    sys.update(world, snapshot);

    ASSERT_EQ(snapshot.size(), 1);

    world.registry().destroyEntity(entity);
    sys.update(world, snapshot);
    ASSERT_EQ(snapshot.size(), 0);
}
