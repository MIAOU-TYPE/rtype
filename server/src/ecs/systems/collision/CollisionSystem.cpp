/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CollisionSystem
*/

#include "CollisionSystem.hpp"

namespace
{
    [[nodiscard]] bool intersects(
        const Ecs::Position &a, const Ecs::Collision &ac, const Ecs::Position &b, const Ecs::Collision &bc)
    {
        return !(a.x > b.x + bc.width || a.x + ac.width < b.x || a.y > b.y + bc.height || a.y + ac.height < b.y);
    }

    [[nodiscard]] bool bothAI(const Ecs::Registry &reg, const size_t a, const size_t b)
    {
        return reg.hasComponent<Ecs::AIBrain>(Ecs::Entity(a)) && reg.hasComponent<Ecs::AIBrain>(Ecs::Entity(b));
    }

    [[nodiscard]] bool sameShooter(Ecs::Registry &reg, const size_t a, const size_t b)
    {
        const auto &shootArr = reg.getComponents<Ecs::Projectile>();
        return shootArr.at(a) && shootArr.at(b) && shootArr.at(a)->shooter == shootArr.at(b)->shooter;
    }

    [[nodiscard]] bool projectileHitsShooter(Ecs::Registry &reg, const size_t projectileIdx, const size_t targetIdx)
    {
        const auto &projectile = reg.getComponents<Ecs::Projectile>().at(projectileIdx);
        return projectile && projectile->shooter == targetIdx;
    }

    [[nodiscard]] bool shootFromAiToAi(Ecs::Registry &reg, const size_t projectileIdx, const size_t targetIdx)
    {
        if (const auto targetIsAi = reg.hasComponent<Ecs::AIBrain>(Ecs::Entity(targetIdx)); !targetIsAi)
            return false;
        const auto &projectile = reg.getComponents<Ecs::Projectile>().at(projectileIdx);
        if (!projectile)
            return false;
        const auto shooterIdx = projectile->shooter;
        return reg.hasComponent<Ecs::AIBrain>(Ecs::Entity(shooterIdx));
    }

    [[nodiscard]] bool isPlayerToPlayerDamage(Ecs::Registry &reg, const size_t projectileIdx, const size_t targetIdx)
    {
        const auto &projectile = reg.getComponents<Ecs::Projectile>().at(projectileIdx);
        if (!projectile)
            return false;

        const size_t shooterIdx = projectile->shooter;
        const bool shooterIsPlayer = reg.hasComponent<Game::InputComponent>(Ecs::Entity(shooterIdx));
        const bool targetIsPlayer = reg.hasComponent<Game::InputComponent>(Ecs::Entity(targetIdx));

        return shooterIsPlayer && targetIsPlayer;
    }

    [[nodiscard]] bool areBothPlayers(Ecs::Registry &reg, const size_t aIdx, const size_t bIdx)
    {
        const bool aIsPlayer = reg.hasComponent<Game::InputComponent>(Ecs::Entity(aIdx));
        const bool bIsPlayer = reg.hasComponent<Game::InputComponent>(Ecs::Entity(bIdx));

        return aIsPlayer && bIsPlayer;
    }
} // namespace

namespace Game
{

    void CollisionSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();
        const auto &gameConfig = world.getGameConfig();

        auto &posArr = reg.getComponents<Ecs::Position>();
        auto &colArr = reg.getComponents<Ecs::Collision>();

        const bool isFriendlyFireMode = (gameConfig.mode == Engine::GameMode::FriendlyFire);

        for (size_t i = 0; i < posArr.size(); i++) {
            const auto &posA = posArr.at(i);
            const auto &colA = colArr.at(i);
            if (!posA || !colA)
                continue;

            for (size_t j = i + 1; j < posArr.size(); j++) {
                const auto &posB = posArr.at(j);
                const auto &colB = colArr.at(j);
                if (!posB || !colB)
                    continue;
                if (!intersects(*posA, *colA, *posB, *colB))
                    continue;
                if (bothAI(reg, i, j))
                    continue;
                if (projectileHitsShooter(reg, i, j) || projectileHitsShooter(reg, j, i))
                    continue;
                if (sameShooter(reg, i, j))
                    continue;
                if (shootFromAiToAi(reg, i, j) || shootFromAiToAi(reg, j, i))
                    continue;
                if (!isFriendlyFireMode) {
                    if (isPlayerToPlayerDamage(reg, i, j) || isPlayerToPlayerDamage(reg, j, i))
                        continue;
                    if (areBothPlayers(reg, i, j))
                        continue;
                }

                world.events().emit(CollisionEvent{i, j});
            }
        }
    }
} // namespace Game
