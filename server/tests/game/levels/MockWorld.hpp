/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MockWorld
*/

#pragma once
#include "IGameWorld.hpp"
#include "Registry.hpp"

class MockWorld : public Game::IGameWorld {
public:
    Ecs::Registry reg;

    Ecs::Registry &registry() override { return reg; }
};
