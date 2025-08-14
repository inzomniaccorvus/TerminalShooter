#pragma once
#include "types.h"

// Player bullet functions
void shoot_bullet(GameState *game);
void update_bullets(GameState *game);

// Enemy functions
void init_enemies(GameState *game);
void update_enemies(GameState *game);

// Utility functions
void clear_bullets(GameState *game);
void reset_player(GameState *game);

// Boss functions
void update_boss(GameState *game);
void update_boss_bullets(GameState *game);