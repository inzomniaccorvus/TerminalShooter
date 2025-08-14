#include "config.h"
#include "types.h"
#include "entities.h"
#include "levels.h"

// Configure level based on current level number
void configure_level(GameState *game)
{
    if (game->current_level == 10)
    {
        // Level 10 - Boss fight
        game->enemy_count = 0;
        game->boss.active = 1;
        game->boss.hp = game->boss.max_hp = BOSS_MAX_HP;
        game->boss.x = SCREEN_WIDTH / 2;
        game->boss.y = 5;

        // Reset boss timers and values
        game->boss.move_delay = game->boss.shoot_delay = 0;
        game->boss.move_direction = 1;
        game->boss.move_counter = game->boss.pause_counter = 0;
        game->boss.attack_pattern = game->boss.flash_timer = 0;

        // Clear all boss bullets
        for (int i = 0; i < MAX_BULLETS; i++)
            game->boss.bullets[i].active = 0;
        return;
    }

    // Regular levels 1-9 using lookup table on config.h
    const LevelConfig *cfg = &LEVEL_CONFIGS[game->current_level - 1];
    game->enemy_cols = cfg->cols;
    game->enemy_rows = cfg->rows;
    game->enemy_hp = cfg->hp;

    // Initialize enemy formation
    init_enemies(game);
    game->enemy_count = game->enemy_cols * game->enemy_rows;
}

// Advance to next level with efficiency bonus
void advance_level(GameState *game)
{
    // Award efficiency bonus for accurate shooting
    if (game->current_level > 0)
    {
        int total_enemies = game->enemy_cols * game->enemy_rows * game->enemy_hp;
        if (game->bullets_fired_this_level <= total_enemies)
            game->score += SCORE_EFFICIENCY_BONUS;
    }

    // Reset level state
    game->current_level++;
    game->shoot_cooldown = game->enemy_move_delay = game->bullet_move_delay = 0;
    game->bullets_fired_this_level = 0;

    // Reset entities
    clear_bullets(game);
    reset_player(game);
    configure_level(game);
}