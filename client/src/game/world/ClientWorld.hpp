#pragma once

#include "AnimationState.hpp"
#include "AnimationSystem.hpp"
#include "Drawable.hpp"
#include "IRenderer.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "Render.hpp"
#include "RenderSystem.hpp"
#include "SpriteRegistry.hpp"
#include "WorldCommandBuffer.hpp"
#include <unordered_set>

namespace Engine
{
    /**
     * @class ClientWorld
     * @brief Represents the client-side world, managing entities and their components.
     * Responsible for updating the world state and rendering entities.
     */
    class ClientWorld {
      public:
        /**
         * @brief Constructs a ClientWorld with the given SpriteRegistry.
         * @param spriteRegistry Shared pointer to the SpriteRegistry used for rendering sprites.
         */
        explicit ClientWorld(std::shared_ptr<const SpriteRegistry> spriteRegistry);

        /**
         * @brief Updates the world state and renders entities.
         * @param dt Delta time since the last update.
         */
        void step(float dt);

        /**
         * @brief Provides access to the entity registry.
         * @return Reference to the entity registry.
         */
        Ecs::Registry &registry();

        void applyCommand(const WorldCommand &cmd)
        {
            switch (cmd.type) {
                case WorldCommand::Type::Snapshot:
                    applySnapshot(std::get<std::vector<SnapshotEntity>>(cmd.payload));
                    break;
                default: break;
            }
        }

        void buildRenderCommands(std::vector<RenderCommand> &out) const
        {
            _registry.view<Position, Drawable, AnimationState, Render>(
                [&](Ecs::Entity, const Position &pos, const Drawable &drawable, const AnimationState &anim,
                    const Render &render) {
                    if (!_spriteRegistry->exists(drawable.spriteId))
                        return;

                    if (anim.currentAnimation.empty())
                        return;

                    const SpriteDefinition &sprite = _spriteRegistry->get(drawable.spriteId);

                    const auto it = sprite.animations.find(anim.currentAnimation);
                    if (it == sprite.animations.end())
                        return;

                    const Animation &animation = it->second;
                    if (anim.frameIndex >= animation.frames.size())
                        return;

                    RenderCommand cmd;
                    cmd.textureId = render.texture;
                    cmd.position = {pos.x, pos.y};
                    cmd.frame = animation.frames[anim.frameIndex].rect;

                    out.push_back(cmd);
                });
        }

        void applyCreate(const EntityCreate &data);
        void applySingleSnapshot(const SnapshotEntity &entity);

        void applySnapshot(const std::vector<SnapshotEntity> &entities)
        {
            std::unordered_set<size_t> receivedIds;
            receivedIds.reserve(entities.size());

            for (const auto &entity : entities) {
                receivedIds.insert(entity.id);
                applySingleSnapshot(entity);
            }

            for (auto it = _entityMap.begin(); it != _entityMap.end();) {
                if (!receivedIds.contains(it->first)) {
                    _registry.destroyEntity(it->second);
                    it = _entityMap.erase(it);
                } else {
                    ++it;
                }
            }
        }

        Ecs::Registry _registry; ///> Entity registry managing entities and their components
        std::shared_ptr<const SpriteRegistry>
            _spriteRegistry; ///> Shared pointer to the SpriteRegistry for sprite management

        std::unordered_map<size_t, Ecs::Entity> _entityMap; ///> Maps network entity IDs to local entity IDs

        std::unordered_map<size_t, float> _destroyTimers; ///> Timers for entities scheduled for destruction
        static constexpr float DestroyDelay =
            0.5f; ///> Delay before destroying an entity after receiving a destroy command
    };
} // namespace Engine
