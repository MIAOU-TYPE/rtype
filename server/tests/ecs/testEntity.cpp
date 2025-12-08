/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** testEntity
*/

#include <gtest/gtest.h>
#include "ecs/core/Entity.hpp"

TEST(Entity, default_constructor)
{
    Ecs::Entity e;
    ASSERT_EQ(static_cast<size_t>(e), 0);
}

TEST(Entity, custom_id)
{
    Ecs::Entity e(42);
    ASSERT_EQ(static_cast<size_t>(e), 42);
}