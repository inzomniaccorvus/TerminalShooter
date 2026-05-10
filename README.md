# Terminal Shooter

A Space Invaders-style arcade shooter running entirely in the Windows terminal. Built from scratch in C with no external libraries — includes a custom double-buffered terminal renderer using the Windows Console API.

> Built as a CSE115 Structured Programming capstone project at North South University, Fall 2025.

![Gameplay GIF](gameplay.gif)

---

## Gameplay

- Survive 10 waves of enemies across progressively harder formations
- Enemies move in classic Space Invaders fashion — horizontal sweeps with downward steps on edge collision
- Waves 6-9 introduce armoured enemies that require two hits to destroy
- Wave 10 is a boss fight with three attack patterns: spread, wall, and wave
- Shoot enemy bullets out of the air for a deflection bonus
- Efficiency bonus awarded for accurate shooting each wave
- High scores saved locally and persist between sessions
- Save and continue — progress is saved on exit via ESC

---

## Controls

| Input | Action |
|---|---|
| Arrow Keys / WASD | Move left and right |
| Space | Shoot |
| ESC | Save and exit to menu |

---

## Building

### Requirements
- Windows (uses `windows.h` — not cross-platform)
- GCC (MinGW recommended)
- Make

### Steps

1. Clone the repository:
```bash
git clone https://github.com/inzomniaccorvus/terminal-shooter.git
cd terminal-shooter
```

2. Build with Make:
```bash
make
```

3. Run:
```bash
./build/terminal_shooter.exe
```

To clean build artifacts:
```bash
make clean
```

### Note on Unicode glyphs
On first launch the game asks whether your terminal renders Unicode correctly. Press `Y` for fancy Unicode glyphs or `N` for ASCII fallbacks. If characters look like boxes or question marks, choose `N`.

---

## Project Structure

```
terminal_shooter/
├── src/
│   ├── main.c          # Entry point, game loop
│   ├── entities.c      # Player, enemies, bullets, boss logic
│   ├── collision.c     # All collision detection and scoring
│   ├── console.c       # Double-buffered terminal renderer
│   ├── input.c         # Keyboard input handling
│   ├── levels.c        # Level configuration and progression
│   ├── menu.c          # Main menu, high scores, save/load
│   ├── ui.c            # Drawing entities and HUD
│   └── sprites.c       # ASCII art for game over and victory
├── include/
│   ├── config.h        # Constants, colours, level data
│   ├── types.h         # GameState, Player, Enemy, Boss structs
│   └── *.h             # Header for each module
├── build/              # Compiled output
└── Makefile
```

---

## Technical Notes

The renderer uses `WriteConsoleOutputW` to write an entire frame from a back buffer in a single call each tick, avoiding the flicker of per-character writes. All game state lives in a single `GameState` struct passed by pointer through the system.

The boss has three attack patterns that cycle on each direction change — a homing spread with 80% movement prediction, a wide bullet wall, and a sine wave pattern. Fire rate scales with remaining HP.

---

## License

MIT License — see [LICENSE](LICENSE).
