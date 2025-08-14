#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include "config.h"
#include "types.h"
#include "entities.h"

// Fire a bullet from player position
void shoot_bullet(GameState *game)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!game->bullets[i].active)
        {
            game->bullets[i].x = game->player.x;
            game->bullets[i].y = game->player.y - 1;
            game->bullets[i].active = 1;
            game->bullets_fired_this_level++;
            break;
        }
    }
}

// Move all active player bullets upward
void update_bullets(GameState *game)
{
    if (++game->bullet_move_delay < BULLET_MOVE_DELAY)
        return;
    game->bullet_move_delay = 0;

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (game->bullets[i].active)
        {
            game->bullets[i].y--;
            if (game->bullets[i].y < 2) // Remove if off-screen
                game->bullets[i].active = 0;
        }
    }
}

// Initialize enemy formation for current level
void init_enemies(GameState *game)
{
    int idx = 0;
    int total_w = game->enemy_cols * ENEMY_SPACING;
    int start_x = (SCREEN_WIDTH - total_w) / 2;

    for (int row = 0; row < game->enemy_rows; row++)
    {
        for (int col = 0; col < game->enemy_cols; col++, idx++)
        {
            game->enemies[idx].x = start_x + col * ENEMY_SPACING;
            game->enemies[idx].y = row * 2 + 2;
            game->enemies[idx].active = game->enemy_hp;
        }
    }
}

// Update enemy movement (Horizontal movement and Vertical downward movment with edge collision)
void update_enemies(GameState *game)
{
    if (++game->enemy_move_delay < ENEMY_MOVE_DELAY)
        return;
    game->enemy_move_delay = 0;

    // Check if any enemy hits screen edge
    int move_down = 0;
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (game->enemies[i].active &&
            ((game->enemies[i].x <= 4 && game->enemy_direction == -1) ||
             (game->enemies[i].x >= SCREEN_WIDTH - 6 && game->enemy_direction == 1)))
        {
            move_down = 1;
            break;
        }
    }

    // Move all enemies
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!game->enemies[i].active)
            continue;
        if (move_down)
            game->enemies[i].y++;
        else
            game->enemies[i].x += game->enemy_direction;
    }

    if (move_down)
        game->enemy_direction *= -1; // Reverse direction when enemy moves downward
}

// Deactivate all player bullets
void clear_bullets(GameState *game)
{
    for (int i = 0; i < MAX_BULLETS; i++)
        game->bullets[i].active = 0;
}

// Reset player to starting position
void reset_player(GameState *game)
{
    game->player.x = SCREEN_WIDTH / 2;
    game->player.y = SCREEN_HEIGHT - 4;
}

// Spawn a boss bullet at given position
static void spawn_boss_bullet(Boss *boss, int x, int y)
{
    // Limit active boss bullets
    int bullet_count = 0;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (boss->bullets[i].active)
            bullet_count++;
    }
    if (bullet_count >= MAX_BULLETS - 1)
        return;

    // Find empty slot and spawn bullet
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!boss->bullets[i].active)
        {
            boss->bullets[i].x = x;
            boss->bullets[i].y = y;
            boss->bullets[i].active = 1;
            break;
        }
    }
}

// Boss spread attack with 80% homing chance
static void boss_shoot_spread(GameState *game)
{
    Boss *boss = &game->boss;
    int target_x = game->player.x;

    // 80% chance to predict player movement
    if (rand() % 10 < 8)
    {
        if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000)
        {
            target_x = game->player.x - 2;
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000)
        {
            target_x = game->player.x + 2;
        }
    }

    // Fire 5-bullet spread pattern
    int spread[5] = {target_x - 2, target_x - 1, target_x, target_x + 1, target_x + 2};
    for (int i = 0; i < 5; i++)
    {
        if (spread[i] >= 3 && spread[i] <= SCREEN_WIDTH - 4)
        {
            spawn_boss_bullet(boss, spread[i], boss->y + BOSS_HEIGHT);
        }
    }
}

// Boss wall attack - wide bullet pattern
static void boss_shoot_wall(GameState *game)
{
    Boss *boss = &game->boss;
    int wall_width = 8;
    int start_x = boss->x - (wall_width / 2);

    for (int i = 0; i < wall_width; i++)
    {
        int bullet_x = start_x + i;
        if (bullet_x >= 3 && bullet_x <= SCREEN_WIDTH - 4)
        {
            spawn_boss_bullet(boss, bullet_x, boss->y + BOSS_HEIGHT);
        }
    }
}

// Boss wave attack - clean and smooth vertical wobble
static void boss_shoot_wave(GameState *game)
{
    Boss *boss = &game->boss;
    static int wave_offset = 0; // persists between calls

    wave_offset += 10; // advances the wave

    int bullet_count = 7;       // number of bullets
    int horizontal_spacing = 5; // space between bullets
    int vertical_amplitude = 3; // vertical wobble size (increasing range from -1 to 1)

    for (int i = 0; i < bullet_count; i++)
    {
        // X position = evenly spaced around the boss
        int bullet_x = boss->x - (bullet_count / 2) * horizontal_spacing + i * horizontal_spacing;

        // Vertical wobble using sine for smooth up/down movement
        int bullet_y = boss->y + BOSS_HEIGHT + (int)(sin((wave_offset + i * 30) * 3.14159 / 180.0) * vertical_amplitude);

        // Spawn bullet if within screen bounds
        if (bullet_x >= 3 && bullet_x <= SCREEN_WIDTH - 4)
        {
            spawn_boss_bullet(boss, bullet_x, bullet_y);
        }
    }
}

// Update boss bullet movement
void update_boss_bullets(GameState *game)
{
    Boss *boss = &game->boss;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!boss->bullets[i].active)
            continue;

        boss->bullets[i].y += BOSS_BULLET_SPEED;
        if (boss->bullets[i].y >= SCREEN_HEIGHT - 3)
            boss->bullets[i].active = 0;
    }
}

// Update boss movement and attacks
void update_boss(GameState *game)
{
    Boss *boss = &game->boss;
    if (!boss->active || boss->hp <= 0)
        return;

    // Handle pause between movements
    if (boss->pause_counter > 0)
    {
        boss->pause_counter--;
    }

    // Boss movement logic
    if (++boss->move_delay >= BOSS_MOVE_DELAY)
    {
        boss->move_delay = 0;

        if (boss->pause_counter <= 0)
        {
            boss->x += boss->move_direction;
            boss->move_counter++;

            // Check for direction change
            if (boss->move_counter >= BOSS_MOVE_DISTANCE ||
                boss->x <= 8 || boss->x >= SCREEN_WIDTH - 8)
            {
                boss->pause_counter = BOSS_PAUSE_TIME;
                boss->move_direction *= -1;
                boss->move_counter = 0;
                boss->attack_pattern = (boss->attack_pattern + 1) % 3; // Cycle patterns
            }
        }
    }

    // Boss shooting logic with difficulty scaling
    int shoot_delay = BOSS_SHOOT_DELAY;
    if (boss->hp <= boss->max_hp / 4)
    {
        shoot_delay = BOSS_SHOOT_DELAY * 2 / 3;
    }
    else if (boss->hp <= boss->max_hp / 2)
    {
        shoot_delay = BOSS_SHOOT_DELAY * 4 / 5;
    }

    if (++boss->shoot_delay >= shoot_delay)
    {
        boss->shoot_delay = 0;

        // Execute current attack pattern
        switch (boss->attack_pattern)
        {
        case 0:
            boss_shoot_spread(game);
            break;
        case 1:
            boss_shoot_wall(game);
            break;
        case 2:
            boss_shoot_wave(game);
            break;
        }
    }

    // Update flash timer for hit feedback
    if (boss->flash_timer > 0)
        boss->flash_timer--;
}
