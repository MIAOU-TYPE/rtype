/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameWorld
*/

#pragma once

#include <cstdint>
#include <unordered_map>

#include <iostream>
#include "CommandBuffer.hpp"
#include "Entity.hpp"
#include "EntityCreateData.hpp"
#include "EntityDestroyData.hpp"
#include "Health.hpp"
#include "IClientMessageSink.hpp"
#include "NetworkInterpolation.hpp"
#include "NetworkIdentity.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "SnapEntityData.hpp"
#include "Velocity.hpp"

namespace Game
{
    /**
     * @class GameWorld
     * @brief Manages the game world state on the client side.
     *
     * The GameWorld class is responsible for handling incoming messages from the server,
     * updating the game state, and managing entities within the game world.
     * It implements the IClientMessageSink interface to process network messages.
     */
    class GameWorld : public Ecs::IClientMessageSink {
      public:
        /*
         * @brief Constructor for GameWorld.
         * @param registry Reference to the ECS registry for managing entities and components.
         */
        explicit GameWorld(Ecs::Registry &registry);

        /*
         * @brief Updates the game world state.
         * @param dt Delta time since the last update.
         */
        void update(float dt);

        /*
         * @brief Handles the acceptance message from the server.
         */
        void onAccept() override;

        /*
         * @brief Handles the rejection message from the server.
         */
        void onReject() override;

        /*
         * @brief Handles the pong message from the server.
         */
        void onPong() override;

        /*
         * @brief Handles the game over message from the server.
         */
        void onGameOver() override;

        /*
         * @brief Handles the entity creation message from the server.
         * @param data Data for creating the entity.
         */
        void onEntityCreate(const EntityCreateData &data) override;

        /*
         * @brief Handles the entity destruction message from the server.
         * @param data Data for destroying the entity.
         */
        void onEntityDestroy(const EntityDestroyData &data) override;

        /*
         * @brief Handles the snapshot message from the server.
         * @param data Data for updating the entity snapshot.
         */
        void onSnapshot(const SnapshotEntityData &data) override;

      private:
        enum class CommandType { Accept, Reject, GameOver, CreateEntity, DestroyEntity, Snapshot };

        struct Command {
            CommandType type;
            EntityCreateData create{};
            EntityDestroyData destroy{};
            SnapshotEntityData snapshot{};
        };

        void applyCommand(const Command &cmd);

        Ecs::Registry &_registry;
        ::Command::CommandBuffer<Command> _commandBuffer;
        std::unordered_map<std::uint64_t, Ecs::Entity> _entityMap;
    };
} // namespace Game
