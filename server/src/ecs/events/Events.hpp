/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** eventsPart
*/

#pragma once
#include <cstddef>
#include <cstdint>
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

/**
 * @struct ShootEvent
 * @brief Event triggered when an entity shoots a projectile.
 */
struct ShootEvent {
    float x;                        ///> Starting x position of the projectile
    float y;                        ///> Starting y position of the projectile
    float vx;                       ///> Velocity in the x direction
    float vy;                       ///> Velocity in the y direction
    int damage;                     ///> Damage dealt by the projectile
    size_t shooter;                 ///> ID of the entity that fired the projectile
    std::pair<float, float> bounds; ///> Width and height of the projectile
    float lifetime;                 ///> Lifetime of the projectile in seconds
    unsigned int spriteId;          ///> Sprite ID for rendering the projectile
    int health;                     ///> Health of the projectile
    int maxHealth;                  ///> Max health of the projectile
};

/**
 * @struct DestroyEvent
 * @brief Event triggered when an entity is destroyed.
 */
struct DestroyEvent {
    size_t entityId; ///> ID of the entity to be destroyed
    bool wasKilled;  ///> True if entity was killed (play sound), false otherwise
};

/**
 * @struct UpdateScoreEvent
 * @brief Event triggered to update a player's score.
 */
struct UpdateScoreEvent {
    size_t playerId;         ///> ID of the player whose score is to be updated
    unsigned int scoreDelta; ///> Amount to change the player's score by
};

/**
 * @struct ScoreUpdatedEvent
 * @brief Event triggered when a player's score has been updated.
 */
struct ScoreUpdatedEvent {
    size_t playerId;       ///> ID of the player whose score was updated
    unsigned int newScore; ///> The player's new total score
};

/**
 * @struct PlayerConnectedEvent
 * @brief Event triggered when a player connects to the game.
 */
struct PlayerConnectedEvent {
    int sessionId;      ///> Session identifier of the connected player
    size_t netPlayerId; ///> Network player entity identifier
};

/**
 * @struct DamageApplyEvent
 * @brief Event triggered when damage is applied to an entity.
 */
struct DamageApplyEvent {
    size_t entityId;      ///> ID of the entity that received damage
    uint32_t currentLife; ///> Current life of the entity
    uint32_t maxLife;     ///> Maximum life of the entity
};

/**
 * @struct PowerUpBarCreateEvent
 * @brief Event triggered to create a power-up bar for a player.
 */
struct PowerUpBarCreateEvent {
    size_t playerId; ///> ID of the player entity
    int playerIndex; ///> Index of the player for positioning
};

/**
 * @struct PowerUpBarDestroyEvent
 * @brief Event triggered to destroy a power-up bar for a player.
 */
struct PowerUpBarDestroyEvent {
    size_t playerId; ///> ID of the player entity
};

/**
 * @struct BubblePowerUpCreateEvent
 * @brief Event triggered to create a bubble shield.
 */
struct BubblePowerUpCreateEvent {
    size_t playerId; ///> ID of the player entity
    float playerX;   ///> Player's X position
    float playerY;   ///> Player's Y position
};

/**
 * @struct BubblePowerUpUpdatePosEvent
 * @brief Event triggered to update bubble shield position.
 */
struct BubblePowerUpUpdatePosEvent {
    size_t playerId; ///> ID of the player entity
    float playerX;   ///> Player's X position
    float playerY;   ///> Player's Y position
};

/**
 * @struct BubblePowerUpDestroyEvent
 * @brief Event triggered to destroy a bubble shield.
 */
struct BubblePowerUpDestroyEvent {
    size_t playerId; ///> ID of the player entity
};

/**
 * @struct GameOverEvent
 * @brief Event triggered when the game ends (all players dead or level finished).
 */
struct GameOverEvent {};