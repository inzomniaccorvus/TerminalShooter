#pragma once

// Screen dimensions
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30

// Colorscheme using windows console color values
#define RED 12
#define GREEN 10
#define BLUE 9
#define PURPLE 5
#define YELLOW 14
#define CYAN 11
#define WHITE 15
#define BLACK 0

// Game limits and array sizes
#define MAX_BULLETS 15
#define MAX_ENEMIES 100
#define MAX_PLAYER_HEALTH 3
#define MAX_LEVELS 10

// Enemy formation settings
#define ENEMY_SPACING 4

// Game timing (lower = faster)
#define BASE_TICK_DELAY 25
#define SHOOT_COOLDOWN_TICKS 15
#define ENEMY_MOVE_DELAY 6
#define BULLET_MOVE_DELAY 3

// Boss configuration
#define BOSS_MAX_HP 10
#define BOSS_WIDTH 6
#define BOSS_HEIGHT 3
#define BOSS_MOVE_DELAY 12
#define BOSS_SHOOT_DELAY 80
#define BOSS_BULLET_SPEED 1
#define BOSS_MOVE_DISTANCE 20
#define BOSS_PAUSE_TIME 20
#define BOSS_FLASH_TIME 5

// Scoring system
#define SCORE_ENEMY_HIT 25
#define SCORE_ENEMY_KILL 25
#define SCORE_BOSS_HIT 100
#define SCORE_BOSS_KILL 2000
#define SCORE_BULLET_DEFLECT 10
#define SCORE_EFFICIENCY_BONUS 100

// Level progression data
typedef struct
{
    int cols, rows, hp;
} LevelConfig;

static const LevelConfig LEVEL_CONFIGS[MAX_LEVELS] = {
    {15, 3, 1},
    {16, 3, 1},
    {17, 3, 1},
    {18, 4, 1},
    {18, 4, 1},
    {15, 3, 2}, // Level 6 - armored enemies
    {16, 3, 2},
    {17, 3, 2},
    {18, 4, 2},
    {0, 0, 0} // Level 10 - boss level
};

// Global settings
extern int use_fancy_glyphs;
