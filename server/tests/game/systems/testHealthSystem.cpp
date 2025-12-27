/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** test_HealthSystem
*/

#include <algorithm>
#include <gtest/gtest.h>
#include <vector>
#include "Events.hpp"
#include "HealthSystem.hpp"
#include "Health.hpp"
#include "Registry.hpp"
#include "mockTestsWorld.hpp"

namespace
{
    static size_t id(Ecs::Entity e)
    {
        return static_cast<size_t>(e);
    }

    static bool containsId(const std::vector<DestroyEvent> &evs, size_t entityId)
    {
        return std::any_of(evs.begin(), evs.end(), [&](const DestroyEvent &ev) {
            return ev.entityId == entityId;
        });
    }
} // namespace

class HealthSystemTests : public ::testing::Test {
  protected:
    ::Test::TestWorld world;
    std::vector<DestroyEvent> destroyed;

    void SetUp() override
    {
        world.events().subscribe<DestroyEvent>([this](const DestroyEvent &ev) {
            destroyed.push_back(ev);
        });
    }

    void run()
    {
        Game::HealthSystem::update(world);
        world.events().process();
    }
};

TEST_F(HealthSystemTests, DoesNotEmitDestroy_WhenHpPositive)
{
    auto &reg = world.registry();

    const Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::Health>(e, Ecs::Health{10, 10});

    run();

    EXPECT_TRUE(destroyed.empty());
}

TEST_F(HealthSystemTests, EmitsDestroy_WhenHpZero)
{
    auto &reg = world.registry();

    const Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::Health>(e, Ecs::Health{0, 10});

    run();

    ASSERT_EQ(destroyed.size(), 1u);

    EXPECT_TRUE(containsId(destroyed, id(e)));
}

TEST_F(HealthSystemTests, EmitsDestroy_WhenHpNegative)
{
    auto &reg = world.registry();

    const Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::Health>(e, Ecs::Health{-5, 10});

    run();

    ASSERT_EQ(destroyed.size(), 1u);
    EXPECT_TRUE(containsId(destroyed, id(e)));
}

TEST_F(HealthSystemTests, EmitsDestroy_ForEachEntityWithHpNonPositive)
{
    auto &reg = world.registry();

    const Ecs::Entity alive = reg.createEntity();
    reg.emplaceComponent<Ecs::Health>(alive, Ecs::Health{3, 10});

    const Ecs::Entity dead1 = reg.createEntity();
    reg.emplaceComponent<Ecs::Health>(dead1, Ecs::Health{0, 10});

    const Ecs::Entity dead2 = reg.createEntity();
    reg.emplaceComponent<Ecs::Health>(dead2, Ecs::Health{-1, 10});

    run();

    ASSERT_EQ(destroyed.size(), 2u);
    EXPECT_TRUE(containsId(destroyed, id(dead1)));
    EXPECT_TRUE(containsId(destroyed, id(dead2)));
    EXPECT_FALSE(containsId(destroyed, id(alive)));
}
