/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MockWorld
*/

#pragma once
#include "IGameWorld.hpp"
#include "World.hpp"

class MockWorld final : public Game::IGameWorld {
public:
    MockWorld() = default;
    ~MockWorld() override = default;

    Ecs::Registry &registry() override
    {
        return _world.registry();
    }

    Ecs::Entity createPlayer() override
    {
        return _world.createPlayer();
    }

    void destroyEntity(const Ecs::Entity ent) override
    {
        _world.destroyEntity(ent);
    }

    void copyFrom(IGameWorld &other) override
    {
        _world.copyFrom(static_cast<Game::World &>(other));
    }

private:
    Game::World _world;
};
