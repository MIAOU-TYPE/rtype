/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** IEntitiesFactory
*/

#pragma once

#include "ITextureManager.hpp"
#include "Registry.hpp"

namespace Ecs
{
    class IEntitiesFactory {
      public:
        virtual ~IEntitiesFactory();

        virtual void createEntityFromServer(u_int64_t id, float x, float y, uint16_t sprite) = 0;
        virtual void destroyEntityFromServer(u_int64_t id) = 0;
        virtual void updateEntityDamage(u_int32_t id, uint16_t amount) = 0;
        // virtual void updateEntityPosition(u_int32_t id, float x, float y) = 0;

        virtual std::optional<Entity> getEntityById(u_int64_t id) const = 0;
    };
} // namespace Ecs