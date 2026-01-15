/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** mockTestsWorld
*/

#pragma once
#include "IGameWorld.hpp"

namespace Test
{
    class TestWorld final : public Game::IGameWorld {
      public:
        Ecs::Registry &registry() override
        {
            return _reg;
        }

        Ecs::EventsRegistry &events() override
        {
            return _events;
        }

        Ecs::Entity createPlayer(int sessionId) override
        {
            return _reg.createEntity();
        }

        Ecs::Entity createEntity() override
        {
            const auto ent = _reg.createEntity();
            _reg.emplaceComponent<Ecs::Id>(ent, Ecs::Id{static_cast<size_t>(ent)});
            return ent;
        }

        void destroyEntity(const Ecs::Entity) override
        {
        }

        void copyFrom(IGameWorld &) override
        {
        }

      private:
        Ecs::Registry _reg;
        Ecs::EventsRegistry _events;
    };
} // namespace Test
