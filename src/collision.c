#include <stdlib.h>
#include "config.h"
#include "types.h"
#include "entities.h"

// Handle player death - reset level
void handle_loss(GameState *game)
{
    clear_bullets(game);
    reset_player(game);

    if (!game->boss.active)
    {
        // Reset enemy formation for regular levels
        init_enemies(game);
        game->enemy_count = game->enemy_cols * game->enemy_rows;
    }
    else
    {
        // Clear boss bullets on boss levels
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            game->boss.bullets[i].active = 0;
        }
    }
}

// Check if two objects are colliding (1-pixel tolerance 2D)
static int check_collision(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1;
}

// player bullets against boss
static void check_player_bullets_vs_boss(GameState *game)
{
    if (!game->boss.active || game->boss.hp <= 0)
        return;

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!game->bullets[i].active)
            continue;

        // Check if bullet hits boss hitbox
        if (game->bullets[i].x >= game->boss.x - (BOSS_WIDTH / 2) &&
            game->bullets[i].x <= game->boss.x + (BOSS_WIDTH / 2) &&
            game->bullets[i].y >= game->boss.y &&
            game->bullets[i].y <= game->boss.y + BOSS_HEIGHT - 1)
        {

            // Bullet hits boss
            game->bullets[i].active = 0;
            game->boss.hp--;
            game->score += SCORE_BOSS_HIT;
            game->boss.flash_timer = BOSS_FLASH_TIME;

            // Check if boss is defeated
            if (game->boss.hp <= 0)
            {
                game->boss.active = 0;
                game->score += SCORE_BOSS_KILL;

                // Clear all boss bullets
                for (int j = 0; j < MAX_BULLETS; j++)
                {
                    game->boss.bullets[j].active = 0;
                }
            }
        }
    }
}

// Player bullets against enemies
static void check_player_bullets_vs_enemies(GameState *game)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!game->bullets[i].active)
            continue;

        for (int j = 0; j < MAX_ENEMIES; j++)
        {
            if (!game->enemies[j].active)
                continue;

            if (check_collision(game->bullets[i].x, game->bullets[i].y,
                                game->enemies[j].x, game->enemies[j].y))
            {

                // Bullet hits enemy
                game->bullets[i].active = 0;
                game->enemies[j].active--;
                game->score += SCORE_ENEMY_HIT;

                // Check if enemy is destroyed
                if (!game->enemies[j].active)
                {
                    game->score += SCORE_ENEMY_KILL;
                    game->enemy_count--;
                }
                break;
            }
        }
    }
}

// Player bullets against boss bullets + deflection bonus
static void check_bullet_deflection(GameState *game)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!game->bullets[i].active)
            continue;

        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (!game->boss.bullets[j].active)
                continue;

            if (check_collision(game->bullets[i].x, game->bullets[i].y,
                                game->boss.bullets[j].x, game->boss.bullets[j].y))
            {

                // Bullets collide and cancel each other
                game->bullets[i].active = 0;
                game->boss.bullets[j].active = 0;
                game->score += SCORE_BULLET_DEFLECT;
                break;
            }
        }
    }
}

// Enemies colliding with player
static void check_enemies_vs_player(GameState *game)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!game->enemies[i].active)
            continue;

        // Check direct collision with player
        if (check_collision(game->player.x, game->player.y,
                            game->enemies[i].x, game->enemies[i].y))
        {

            game->enemies[i].active = 0;
            game->enemy_count--;
            game->player.health--;
            handle_loss(game);
            return;
        }

        // Check if enemies reached the bottom
        if (game->enemies[i].y >= SCREEN_HEIGHT - 3)
        {
            game->enemies[i].active = 0;
            game->enemy_count--;
            game->player.health--;
            handle_loss(game);
            return;
        }
    }
}

// Boss bullets agaisnt player
static void check_boss_bullets_vs_player(GameState *game)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!game->boss.bullets[i].active)
            continue;

        if (check_collision(game->player.x, game->player.y,
                            game->boss.bullets[i].x, game->boss.bullets[i].y))
        {

            // Boss bullet hits player
            game->boss.bullets[i].active = 0;
            game->player.health--;
            handle_loss(game);
            return;
        }
    }
}

// Main collision detection and scoring system
void collision_check(GameState *game)
{
    // Process all collision types in logical order

    // 1. Player offensive actions (bullets hitting targets)
    check_player_bullets_vs_boss(game);
    check_player_bullets_vs_enemies(game);
    check_bullet_deflection(game);

    // 2. Enemy/Boss offensive actions (damage to player)
    check_enemies_vs_player(game);
    check_boss_bullets_vs_player(game);
}