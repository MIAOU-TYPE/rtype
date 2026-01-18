/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** mockTestsWorld
*/

#pragma once
#include "GameConfig.hpp"
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

        Ecs::Entity createPlayer(int sessionId, size_t index) override
        {
            (void) sessionId;
            (void) index;
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

        const Engine::GameConfig &getGameConfig() const override
        {
            return _config;
        }

        void setConfig(const Engine::GameConfig &config)
        {
            _config = config;
        }

      private:
        Ecs::Registry _reg;
        Ecs::EventsRegistry _events;
        Engine::GameConfig _config{
            Engine::Difficulty::Medium, Engine::GameMode::Standard, Engine::ModeParameters{}, ""};
    };
} // namespace Test
