/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testCollisionSystem
*/

#include <gtest/gtest.h>
#include "Events.hpp"
#include "EventsRegistry.hpp"
#include "IGameWorld.hpp"
#include "Registry.hpp"

#include <algorithm>
#include <vector>

#include "CollisionSystem.hpp"

#include "AIBrain.hpp"
#include "Collision.hpp"
#include "Position.hpp"
#include "Projectile.hpp"
#include "mockTestsWorld.hpp"

namespace
{
    Ecs::Entity makeEntity(Test::TestWorld &world, float x, float y, float w, float h)
    {
        auto &reg = world.registry();
        const Ecs::Entity e = reg.createEntity();
        reg.emplaceComponent<Ecs::Position>(e, Ecs::Position{x, y});
        reg.emplaceComponent<Ecs::Collision>(e, Ecs::Collision{w, h});
        return e;
    }

    void addAI(Test::TestWorld &world, Ecs::Entity e)
    {
        world.registry().emplaceComponent<Ecs::AIBrain>(e, Ecs::AIBrain{});
    }

    void addProjectile(Test::TestWorld &world, Ecs::Entity e, size_t shooterId)
    {
        world.registry().emplaceComponent<Ecs::Projectile>(e, Ecs::Projectile{shooterId});
    }

    size_t id(Ecs::Entity e)
    {
        return static_cast<size_t>(e);
    }

    bool containsPair(const std::vector<CollisionEvent> &evs, size_t a, size_t b)
    {
        return std::any_of(evs.begin(), evs.end(), [&](const CollisionEvent &ev) {
            return (ev.a == a && ev.b == b) || (ev.a == b && ev.b == a);
        });
    }
} // namespace

class CollisionSystemEmitTests : public ::testing::Test {
  protected:
    ::Test::TestWorld world;
    std::vector<CollisionEvent> emitted;

    void SetUp() override
    {
        world.events().subscribe<CollisionEvent>([this](const CollisionEvent &ev) {
            emitted.push_back(ev);
        });
    }

    void run()
    {
        Game::CollisionSystem::update(world);
        world.events().process();
    }
};

TEST_F(CollisionSystemEmitTests, EmitsCollision_WhenOverlapAndNoFilters)
{
    const auto a = makeEntity(world, 0.f, 0.f, 10.f, 10.f);
    const auto b = makeEntity(world, 5.f, 5.f, 10.f, 10.f); // overlap

    run();

    ASSERT_FALSE(emitted.empty());
    EXPECT_TRUE(containsPair(emitted, id(a), id(b)));
}

TEST_F(CollisionSystemEmitTests, DoesNotEmit_WhenNoIntersection)
{
    (void) makeEntity(world, 0.f, 0.f, 10.f, 10.f);
    (void) makeEntity(world, 100.f, 100.f, 10.f, 10.f);

    run();

    EXPECT_TRUE(emitted.empty());
}

TEST_F(CollisionSystemEmitTests, DoesNotEmit_WhenBothAreAI)
{
    const auto a = makeEntity(world, 0.f, 0.f, 10.f, 10.f);
    const auto b = makeEntity(world, 5.f, 5.f, 10.f, 10.f);

    addAI(world, a);
    addAI(world, b);

    run();

    EXPECT_TRUE(emitted.empty());
}

TEST_F(CollisionSystemEmitTests, DoesNotEmit_WhenProjectileHitsItsShooter)
{
    const auto shooter = makeEntity(world, 0.f, 0.f, 10.f, 10.f);
    const auto proj = makeEntity(world, 5.f, 5.f, 10.f, 10.f); // overlap shooter

    addProjectile(world, proj, id(shooter)); // projectile->shooter == target => skip

    run();

    EXPECT_TRUE(emitted.empty());
}

TEST_F(CollisionSystemEmitTests, DoesNotEmit_WhenTwoProjectilesHaveSameShooter)
{
    const auto shooter = makeEntity(world, 50.f, 50.f, 10.f, 10.f);

    const auto p1 = makeEntity(world, 0.f, 0.f, 10.f, 10.f);
    const auto p2 = makeEntity(world, 5.f, 5.f, 10.f, 10.f); // overlap p1

    addProjectile(world, p1, id(shooter));
    addProjectile(world, p2, id(shooter)); // same shooter => skip

    run();

    EXPECT_TRUE(emitted.empty());
}

TEST_F(CollisionSystemEmitTests, EmitsCollision_WhenTwoProjectilesHaveDifferentShooters)
{
    const auto shooterA = makeEntity(world, 100.f, 100.f, 10.f, 10.f);
    const auto shooterB = makeEntity(world, 200.f, 200.f, 10.f, 10.f);

    const auto pA = makeEntity(world, 0.f, 0.f, 10.f, 10.f);
    const auto pB = makeEntity(world, 5.f, 5.f, 10.f, 10.f); // overlap

    addProjectile(world, pA, id(shooterA));
    addProjectile(world, pB, id(shooterB)); // different shooter => event

    run();

    ASSERT_FALSE(emitted.empty());
    EXPECT_TRUE(containsPair(emitted, id(pA), id(pB)));
}

TEST_F(CollisionSystemEmitTests, EmitsForEachPair_AmongThreeOverlappingEntities)
{
    const auto e0 = makeEntity(world, 0.f, 0.f, 10.f, 10.f);
    const auto e1 = makeEntity(world, 2.f, 2.f, 10.f, 10.f);
    const auto e2 = makeEntity(world, 4.f, 4.f, 10.f, 10.f);

    run();

    EXPECT_TRUE(containsPair(emitted, id(e0), id(e1)));
    EXPECT_TRUE(containsPair(emitted, id(e0), id(e2)));
    EXPECT_TRUE(containsPair(emitted, id(e1), id(e2)));
}
