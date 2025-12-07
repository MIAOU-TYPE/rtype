/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testDLLoader
*/

#include <filesystem>
#include <gtest/gtest.h>
#include "DLLoader.hpp"

using namespace Library;
const std::string libName = "testlib";
const std::string baseDir = "../../";
const std::string libPath = DLLoader::getLibraryPath(libName, baseDir);

TEST(DLLoaderTests, LoadValidLibrary)
{
    DLLoader loader(libName, baseDir);
    EXPECT_TRUE(loader.isLoaded());
    EXPECT_EQ(loader.path(), libPath);
}

TEST(DLLoaderTests, LoadInvalidLibraryShouldThrow)
{
    EXPECT_THROW(DLLoader("nonexistent_library.so", baseDir), DLLoaderError);
}

TEST(DLLoaderTests, GetValidSymbol)
{
    DLLoader loader(libName, baseDir);

    using Func = int (*)();
    Func f = loader.getSymbol<Func>("get_number");

    ASSERT_NE(f, nullptr);
    EXPECT_EQ(f(), 42);
}

TEST(DLLoaderTests, GetInvalidSymbolShouldThrow)
{
    DLLoader loader(libName, baseDir);
    using Func = void (*)();

    EXPECT_THROW(loader.getSymbol<Func>("unknown_symbol"), DLLoaderError);
}

TEST(DLLoaderTests, IsLoadedAfterDestruction)
{
    {
        DLLoader loader(libName, baseDir);
        EXPECT_TRUE(loader.isLoaded());
    }

    SUCCEED();
}
