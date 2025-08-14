#pragma once
#include "config.h"

// Basic game entities
typedef struct
{
    int x, y, health;
} Player;
typedef struct
{
    int x, y, active;
} Bullet;
typedef struct
{
    int x, y, active;
} Enemy;

// Boss entity
typedef struct
{
    int x, y, active, hp, max_hp;
    int move_delay, shoot_delay, move_direction, move_counter;
    int pause_counter, attack_pattern, flash_timer;
    Bullet bullets[MAX_BULLETS];
} Boss;

// Type to pack globals
typedef struct
{
    Player player;
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    Boss boss;
    int score, current_level, enemy_count, enemy_direction;
    int enemy_cols, enemy_rows, enemy_hp;
    int shoot_cooldown, game_over;
    int enemy_move_delay, bullet_move_delay, bullets_fired_this_level;
} GameState;

// High score storage
typedef struct
{
    int scores[10];
    int count;
} HighScores;