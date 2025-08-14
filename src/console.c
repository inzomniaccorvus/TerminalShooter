#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "sprites.h"
#include "console.h"

HANDLE hconsole;
CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH]; // Back buffer

// Initialize Windows console for displaying buffer
void init_console(void)
{
    hconsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set console buffer and window size
    COORD sz = {SCREEN_WIDTH, SCREEN_HEIGHT};
    SetConsoleScreenBufferSize(hconsole, sz);
    SMALL_RECT win = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};
    SetConsoleWindowInfo(hconsole, TRUE, &win);

    // Hide cursor
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hconsole, &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hconsole, &cci);

    system("cls");
}

// Clear the back buffer
void clear_buffer(void)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            buffer[y][x].Char.UnicodeChar = GLYPH_EMPTY;
            buffer[y][x].Attributes = BLACK;
        }
    }
}

// Draw character to back buffer
void draw_to_buffer(int x, int y, wchar_t ch, int color)
{
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
        return;
    buffer[y][x].Char.UnicodeChar = ch;
    buffer[y][x].Attributes = color;
}

// Display back buffer to screen
void display_buffer(void)
{
    COORD sz = {SCREEN_WIDTH, SCREEN_HEIGHT}, zero = {0, 0};
    SMALL_RECT region = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};
    WriteConsoleOutputW(hconsole, (CHAR_INFO *)buffer, sz, zero, &region);
}

// Draw text centered horizontally
void draw_centered_text(const wchar_t *text, int y, int color)
{
    int len = wcslen(text);
    int x = (SCREEN_WIDTH - len) / 2;
    draw_text(text, x, y, color);
}

// Draw text at specific position
void draw_text(const wchar_t *text, int x, int y, int color)
{
    int len = wcslen(text);
    for (int i = 0; i < len; i++)
        draw_to_buffer(x + i, y, text[i], color);
}

// Wait for any key press
void wait_for_key(void)
{
    while (1)
    {
        for (int key = 0; key < 256; key++)
        {
            if (GetAsyncKeyState(key) & 0x8000)
            {
                Sleep(200); // Debounce
                return;
            }
        }
        Sleep(50);
    }
}