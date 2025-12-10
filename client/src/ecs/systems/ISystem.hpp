/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ISystem
*/

#pragma once

namespace Ecs
{
    class ISystem {
      public:
        virtual ~ISystem() = default;

        virtual void update(float deltaTime) = 0;
    };
} // namespace Ecs
