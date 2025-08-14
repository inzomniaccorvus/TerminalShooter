#pragma once
#include "types.h"

// Menu options
typedef enum
{
    MENU_CONTINUE,
    MENU_NEW_GAME,
    MENU_HIGH_SCORES,
    MENU_INFO,
    MENU_EXIT,
    MENU_COUNT
} MenuOption;

// Menu and settings functions
int show_menu(void);
void show_high_scores(void);
void show_settings(void);
void show_info(void);

// Save functions
void save_high_score(int score);
void save_game(GameState *game);
int load_game(GameState *game);