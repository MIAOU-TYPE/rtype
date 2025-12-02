# R-Type -- Game Design Document

## 1. Global Gameplay (Game Mechanics)

-   Constant horizontal scrolling.
-   The player moves their ship freely in all four directions.
-   The player can shoot projectiles.
-   Enemies appear on the right and move toward the left.
-   Some enemies shoot projectiles.
-   Complete collision management:
    -   Player / Enemy
    -   Player / Enemy projectile
    -   Player projectile / Enemy
-   When an enemy is destroyed, it may drop a bonus.
-   The game continues as long as the player has not lost all their lives.
-   Up to **4 players** can play simultaneously in the same game.
-   Purely gameplay description, independent of code.

------------------------------------------------------------------------

## 2. Main Entity Roles

### Player

-   Entity controlled by a human.
-   Can move up, down, left, right.
-   Can shoot projectiles.
-   Has:
    -   position,
    -   speed,
    -   health points.
-   Visually identifiable (sprite, color).
-   Sends inputs to the server.

### Enemy

-   Entity controlled by server logic.
-   Appears on the right, at the top or bottom of the screen.
-   Follows a simple or advanced movement pattern.
-   Can shoot at regular intervals.
-   Has a certain number of health points.
-   May give a bonus/power-up upon destruction.

### Player Projectile

-   Shot by the player.
-   Moves to the right.
-   Deals damage to enemies.
-   Disappears on impact.

### Enemy Projectile

-   Shot by enemies.
-   Moves to the left (angle 0° to -180°).
-   Kills or damages the player on impact.
-   Disappears on collision.

### Obstacles

-   Static entities.
-   Can block mobile entities.
-   Some can be destructible, others cannot.

------------------------------------------------------------------------

## 3. Summary

**Global Mechanics:**\
Horizontal shoot'em up based on dodging, shooting and constant movement in a scrolling environment. Enemies and projectiles move from the right toward the players.

**Entity Roles:**\
- *Player*: mobile, shoots, has health points.\
- *Enemy*: autonomous, follows a pattern, shoots, can die.\
- *Player Projectile*: offensive shot toward the right.\
- *Enemy Projectile*: shot toward the left, dangerous.\
- *Obstacles*: static elements potentially destructible.
