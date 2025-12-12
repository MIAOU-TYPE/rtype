/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntitiesFactory
*/

#pragma once

#include "IEntitiesFactory.hpp"
namespace Ecs
{
    class EntitiesFactory : public IEntitiesFactory {
      public:
        EntitiesFactory(Registry &registry, std::shared_ptr<Graphics::ITextureManager> textureManager);
        ~EntitiesFactory() = default;

        void createEntityFromServer(u_int64_t id, float x, float y, uint16_t sprite) override;
        void destroyEntityFromServer(u_int64_t id) override;
        void updateEntityDamage(u_int32_t id, uint16_t amount) override;
        // void updateEntityPosition(u_int32_t id, float x, float y) override;

        std::optional<Entity> getEntityById(u_int64_t id) const override;

      private:
        Registry &_registry;
        std::shared_ptr<Graphics::ITextureManager> _textureManager;
        std::unordered_map<u_int64_t, Entity> _entityMap;

        void createPlayer(Entity entity, float x, float y, uint16_t sprite);
        void createEnemy(Entity entity, float x, float y, uint16_t sprite);
        void createProjectile(Entity entity, float x, float y, uint16_t sprite);
    };
} // namespace Ecs