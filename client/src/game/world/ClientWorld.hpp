#pragma once

#include "AnimationState.hpp"
#include "AnimationSystem.hpp"
#include "Drawable.hpp"
#include "IRenderer.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "Render.hpp"
#include "RenderCommandBuffer.hpp"
#include "RenderSystem.hpp"
#include "SpriteRegistry.hpp"
#include "WorldCommandBuffer.hpp"

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
                case WorldCommand::Type::CreateEntity: applyCreate(std::get<EntityCreate>(cmd.payload)); break;
                case WorldCommand::Type::DestroyEntity: applyDestroy(std::get<EntityDestroy>(cmd.payload)); break;
                case WorldCommand::Type::Snapshot: applySnapshot(std::get<SnapshotEntity>(cmd.payload)); break;
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

      private:
        void applyCreate(const EntityCreate &data);
        void applyDestroy(const EntityDestroy &data);
        void applySnapshot(const SnapshotEntity &entity);

        Ecs::Registry _registry; ///> Entity registry managing entities and their components
        std::shared_ptr<const SpriteRegistry>
            _spriteRegistry; ///> Shared pointer to the SpriteRegistry for sprite management

        std::unordered_map<size_t, Ecs::Entity> _entityMap; ///> Maps network entity IDs to local entity IDs
    };
} // namespace Engine
