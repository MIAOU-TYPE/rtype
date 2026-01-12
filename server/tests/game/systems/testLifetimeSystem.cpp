/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** test_LifetimeSystem
*/

#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

#include "Events.hpp"
#include "LifetimeSystem.hpp"

#include "Lifetime.hpp"
#include "Registry.hpp"

#include "mockTestsWorld.hpp"

namespace
{
    static size_t id(Ecs::Entity e)
    {
        return static_cast<size_t>(e);
    }

    static bool containsDestroyed(const std::vector<DestroyEvent> &evs, size_t entityId)
    {
        return std::any_of(evs.begin(), evs.end(), [&](const DestroyEvent &ev) {
            return ev.entityId == entityId;
        });
    }
} // namespace

class LifetimeSystemTests : public ::testing::Test {
  protected:
    ::Test::TestWorld world;
    std::vector<DestroyEvent> destroyed;

    void SetUp() override
    {
        world.events().subscribe<DestroyEvent>([this](const DestroyEvent &ev) {
            destroyed.push_back(ev);
        });
    }

    void run(const float dt)
    {
        Game::LifetimeSystem::update(world, dt);
        world.events().process();
    }
};

TEST_F(LifetimeSystemTests, DecreasesRemainingTime_WhenDtIsPositive)
{
    auto &reg = world.registry();

    const Ecs::Entity e = world.createEntity();
    reg.emplaceComponent<Ecs::Lifetime>(e, Ecs::Lifetime{1.0f});

    run(0.25f);

    auto &lifeArr = reg.getComponents<Ecs::Lifetime>();
    ASSERT_TRUE(lifeArr.at(static_cast<size_t>(e)).has_value());
    EXPECT_FLOAT_EQ(lifeArr.at(static_cast<size_t>(e))->remaining, 0.75f);
    EXPECT_TRUE(destroyed.empty());
}

TEST_F(LifetimeSystemTests, DoesNotEmitDestroy_WhenStillAlive)
{
    auto &reg = world.registry();

    const Ecs::Entity e = world.createEntity();
    reg.emplaceComponent<Ecs::Lifetime>(e, Ecs::Lifetime{2.0f});

    run(0.5f);

    EXPECT_TRUE(destroyed.empty());
}

TEST_F(LifetimeSystemTests, EmitsDestroy_WhenExactlyExpiresToZero)
{
    auto &reg = world.registry();

    const Ecs::Entity e = world.createEntity();
    reg.emplaceComponent<Ecs::Lifetime>(e, Ecs::Lifetime{1.0f});

    run(1.0f);

    ASSERT_EQ(destroyed.size(), 1u);
    EXPECT_TRUE(containsDestroyed(destroyed, id(e)));
}

TEST_F(LifetimeSystemTests, EmitsDestroy_WhenBecomesNegative)
{
    auto &reg = world.registry();

    const Ecs::Entity e = world.createEntity();
    reg.emplaceComponent<Ecs::Lifetime>(e, Ecs::Lifetime{0.1f});

    run(0.5f);

    ASSERT_EQ(destroyed.size(), 1u);
    EXPECT_TRUE(containsDestroyed(destroyed, id(e)));

    auto &lifeArr = reg.getComponents<Ecs::Lifetime>();
    ASSERT_TRUE(lifeArr.at(static_cast<size_t>(e)).has_value());
    EXPECT_LT(lifeArr.at(static_cast<size_t>(e))->remaining, 0.f);
}

TEST_F(LifetimeSystemTests, EmitsDestroy_ForEachExpiredEntityOnly)
{
    auto &reg = world.registry();

    const Ecs::Entity alive = world.createEntity();
    reg.emplaceComponent<Ecs::Lifetime>(alive, Ecs::Lifetime{10.0f});
    reg.emplaceComponent<Ecs::Id>(alive, Ecs::Id{static_cast<size_t>(alive)});

    const Ecs::Entity dead1 = world.createEntity();
    reg.emplaceComponent<Ecs::Lifetime>(dead1, Ecs::Lifetime{0.5f});
    reg.emplaceComponent<Ecs::Id>(dead1, Ecs::Id{static_cast<size_t>(dead1)});

    const Ecs::Entity dead2 = world.createEntity();
    reg.emplaceComponent<Ecs::Lifetime>(dead2, Ecs::Lifetime{0.1f});
    reg.emplaceComponent<Ecs::Id>(dead2, Ecs::Id{static_cast<size_t>(dead2)});

    run(1.0f);

    ASSERT_EQ(destroyed.size(), 2u);
    EXPECT_TRUE(containsDestroyed(destroyed, id(dead1)));
    EXPECT_TRUE(containsDestroyed(destroyed, id(dead2)));
    EXPECT_FALSE(containsDestroyed(destroyed, id(alive)));
}
