/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** eventsPart
*/

#pragma once
#include <cstddef>
#include <utility>

/**
 * @struct CollisionEvent
 * @brief Event triggered when a collision occurs between two entities.
 */
struct CollisionEvent {
    size_t a; ///> ID of the first entity involved in the collision
    size_t b; ///> ID of the second entity involved in the collision
};

/**
 * @struct DamageEvent
 * @brief Event triggered when an entity deals damage to another entity.
 */
struct DamageEvent {
    size_t source; ///> ID of the entity dealing damage
    size_t target; ///> ID of the entity receiving damage
    int amount;    ///> Amount of damage dealt
};

struct ShootEvent {
    float x;                          ///> Starting x position of the projectile
    float y;                          ///> Starting y position of the projectile
    float vx;                         ///> Velocity in the x direction
    float vy;                         ///> Velocity in the y direction
    int damage;                       ///> Damage dealt by the projectile
    size_t shooter;                   ///> ID of the entity that fired the projectile
    std::pair<size_t, size_t> bounds; ///> Width and height of the projectile
    float lifetime;                   ///> Lifetime of the projectile in seconds
};

/**
 * @struct DestroyEvent
 * @brief Event triggered when an entity is destroyed.
 */
struct DestroyEvent {
    size_t entityId; ///> ID of the entity to be destroyed
};

/**
 * @struct UpdateScoreEvent
 * @brief Event triggered to update a player's score.
 */
struct UpdateScoreEvent {
    size_t playerId;         ///> ID of the player whose score is to be updated
    unsigned int scoreDelta; ///> Amount to change the player's score by
};