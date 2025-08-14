#include <windows.h>
#include <stdio.h>
#include "config.h"
#include "types.h"
#include "console.h"
#include "input.h"
#include "entities.h"
#include "collision.h"
#include "ui.h"
#include "sprites.h"
#include "levels.h"
#include "menu.h"

GameState game;           // Global game state
int use_fancy_glyphs = 1; // Global glyph setting

// Initialize game state for new game
void init_game_state(GameState *game)
{
    // Player setup
    game->player.x = SCREEN_WIDTH / 2;
    game->player.y = SCREEN_HEIGHT - 4;
    game->player.health = MAX_PLAYER_HEALTH;

    // Game state reset
    game->score = 0;
    game->current_level = 0;
    game->enemy_count = 0;
    game->enemy_direction = 1;
    game->shoot_cooldown = 0;
    game->game_over = 0;

    // Timing resets
    game->enemy_move_delay = 0;
    game->bullet_move_delay = 0;
    game->bullets_fired_this_level = 0;

    // Boss inactive
    game->boss.active = 0;

    // Clear all bullets and enemies
    for (int i = 0; i < MAX_BULLETS; i++)
        game->bullets[i].active = 0;
    for (int i = 0; i < MAX_ENEMIES; i++)
        game->enemies[i].active = 0;
}

int main(void)
{
    init_console();  // Setup Windows console
    show_settings(); // Glyph compatibility test

    // Main menu loop
    while (1)
    {
        int choice = show_menu();

        switch (choice)
        {
        case MENU_CONTINUE:
            // Load existing game if available
            if (load_game(&game))
            {
                break;
            }
            continue; // If load fails for some reason, return to menu

        case MENU_NEW_GAME:
            // Start fresh game
            init_game_state(&game);
            advance_level(&game);
            break;

        case MENU_HIGH_SCORES:
            Sleep(200); // Debounce
            show_high_scores();
            continue;

        case MENU_INFO:
            Sleep(200); // Debounce
            show_info();
            continue;

        case MENU_EXIT:
            return 0;
        }

        // Main game loop
        while (1)
        {
            // Check for level completion
            if (game.enemy_count <= 0 && !game.boss.active)
            {
                if (game.current_level >= 10)
                {
                    // Game completed
                    show_victory(game.score);
                    save_high_score(game.score);
                    remove("savegame.dat"); // Clear save file
                    break;
                }
                else
                {
                    // Advance to next level
                    advance_level(&game);
                }
            }

            // Process game logic
            handle_input(&game);
            update_bullets(&game);

            if (game.boss.active)
            {
                // Boss level updates
                update_boss(&game);
                update_boss_bullets(&game);
            }
            else
            {
                // Regular enemy updates
                update_enemies(&game);
            }

            collision_check(&game);

            // Check for game over
            if (game.player.health <= 0)
            {
                draw_game_over(game.score); // Show game over screen
                Sleep(2000);
                save_high_score(game.score);
                remove("savegame.dat"); // Clear save file
                exit(0);
            }

            // Render frame
            clear_buffer();
            draw_top_bar(&game);
            draw_border();
            draw_entities(&game);
            display_buffer();

            Sleep(BASE_TICK_DELAY); // Control frame rate
        }
    }

    return 0;
}
