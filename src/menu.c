#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "config.h"
#include "types.h"
#include "console.h"
#include "menu.h"
#include "sprites.h"

extern int use_fancy_glyphs;

static const wchar_t *menu_options[] = {
    L"Continue Game", L"New Game", L"High Scores", L"Info", L"Exit"};

// Draw a single menu option with highlighting
static void draw_menu_option(int index, int selected, int has_save)
{
    if (index == MENU_CONTINUE && !has_save)
        return; // Skip if no save file

    int y = 10 + index * 2;
    int color = (index == selected) ? YELLOW : GREEN;
    wchar_t prefix = (index == selected) ? L'>' : L' ';

    draw_to_buffer(SCREEN_WIDTH / 2 - 10, y, prefix, color);
    draw_text(menu_options[index], SCREEN_WIDTH / 2 - 8, y, color);
}

// Main menu navigation and selection
int show_menu(void)
{
    int selected = 1;               // Start on "New Game"
    int has_save = load_game(NULL); // Check if save file exists

    while (1)
    {
        clear_buffer();
        draw_centered_text(L"TERMINAL SHOOTER", 5, CYAN);

        // Draw all menu options
        for (int i = 0; i < MENU_COUNT; i++)
            draw_menu_option(i, selected, has_save);

        draw_centered_text(L"Use Arrow Keys and Space", SCREEN_HEIGHT - 5, BLUE);
        display_buffer();

        // Handle input
        if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
            do
            {
                selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;
            } while (selected == MENU_CONTINUE && !has_save); // Skip continue if no save
            Sleep(200);
        }
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
            do
            {
                selected = (selected + 1) % MENU_COUNT;
            } while (selected == MENU_CONTINUE && !has_save); // Skip continue if no save
            Sleep(200);
        }
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            return selected;
        }
    }
}

// Save high score to file
void save_high_score(int score)
{
    HighScores scores = {0};

    // Load existing scores
    FILE *file = fopen("highscores.dat", "rb");
    if (file)
    {
        fread(&scores, sizeof(HighScores), 1, file);
        fclose(file);
    }

    // Check if score qualifies for high score list
    if (scores.count < 10 || score > scores.scores[9])
    {
        // Find insertion position
        int pos = scores.count;
        for (int i = 0; i < scores.count; i++)
        {
            if (score > scores.scores[i])
            {
                pos = i;
                break;
            }
        }

        // Shift lower scores down
        for (int i = 9; i > pos; i--)
            scores.scores[i] = scores.scores[i - 1];

        // Insert new score
        scores.scores[pos] = score;
        if (scores.count < 10)
            scores.count++;

        // Save updated scores
        file = fopen("highscores.dat", "wb");
        if (file)
        {
            fwrite(&scores, sizeof(HighScores), 1, file);
            fclose(file);
        }
    }
}

// Display high scores screen
void show_high_scores(void)
{
    HighScores scores = {0};

    // Load scores from file
    FILE *file = fopen("highscores.dat", "rb");
    if (file)
    {
        fread(&scores, sizeof(HighScores), 1, file);
        fclose(file);
    }

    clear_buffer();
    draw_centered_text(L"HIGH SCORES", 3, YELLOW);

    if (scores.count == 0)
    {
        draw_centered_text(L"No high scores yet!", 10, RED);
    }
    else
    {
        // Display each score
        for (int i = 0; i < scores.count; i++)
        {
            wchar_t line[32];
            swprintf(line, 32, L"%d. %d", i + 1, scores.scores[i]);
            draw_centered_text(line, 6 + i * 2, GREEN);
        }
    }

    draw_centered_text(L"Press any key to return", SCREEN_HEIGHT - 3, BLUE);
    display_buffer();
    wait_for_key();
}

// Glyph compatibility test for display settings
void show_settings(void)
{
    clear_buffer();
    draw_centered_text(L"SETTINGS - GLYPH TEST", 3, YELLOW);

    // Display test glyphs with labels
    const wchar_t glyphs[] = {GLYPH_ENEMY, GLYPH_PLAYER, GLYPH_BULLET, GLYPH_BOSS};
    const wchar_t *names[] = {L"Enemy", L"Player", L"Bullet", L"Boss"};
    const int colors[] = {RED, GREEN, BLUE, PURPLE};

    for (int i = 0; i < 4; i++)
    {
        draw_to_buffer(10, 8 + i * 2, glyphs[i], colors[i]);
        draw_text(names[i], 17, 8 + i * 2, WHITE);
    }

    // Test border glyphs
    draw_to_buffer(10, 16, BORDER_TL, BLUE);
    draw_to_buffer(11, 16, BORDER_H, BLUE);
    draw_to_buffer(12, 16, BORDER_TR, BLUE);
    draw_to_buffer(10, 17, BORDER_V, BLUE);
    draw_to_buffer(12, 17, BORDER_V, BLUE);
    draw_to_buffer(10, 18, BORDER_BL, BLUE);
    draw_to_buffer(11, 18, BORDER_H, BLUE);
    draw_to_buffer(12, 18, BORDER_BR, BLUE);
    draw_text(L"Border", 17, 17, WHITE);

    draw_centered_text(L"Do all glyphs print correctly? (Y/N)", 22, CYAN);
    display_buffer();

    // Wait for user response
    while (1)
    {
        if (GetAsyncKeyState('Y') & 0x8000)
        {
            use_fancy_glyphs = 1;
            Sleep(200);
            return;
        }
        else if (GetAsyncKeyState('N') & 0x8000)
        {
            use_fancy_glyphs = 0;
            Sleep(200);
            return;
        }
        Sleep(50);
    }
}

// Display game information and credits
void show_info(void)
{
    clear_buffer();
    draw_centered_text(L"GAME INFO", 2, YELLOW);

    const wchar_t *info_lines[] = {
        L"CONTROLS:",
        L"  Arrow Keys / WASD - Move",
        L"  Space - Shoot",
        L"",
        L"SCORING:",
        L"  Enemy Hit: 25 points",
        L"  Enemy Kill: +25 bonus",
        L"  Boss Hit: 100 points",
        L"  Boss Kill: 2000 points",
        L"  Bullet Deflect: 10 points",
        L"  Efficiency Bonus: 100",
        L"",
        L"GAME INFO:",
        L"  10 Levels total",
        L"  Boss fight on level 10",
        L"  3 Lives to start",
        L"",
        L"CSE115 Capstone Project by SAI Sammo, Omar and Shantanu",
        L"Press any key to return"};

    int line_num = sizeof(info_lines) / sizeof(info_lines[0]);

    // Draw all info lines with appropriate colors
    for (int i = 0; i < line_num; i++)
    {
        int color = (i == 0 || i == 4 || i == 12) ? CYAN : // Headers
                        (i == 17) ? PURPLE
                                  : WHITE; // Credits
        draw_centered_text(info_lines[i], 5 + i, color);
    }

    display_buffer();
    wait_for_key();
}

// Save complete game state to file
void save_game(GameState *game)
{
    FILE *file = fopen("savegame.dat", "wb");
    if (file)
    {
        fwrite(game, sizeof(GameState), 1, file);
        fclose(file);
    }
}

// Load game state from file (or just check if file exists)
int load_game(GameState *game)
{
    FILE *file = fopen("savegame.dat", "rb");
    if (file)
    {
        if (game)
            fread(game, sizeof(GameState), 1, file); // Load if game state provided
        fclose(file);
        return 1; // File exists
    }
    return 0; // No save file
}
