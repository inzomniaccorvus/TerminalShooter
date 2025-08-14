#pragma once

// Fancy Unicode glyphs
#define GLYPH_ENEMY_FANCY L'☢'
#define GLYPH_PLAYER_FANCY L'▲'
#define GLYPH_BULLET_FANCY L'╵'
#define GLYPH_BOSS_FANCY L'░'
#define GLYPH_BOSS_BULLET_FANCY L'▼'

// Fancy Border glyphs
#define BORDER_H_FANCY L'─'
#define BORDER_V_FANCY L'│'
#define BORDER_TL_FANCY L'┌'
#define BORDER_TR_FANCY L'┐'
#define BORDER_BL_FANCY L'└'
#define BORDER_BR_FANCY L'┘'

// ASCII fallbacks
#define GLYPH_ENEMY_ASCII L'*'
#define GLYPH_PLAYER_ASCII L'^'
#define GLYPH_BULLET_ASCII L'|'
#define GLYPH_BOSS_ASCII L'X'
#define GLYPH_BOSS_BULLET_ASCII L'v'

// Border fallbacks
#define BORDER_H_ASCII L'-'
#define BORDER_V_ASCII L'|'
#define BORDER_TL_ASCII L'+'
#define BORDER_TR_ASCII L'+'
#define BORDER_BL_ASCII L'+'
#define BORDER_BR_ASCII L'+'

// Active glyph selection based on settings
#define GLYPH_ENEMY (use_fancy_glyphs ? GLYPH_ENEMY_FANCY : GLYPH_ENEMY_ASCII)
#define GLYPH_PLAYER (use_fancy_glyphs ? GLYPH_PLAYER_FANCY : GLYPH_PLAYER_ASCII)
#define GLYPH_BULLET (use_fancy_glyphs ? GLYPH_BULLET_FANCY : GLYPH_BULLET_ASCII)
#define GLYPH_BOSS (use_fancy_glyphs ? GLYPH_BOSS_FANCY : GLYPH_BOSS_ASCII)
#define GLYPH_BOSS_BULLET (use_fancy_glyphs ? GLYPH_BOSS_BULLET_FANCY : GLYPH_BOSS_BULLET_ASCII)

#define BORDER_H (use_fancy_glyphs ? BORDER_H_FANCY : BORDER_H_ASCII)
#define BORDER_V (use_fancy_glyphs ? BORDER_V_FANCY : BORDER_V_ASCII)
#define BORDER_TL (use_fancy_glyphs ? BORDER_TL_FANCY : BORDER_TL_ASCII)
#define BORDER_TR (use_fancy_glyphs ? BORDER_TR_FANCY : BORDER_TR_ASCII)
#define BORDER_BL (use_fancy_glyphs ? BORDER_BL_FANCY : BORDER_BL_ASCII)
#define BORDER_BR (use_fancy_glyphs ? BORDER_BR_FANCY : BORDER_BR_ASCII)

#define GLYPH_EMPTY L' '

// ASCII art dimensions
#define GAME_OVER_ART_HEIGHT 5
#define VICTORY_ART_HEIGHT 6

// Art defined in sprites.c
extern wchar_t *GAME_OVER_ART[GAME_OVER_ART_HEIGHT];
extern wchar_t *victory_art[VICTORY_ART_HEIGHT];