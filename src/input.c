#include <windows.h>
#include "config.h"
#include "types.h"
#include "input.h"
#include "entities.h"
#include "menu.h"

// Handle all player input each frame
void handle_input(GameState *game)
{
    static int space_prev = 0; // Previous space key state to stop spamming on hold

    // ESC key - save and exit to menu
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        save_game(game);
        exit(0);
    }

    // Left movement (Arrow key or A)
    if ((GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000) &&
        game->player.x > 3)
    {
        game->player.x--;
    }

    // Right movement (Arrow key or D)
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000) &&
        game->player.x < SCREEN_WIDTH - 4)
    {
        game->player.x++;
    }

    // Update shoot cooldown
    if (game->shoot_cooldown > 0)
    {
        game->shoot_cooldown--;
    }

    // Shooting with hold detection (only fire on key press, not hold)
    int space_now = GetAsyncKeyState(VK_SPACE) & 0x8000;
    if (space_now && !space_prev && game->shoot_cooldown == 0)
    {
        shoot_bullet(game);
        game->shoot_cooldown = SHOOT_COOLDOWN_TICKS;
    }
    space_prev = space_now;
}