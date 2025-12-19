/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** testRegistry
*/

#include <gtest/gtest.h>
#include "Health.hpp"
#include "Position.hpp"
#include "Velocity.hpp"
#include "Registry.hpp"

TEST(Registry, create_entities)
{
    Ecs::Registry registry;

    auto e1 = registry.createEntity();
    auto e2 = registry.createEntity();

    ASSERT_EQ(static_cast<size_t>(e1), 0);
    ASSERT_EQ(static_cast<size_t>(e2), 1);
}

TEST(Registry, emplace_and_has)
{
    Ecs::Registry registry;
    auto entity = registry.createEntity();

    registry.emplaceComponent<Ecs::Position>(entity, 10.f, 20.f);

    ASSERT_TRUE(registry.hasComponent<Ecs::Position>(entity));
    ASSERT_FALSE(registry.hasComponent<Ecs::Velocity>(entity));
}

TEST(Registry, destroy_entity)
{
    Ecs::Registry registry;
    auto entity = registry.createEntity();

    registry.emplaceComponent<Ecs::Health>(entity, 100);

    ASSERT_TRUE(registry.hasComponent<Ecs::Health>(entity));

    registry.destroyEntity(entity);

    ASSERT_FALSE(registry.hasComponent<Ecs::Health>(entity));
}

TEST(Registry, view_position_velocity)
{
    Ecs::Registry registry;

    auto e1 = registry.createEntity();
    auto e2 = registry.createEntity();
    auto e3 = registry.createEntity();

    registry.emplaceComponent<Ecs::Position>(e1, 1.f, 1.f);
    registry.emplaceComponent<Ecs::Velocity>(e1, 1.f, 0.f);

    registry.emplaceComponent<Ecs::Position>(e2, 2.f, 2.f);

    registry.emplaceComponent<Ecs::Position>(e3, 3.f, 3.f);
    registry.emplaceComponent<Ecs::Velocity>(e3, 0.f, 1.f);

    int processed = 0;

    registry.view<Ecs::Position, Ecs::Velocity>([&](Ecs::Entity, Ecs::Position &pos, Ecs::Velocity &vel) {
        pos.x += vel.vx;
        pos.y += vel.vy;
        processed++;
    });

    ASSERT_EQ(processed, 2);

    auto &pos = registry.getComponents<Ecs::Position>();

    ASSERT_EQ(pos[static_cast<size_t>(e1)]->x, 2.f);
    ASSERT_EQ(pos[static_cast<size_t>(e3)]->y, 4.f);
}
