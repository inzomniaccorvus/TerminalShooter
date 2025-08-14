#pragma once
#include "types.h"

// Main drawing functions
void draw_entities(GameState *game);
void draw_top_bar(GameState *game);
void draw_border(void);

// End game screens
void draw_game_over(int score);
void show_victory(int score);