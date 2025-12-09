/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** testSparseArray
*/

#include <gtest/gtest.h>
#include "ecs/core/SparseArray.hpp"

TEST(SparseArray, insert_and_access)
{
    Ecs::SparseArray<int> arr;

    arr.insert(5, 42);

    ASSERT_EQ(arr.size(), 6);
    ASSERT_TRUE(arr[5].has_value());
    ASSERT_EQ(arr[5].value(), 42);
}

TEST(SparseArray, out_of_bounds_is_empty)
{
    Ecs::SparseArray<int> arr;

    ASSERT_EQ(arr.size(), 0);
    ASSERT_FALSE(arr[42].has_value());
}

TEST(SparseArray, remove)
{
    Ecs::SparseArray<int> arr;
    arr.insert(3, 10);

    ASSERT_TRUE(arr[3].has_value());

    arr.remove(3);

    ASSERT_FALSE(arr[3].has_value());
}
