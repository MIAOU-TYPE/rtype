/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderCommandBuffer
*/

#pragma once
#include <mutex>
#include <queue>
#include <variant>
#include "RenderCommand.hpp"

struct RenderFrame {
    std::vector<Engine::RenderCommand> commands;
};
