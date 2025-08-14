#pragma once
#include <windows.h>

// Core console functions
void init_console(void);
void clear_buffer(void);
void draw_to_buffer(int x, int y, wchar_t ch, int color);
void display_buffer(void);

// Text drawing helpers
void draw_centered_text(const wchar_t *text, int y, int color);
void draw_text(const wchar_t *text, int x, int y, int color);

// Input helper
void wait_for_key(void);