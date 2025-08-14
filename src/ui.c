#include <stdio.h>
#include <string.h>
#include "config.h"
#include "types.h"
#include "console.h"
#include "sprites.h"

// Draw all game entities to screen
void draw_entities(GameState *game)
{
    // Draw enemies with color based on HP
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (game->enemies[i].active == 1)
            draw_to_buffer(game->enemies[i].x, game->enemies[i].y, GLYPH_ENEMY, RED);
        else if (game->enemies[i].active == 2)
            draw_to_buffer(game->enemies[i].x, game->enemies[i].y, GLYPH_ENEMY, PURPLE);
    }

    // Draw player
    draw_to_buffer(game->player.x, game->player.y, GLYPH_PLAYER, GREEN);

    // Draw player bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active)
            draw_to_buffer(game->bullets[i].x, game->bullets[i].y, GLYPH_BULLET, BLUE);
    }

    // Draw boss and its bullets
    if (game->boss.active && game->boss.hp > 0) {
        int boss_color = (game->boss.flash_timer > 0) ? RED : PURPLE;
        
        // Draw boss body as rectangle
        for (int dy = 0; dy < BOSS_HEIGHT; dy++) {
            for (int dx = 0; dx < BOSS_WIDTH; dx++) {
                draw_to_buffer(game->boss.x + dx - (BOSS_WIDTH / 2),
                              game->boss.y + dy, GLYPH_BOSS, boss_color);
            }
        }

        // Draw boss bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (game->boss.bullets[i].active) {
                draw_to_buffer(game->boss.bullets[i].x, game->boss.bullets[i].y,
                              GLYPH_BOSS_BULLET, RED);
            }
        }
    }
}

// Draw score and status information
void draw_top_bar(GameState *game)
{
    wchar_t info[64];
    
    if (game->current_level == 10) {
        // Boss level - show boss health
        swprintf(info, 64, L"Score: %d Lives: %d Boss health: %d",
                game->score, game->player.health, game->boss.hp);
    } else {
        // Regular level
        swprintf(info, 64, L"Score: %d Lives: %d Level %d",
                game->score, game->player.health, game->current_level);
    }
    
    int color = (game->current_level == 10) ? RED : GREEN;
    draw_centered_text(info, 0, color);
}

// Draw game border using appropriate glyphs
void draw_border(void)
{
    // Draw horizontal borders
    for (int i = 3; i < SCREEN_WIDTH - 3; i++) {
        draw_to_buffer(i, 1, BORDER_H, BLUE);
        draw_to_buffer(i, SCREEN_HEIGHT - 3, BORDER_H, BLUE);
    }
    
    // Draw vertical borders
    for (int i = 2; i < SCREEN_HEIGHT - 3; i++) {
        draw_to_buffer(2, i, BORDER_V, BLUE);
        draw_to_buffer(SCREEN_WIDTH - 3, i, BORDER_V, BLUE);
    }
    
    // Draw corner pieces
    draw_to_buffer(2, 1, BORDER_TL, BLUE);
    draw_to_buffer(SCREEN_WIDTH - 3, 1, BORDER_TR, BLUE);
    draw_to_buffer(2, SCREEN_HEIGHT - 3, BORDER_BL, BLUE);
    draw_to_buffer(SCREEN_WIDTH - 3, SCREEN_HEIGHT - 3, BORDER_BR, BLUE);
}

// Display game over screen
void draw_game_over(int score)
{
    clear_buffer();
    int y = (SCREEN_HEIGHT - GAME_OVER_ART_HEIGHT - 1) / 2;

    // Draw ASCII art
    for (int i = 0; i < GAME_OVER_ART_HEIGHT; i++) {
        int len = wcslen(GAME_OVER_ART[i]);
        int x = (SCREEN_WIDTH - len) / 2;
        for (int j = 0; j < len; j++)
            draw_to_buffer(x + j, y + i, GAME_OVER_ART[i][j], RED);
    }
    
    // Show final score
    wchar_t score_text[64];
    swprintf(score_text, 64, L"Final Score: %d", score);
    draw_centered_text(score_text, SCREEN_HEIGHT - 5, YELLOW);
    display_buffer();
}

// Display victory screen
void show_victory(int score)
{
    clear_buffer();
    int art_height = sizeof(victory_art) / sizeof(victory_art[0]);
    int start_y = (SCREEN_HEIGHT - art_height - 5) / 2;

    // Draw victory ASCII art
    for (int i = 0; i < art_height; i++) {
        int len = wcslen(victory_art[i]);
        int x = (SCREEN_WIDTH - len) / 2;
        for (int j = 0; j < len; j++)
            draw_to_buffer(x + j, start_y + i, victory_art[i][j], YELLOW);
    }

    // Show final score
    wchar_t score_text[64];
    swprintf(score_text, 64, L"Final Score: %d", score);
    draw_centered_text(score_text, start_y + art_height + 2, GREEN);
    display_buffer();
    Sleep(3000);  // Show for 3 seconds
}