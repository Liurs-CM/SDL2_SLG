//=====================================================================
//
// GameLib_SDL2.h - A single-header game library based on SDL2
//
// This version removes all Windows dependencies and uses SDL2 for
// cross-platform windowing, rendering, input, and timing.
//
// How to use (single file project, most common):
//
//     #define GAMELIB_IMPLEMENTATION
//     #include "GameLib_SDL2.h"
//
//     int main() {
//         GameLib game;
//         game.Open(640, 480, "My Game", true);
//
//         int x = 320, y = 240;
//
//         while (!game.IsClosed()) {
//             if (game.IsKeyDown(KEY_UP))    y -= 3;
//             if (game.IsKeyDown(KEY_DOWN))  y += 3;
//             if (game.IsKeyDown(KEY_LEFT))  x -= 3;
//             if (game.IsKeyDown(KEY_RIGHT)) x += 3;
//
//             game.Clear(COLOR_BLACK);
//             game.FillRect(x - 10, y - 10, 20, 20, COLOR_RED);
//             game.DrawText(10, 10, "Move the box!", COLOR_WHITE);
//             game.Update();
//
//             game.WaitFrame(60);
//         }
//         return 0;
//     }
//
// Compile command (any platform with SDL2):
//     g++ -o game main.cpp -lSDL2
//
// Last Modified: 2026/04/11
//
//=====================================================================
#ifndef GAMELIB_SDL2_H
#define GAMELIB_SDL2_H

// Default behavior: include enables implementation (good for single file projects)
#ifndef GAMELIB_NO_IMPLEMENTATION
#ifndef GAMELIB_IMPLEMENTATION
#define GAMELIB_IMPLEMENTATION
#endif
#endif

// Version Info
#define GAMELIB_VERSION_MAJOR     1
#define GAMELIB_VERSION_MINOR     0
#define GAMELIB_VERSION_PATCH     2


//---------------------------------------------------------------------
// System header files
//---------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>

#include <vector>
#include <string>
#include <algorithm> // For std::min/std::max if needed


//---------------------------------------------------------------------
// SDL2 does not require explicit linking like Windows
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// Dynamically loaded function pointers are no longer needed
//---------------------------------------------------------------------


//=====================================================================
// Part 1: Constants
//=====================================================================

//---------------------------------------------------------------------
// Color constants (ARGB format: 0xAARRGGBB)
//---------------------------------------------------------------------
#define COLOR_BLACK       0xFF000000
#define COLOR_WHITE       0xFFFFFFFF
#define COLOR_RED         0xFFFF0000
#define COLOR_GREEN       0xFF00FF00
#define COLOR_BLUE        0xFF0000FF
#define COLOR_YELLOW      0xFFFFFF00
#define COLOR_CYAN        0xFF00FFFF
#define COLOR_MAGENTA     0xFFFF00FF
#define COLOR_ORANGE      0xFFFF8800
#define COLOR_PINK        0xFFFF88CC
#define COLOR_PURPLE      0xFF8800FF
#define COLOR_GRAY        0xFF888888
#define COLOR_DARK_GRAY   0xFF444444
#define COLOR_LIGHT_GRAY  0xFFCCCCCC
#define COLOR_DARK_RED    0xFF880000
#define COLOR_DARK_GREEN  0xFF008800
#define COLOR_DARK_BLUE   0xFF000088
#define COLOR_SKY_BLUE    0xFF87CEEB
#define COLOR_BROWN       0xFF8B4513
#define COLOR_GOLD        0xFFFFD700
#define COLOR_TRANSPARENT 0x00000000

// Color helper macros
#define COLOR_RGB(r, g, b)     ((uint32_t)(0xFF000000 | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))
#define COLOR_ARGB(a, r, g, b) ((uint32_t)((((a) & 0xFF) << 24) | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))

// Color component extraction
#define COLOR_GET_A(c)    (((c) >> 24) & 0xFF)
#define COLOR_GET_R(c)    (((c) >> 16) & 0xFF)
#define COLOR_GET_G(c)    (((c) >> 8) & 0xFF)
#define COLOR_GET_B(c)    ((c) & 0xFF)

//---------------------------------------------------------------------
// Keyboard constants (mapped to SDL Scancodes)
//---------------------------------------------------------------------
#define KEY_LEFT      SDL_SCANCODE_LEFT
#define KEY_RIGHT     SDL_SCANCODE_RIGHT
#define KEY_UP        SDL_SCANCODE_UP
#define KEY_DOWN      SDL_SCANCODE_DOWN
#define KEY_SPACE     SDL_SCANCODE_SPACE
#define KEY_ENTER     SDL_SCANCODE_RETURN
#define KEY_ESCAPE    SDL_SCANCODE_ESCAPE
#define KEY_TAB       SDL_SCANCODE_TAB
#define KEY_SHIFT     SDL_SCANCODE_LSHIFT // Or SDL_SCANCODE_RSHIFT
#define KEY_CONTROL   SDL_SCANCODE_LCTRL  // Or SDL_SCANCODE_RCTRL
#define KEY_BACK      SDL_SCANCODE_BACKSPACE

#define KEY_A         SDL_SCANCODE_A
#define KEY_B         SDL_SCANCODE_B
#define KEY_C         SDL_SCANCODE_C
#define KEY_D         SDL_SCANCODE_D
#define KEY_E         SDL_SCANCODE_E
#define KEY_F         SDL_SCANCODE_F
#define KEY_G         SDL_SCANCODE_G
#define KEY_H         SDL_SCANCODE_H
#define KEY_I         SDL_SCANCODE_I
#define KEY_J         SDL_SCANCODE_J
#define KEY_K         SDL_SCANCODE_K
#define KEY_L         SDL_SCANCODE_L
#define KEY_M         SDL_SCANCODE_M
#define KEY_N         SDL_SCANCODE_N
#define KEY_O         SDL_SCANCODE_O
#define KEY_P         SDL_SCANCODE_P
#define KEY_Q         SDL_SCANCODE_Q
#define KEY_R         SDL_SCANCODE_R
#define KEY_S         SDL_SCANCODE_S
#define KEY_T         SDL_SCANCODE_T
#define KEY_U         SDL_SCANCODE_U
#define KEY_V         SDL_SCANCODE_V
#define KEY_W         SDL_SCANCODE_W
#define KEY_X         SDL_SCANCODE_X
#define KEY_Y         SDL_SCANCODE_Y
#define KEY_Z         SDL_SCANCODE_Z

#define KEY_0         SDL_SCANCODE_0
#define KEY_1         SDL_SCANCODE_1
#define KEY_2         SDL_SCANCODE_2
#define KEY_3         SDL_SCANCODE_3
#define KEY_4         SDL_SCANCODE_4
#define KEY_5         SDL_SCANCODE_5
#define KEY_6         SDL_SCANCODE_6
#define KEY_7         SDL_SCANCODE_7
#define KEY_8         SDL_SCANCODE_8
#define KEY_9         SDL_SCANCODE_9

#define KEY_F1        SDL_SCANCODE_F1
#define KEY_F2        SDL_SCANCODE_F2
#define KEY_F3        SDL_SCANCODE_F3
#define KEY_F4        SDL_SCANCODE_F4
#define KEY_F5        SDL_SCANCODE_F5
#define KEY_F6        SDL_SCANCODE_F6
#define KEY_F7        SDL_SCANCODE_F7
#define KEY_F8        SDL_SCANCODE_F8
#define KEY_F9        SDL_SCANCODE_F9
#define KEY_F10       SDL_SCANCODE_F10
#define KEY_F11       SDL_SCANCODE_F11
#define KEY_F12       SDL_SCANCODE_F12

//---------------------------------------------------------------------
// Mouse button constants
//---------------------------------------------------------------------
#define MOUSE_LEFT    0
#define MOUSE_RIGHT   1
#define MOUSE_MIDDLE  2

//---------------------------------------------------------------------
// Sprite drawing flags
//---------------------------------------------------------------------
#define SPRITE_FLIP_H     1    // flip horizontal
#define SPRITE_FLIP_V     2    // flip vertical
#define SPRITE_COLORKEY   4    // enable transparent color
#define SPRITE_ALPHA      8    // enable alpha blending

// Default Color Key: magenta (255, 0, 255), common transparent color in 2D games
#ifndef COLORKEY_DEFAULT
#define COLORKEY_DEFAULT  0xFFFF00FF
#endif


//=====================================================================
// Part 2: Class Declaration
//=====================================================================

//---------------------------------------------------------------------
// GameLib Main Class
//---------------------------------------------------------------------
class GameLib
{
public:
    GameLib();
    ~GameLib();

    // -------- Window and Main Loop --------
    int Open(int width, int height, const char *title, bool center = false);
    bool IsClosed() const;
    void Update();
    void WaitFrame(int fps);
    float GetDeltaTime() const;
    float GetFPS() const;
    float GetTime() const;
    int GetWidth() const;
    int GetHeight() const;
    void SetTitle(const char *title);
    void ShowFps(bool show);

    // -------- Frame Buffer --------
    void Clear(uint32_t color = COLOR_BLACK);
    void SetPixel(int x, int y, uint32_t color);
    uint32_t GetPixel(int x, int y) const;

    // -------- Drawing --------
    void DrawLine(int x1, int y1, int x2, int y2, uint32_t color);
    void DrawRect(int x, int y, int w, int h, uint32_t color);
    void FillRect(int x, int y, int w, int h, uint32_t color);
    void DrawCircle(int cx, int cy, int r, uint32_t color);
    void FillCircle(int cx, int cy, int r, uint32_t color);
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
    void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);

    // -------- Text Rendering (built-in 8x8 font) --------
    void DrawText(int x, int y, const char *text, uint32_t color);
    void DrawNumber(int x, int y, int number, uint32_t color);
    void DrawTextScale(int x, int y, const char *text, uint32_t color, int scale);
    void DrawPrintf(int x, int y, uint32_t color, const char *fmt, ...);

    // -------- Sprite System (managed by integer ID) --------
    int CreateSprite(int width, int height);
    int LoadSpriteBMP(const char *filename); // Basic BMP loader for simplicity
    int LoadSprite(const char *filename); // Can be extended to use SDL_image
    void FreeSprite(int id);
    void DrawSprite(int id, int x, int y);
    void DrawSpriteEx(int id, int x, int y, int flags);
    void DrawSpriteRegion(int id, int x, int y, int sx, int sy, int sw, int sh);
    void SetSpritePixel(int id, int x, int y, uint32_t color);
    uint32_t GetSpritePixel(int id, int x, int y) const;
    int GetSpriteWidth(int id) const;
    int GetSpriteHeight(int id) const;

    // -------- Input --------
    bool IsKeyDown(int key) const;
    bool IsKeyPressed(int key) const;
    int GetMouseX() const;
    int GetMouseY() const;
    bool IsMouseDown(int button) const;
    bool IsMousePressed(int button) const;

    // -------- Sound --------
    void PlayBeep(int frequency, int duration);
    void PlayWAV(const char *filename, bool loop = false);
    void StopWAV();
    void PlayMusic(const char *filename, bool loop = true);
    void StopMusic();

    // -------- Helper Functions --------
    static int Random(int minVal, int maxVal);
    static bool RectOverlap(int x1, int y1, int w1, int h1,
                            int x2, int y2, int w2, int h2);
    static bool CircleOverlap(int cx1, int cy1, int r1,
                              int cx2, int cy2, int r2);
    static bool PointInRect(int px, int py, int x, int y, int w, int h);
    static float Distance(int x1, int y1, int x2, int y2);

    // -------- Grid Helpers --------
    void DrawGrid(int x, int y, int rows, int cols, int cellSize, uint32_t color);
    void FillCell(int gridX, int gridY, int row, int col, int cellSize, uint32_t color);

    // -------- Tilemap System --------
    int CreateTilemap(int cols, int rows, int tileSize, int tilesetId);
    void FreeTilemap(int mapId);
    void SetTile(int mapId, int col, int row, int tileId);
    int GetTile(int mapId, int col, int row) const;
    void DrawTilemap(int mapId, int x, int y, int flags = 0);

private:
    // disable copy
    GameLib(const GameLib &);
    GameLib &operator=(const GameLib &);

    // internal window management
    void _DispatchMessages();
    void _UpdateTitleFps();

    // internal pixel drawing (no bounds check, for fast drawing after clipping)
    void _SetPixelFast(int x, int y, uint32_t color);
    void _DrawHLine(int x1, int x2, int y, uint32_t color);

    // internal sprite management
    int _AllocSpriteSlot();

    // internal tilemap management
    int _AllocTilemapSlot();

private:
    // SDL state
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Texture* _screenTexture; // Texture to hold the software-rendered frame

    // window state
    bool _closing;
    bool _showFps;
    std::string _title;
    int _width;
    int _height;

    // frame buffer (software rendered)
    uint32_t *_framebuffer;

    // input state
    Uint8 _keys[SDL_NUM_SCANCODES];
    Uint8 _keys_prev[SDL_NUM_SCANCODES];
    int _mouseX;
    int _mouseY;
    Uint8 _mouseButtons[3];
    Uint8 _mouseButtons_prev[3];

    // timing
    Uint64 _timeStart;
    Uint64 _timePrev;
    float _deltaTime;
    float _fps;
    float _fpsAccum;
    Uint32 _fpsTime;
    Uint64 _perfFreq; // Performance counter frequency for high-res timing

    // sprite storage
    struct GameSprite { int width, height; uint32_t *pixels; bool used; };
    std::vector<GameSprite> _sprites;

    // tilemap storage
    struct GameTilemap {
        int cols, rows;     // map grid size
        int tileSize;       // tile size in pixels
        int tilesetId;      // tileset sprite ID
        int tilesetCols;    // tiles per row in tileset
        int *tiles;         // tile ID array (cols * rows, -1 = empty)
        bool used;          // is this slot in use
    };
    std::vector<GameTilemap> _tilemaps;

    // music state (placeholder for future SDL_mixer integration)
    bool _musicPlaying;

    // random seed initialized flag
    static bool _srandDone;
};


//=====================================================================
// Part 3: 8x8 Font Data (ASCII 32-126)
//=====================================================================

// Classic 8x8 bitmap font, 8 bytes per char, one byte per row, MSB on left
static const unsigned char _gamelib_font8x8[95][8] = {
    { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }, // 32 ' '
    { 0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00 }, // 33 '!'
    { 0x6C,0x6C,0x24,0x00,0x00,0x00,0x00,0x00 }, // 34 '"'
    { 0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00 }, // 35 '#'
    { 0x18,0x7E,0xC0,0x7C,0x06,0xFC,0x18,0x00 }, // 36 '$'
    { 0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6,0x00 }, // 37 '%'
    { 0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00 }, // 38 '&'
    { 0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00 }, // 39 '''
    { 0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00 }, // 40 '('
    { 0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00 }, // 41 ')'
    { 0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00 }, // 42 '*'
    { 0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00 }, // 43 '+'
    { 0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30 }, // 44 ','
    { 0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00 }, // 45 '-'
    { 0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00 }, // 46 '.'
    { 0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00 }, // 47 '/'
    { 0x7C,0xCE,0xDE,0xF6,0xE6,0xC6,0x7C,0x00 }, // 48 '0'
    { 0x18,0x38,0x78,0x18,0x18,0x18,0x7E,0x00 }, // 49 '1'
    { 0x7C,0xC6,0x06,0x1C,0x70,0xC6,0xFE,0x00 }, // 50 '2'
    { 0x7C,0xC6,0x06,0x3C,0x06,0xC6,0x7C,0x00 }, // 51 '3'
    { 0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x1E,0x00 }, // 52 '4'
    { 0xFE,0xC0,0xFC,0x06,0x06,0xC6,0x7C,0x00 }, // 53 '5'
    { 0x38,0x60,0xC0,0xFC,0xC6,0xC6,0x7C,0x00 }, // 54 '6'
    { 0xFE,0xC6,0x0C,0x18,0x30,0x30,0x30,0x00 }, // 55 '7'
    { 0x7C,0xC6,0xC6,0x7C,0xC6,0xC6,0x7C,0x00 }, // 56 '8'
    { 0x7C,0xC6,0xC6,0x7E,0x06,0x0C,0x78,0x00 }, // 57 '9'
    { 0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00 }, // 58 ':'
    { 0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30 }, // 59 ';'
    { 0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00 }, // 60 '<'
    { 0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00 }, // 61 '='
    { 0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0x00 }, // 62 '>'
    { 0x7C,0xC6,0x0C,0x18,0x18,0x00,0x18,0x00 }, // 63 '?'
    { 0x7C,0xC6,0xDE,0xDE,0xDC,0xC0,0x7C,0x00 }, // 64 '@'
    { 0x38,0x6C,0xC6,0xC6,0xFE,0xC6,0xC6,0x00 }, // 65 'A'
    { 0xFC,0xC6,0xC6,0xFC,0xC6,0xC6,0xFC,0x00 }, // 66 'B'
    { 0x7C,0xC6,0xC0,0xC0,0xC0,0xC6,0x7C,0x00 }, // 67 'C'
    { 0xF8,0xCC,0xC6,0xC6,0xC6,0xCC,0xF8,0x00 }, // 68 'D'
    { 0xFE,0xC0,0xC0,0xFC,0xC0,0xC0,0xFE,0x00 }, // 69 'E'
    { 0xFE,0xC0,0xC0,0xFC,0xC0,0xC0,0xC0,0x00 }, // 70 'F'
    { 0x7C,0xC6,0xC0,0xCE,0xC6,0xC6,0x7E,0x00 }, // 71 'G'
    { 0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0x00 }, // 72 'H'
    { 0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00 }, // 73 'I'
    { 0x1E,0x0C,0x0C,0x0C,0xCC,0xCC,0x78,0x00 }, // 74 'J'
    { 0xC6,0xCC,0xD8,0xF0,0xD8,0xCC,0xC6,0x00 }, // 75 'K'
    { 0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFE,0x00 }, // 76 'L'
    { 0xC6,0xEE,0xFE,0xD6,0xC6,0xC6,0xC6,0x00 }, // 77 'M'
    { 0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6,0x00 }, // 78 'N'
    { 0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00 }, // 79 'O'
    { 0xFC,0xC6,0xC6,0xFC,0xC0,0xC0,0xC0,0x00 }, // 80 'P'
    { 0x7C,0xC6,0xC6,0xC6,0xD6,0xDE,0x7C,0x06 }, // 81 'Q'
    { 0xFC,0xC6,0xC6,0xFC,0xD8,0xCC,0xC6,0x00 }, // 82 'R'
    { 0x7C,0xC6,0xC0,0x7C,0x06,0xC6,0x7C,0x00 }, // 83 'S'
    { 0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00 }, // 84 'T'
    { 0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00 }, // 85 'U'
    { 0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x10,0x00 }, // 86 'V'
    { 0xC6,0xC6,0xC6,0xD6,0xFE,0xEE,0xC6,0x00 }, // 87 'W'
    { 0xC6,0xC6,0x6C,0x38,0x6C,0xC6,0xC6,0x00 }, // 88 'X'
    { 0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00 }, // 89 'Y'
    { 0xFE,0x06,0x0C,0x18,0x30,0x60,0xFE,0x00 }, // 90 'Z'
    { 0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00 }, // 91 '['
    { 0xC0,0x60,0x30,0x18,0x0C,0x06,0x02,0x00 }, // 92 '\'
    { 0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00 }, // 93 ']'
    { 0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00 }, // 94 '^'
    { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE }, // 95 '_'
    { 0x30,0x18,0x0C,0x00,0x00,0x00,0x00,0x00 }, // 96 '`'
    { 0x00,0x00,0x78,0x0C,0x7C,0xCC,0x76,0x00 }, // 97 'a'
    { 0xC0,0xC0,0xFC,0xC6,0xC6,0xC6,0xFC,0x00 }, // 98 'b'
    { 0x00,0x00,0x7C,0xC6,0xC0,0xC6,0x7C,0x00 }, // 99 'c'
    { 0x06,0x06,0x7E,0xC6,0xC6,0xC6,0x7E,0x00 }, //100 'd'
    { 0x00,0x00,0x7C,0xC6,0xFE,0xC0,0x7C,0x00 }, //101 'e'
    { 0x1C,0x36,0x30,0x7C,0x30,0x30,0x30,0x00 }, //102 'f'
    { 0x00,0x00,0x7E,0xC6,0xC6,0x7E,0x06,0x7C }, //103 'g'
    { 0xC0,0xC0,0xFC,0xC6,0xC6,0xC6,0xC6,0x00 }, //104 'h'
    { 0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00 }, //105 'i'
    { 0x0C,0x00,0x1C,0x0C,0x0C,0x0C,0xCC,0x78 }, //106 'j'
    { 0xC0,0xC0,0xCC,0xD8,0xF0,0xD8,0xCC,0x00 }, //107 'k'
    { 0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00 }, //108 'l'
    { 0x00,0x00,0xCC,0xFE,0xD6,0xC6,0xC6,0x00 }, //109 'm'
    { 0x00,0x00,0xFC,0xC6,0xC6,0xC6,0xC6,0x00 }, //110 'n'
    { 0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7C,0x00 }, //111 'o'
    { 0x00,0x00,0xFC,0xC6,0xC6,0xFC,0xC0,0xC0 }, //112 'p'
    { 0x00,0x00,0x7E,0xC6,0xC6,0x7E,0x06,0x06 }, //113 'q'
    { 0x00,0x00,0xDC,0xE6,0xC0,0xC0,0xC0,0x00 }, //114 'r'
    { 0x00,0x00,0x7E,0xC0,0x7C,0x06,0xFC,0x00 }, //115 's'
    { 0x30,0x30,0x7C,0x30,0x30,0x36,0x1C,0x00 }, //116 't'
    { 0x00,0x00,0xC6,0xC6,0xC6,0xC6,0x7E,0x00 }, //117 'u'
    { 0x00,0x00,0xC6,0xC6,0xC6,0x6C,0x38,0x00 }, //118 'v'
    { 0x00,0x00,0xC6,0xC6,0xD6,0xFE,0x6C,0x00 }, //119 'w'
    { 0x00,0x00,0xC6,0x6C,0x38,0x6C,0xC6,0x00 }, //120 'x'
    { 0x00,0x00,0xC6,0xC6,0xC6,0x7E,0x06,0x7C }, //121 'y'
    { 0x00,0x00,0xFE,0x0C,0x38,0x60,0xFE,0x00 }, //122 'z'
    { 0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0x00 }, //123 '{'
    { 0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00 }, //124 '|'
    { 0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0x00 }, //125 '}'
    { 0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00 }, //126 '~'
};


//=====================================================================
// Part 4: Implementation
//=====================================================================
#ifdef GAMELIB_IMPLEMENTATION

// Static member initialization
bool GameLib::_srandDone = false;


//---------------------------------------------------------------------
// Constructor
//---------------------------------------------------------------------
GameLib::GameLib()
{
    _window = nullptr;
    _renderer = nullptr;
    _screenTexture = nullptr;
    _closing = false;
    _showFps = false;
    _width = 0;
    _height = 0;
    _framebuffer = nullptr;
    memset(_keys, 0, sizeof(_keys));
    memset(_keys_prev, 0, sizeof(_keys_prev));
    _mouseX = 0;
    _mouseY = 0;
    memset(_mouseButtons, 0, sizeof(_mouseButtons));
    memset(_mouseButtons_prev, 0, sizeof(_mouseButtons_prev));
    _timeStart = 0;
    _timePrev = 0;
    _deltaTime = 0.0f;
    _fps = 0.0f;
    _fpsAccum = 0.0f;
    _fpsTime = 0;
    _perfFreq = SDL_GetPerformanceFrequency();
    _musicPlaying = false;
    if (!_srandDone) {
        srand((unsigned int)time(nullptr));
        _srandDone = true;
    }
}


//---------------------------------------------------------------------
// Destructor
//---------------------------------------------------------------------
GameLib::~GameLib()
{
    // Free all sprites
    for (auto& spr : _sprites) {
        if (spr.used && spr.pixels) {
            free(spr.pixels);
            spr.pixels = nullptr;
            spr.used = false;
        }
    }
    // Free all Tilemaps
    for (auto& tm : _tilemaps) {
        if (tm.used && tm.tiles) {
            free(tm.tiles);
            tm.tiles = nullptr;
            tm.used = false;
        }
    }
    // Free framebuffer
    if (_framebuffer) {
        delete[] _framebuffer;
        _framebuffer = nullptr;
    }
    // Free SDL resources
    if (_screenTexture) SDL_DestroyTexture(_screenTexture);
    if (_renderer) SDL_DestroyRenderer(_renderer);
    if (_window) SDL_DestroyWindow(_window);
    // Quit SDL
    SDL_Quit();
}


//---------------------------------------------------------------------
// Dispatch SDL messages
//---------------------------------------------------------------------
void GameLib::_DispatchMessages()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _closing = true;
                break;
            case SDL_KEYDOWN:
                _keys[event.key.keysym.scancode] = 1;
                break;
            case SDL_KEYUP:
                _keys[event.key.keysym.scancode] = 0;
                break;
            case SDL_MOUSEMOTION:
                _mouseX = event.motion.x;
                _mouseY = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) _mouseButtons[MOUSE_LEFT] = 1;
                if (event.button.button == SDL_BUTTON_RIGHT) _mouseButtons[MOUSE_RIGHT] = 1;
                if (event.button.button == SDL_BUTTON_MIDDLE) _mouseButtons[MOUSE_MIDDLE] = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) _mouseButtons[MOUSE_LEFT] = 0;
                if (event.button.button == SDL_BUTTON_RIGHT) _mouseButtons[MOUSE_RIGHT] = 0;
                if (event.button.button == SDL_BUTTON_MIDDLE) _mouseButtons[MOUSE_MIDDLE] = 0;
                break;
        }
    }
}


//---------------------------------------------------------------------
// Open: create window and initialize
//---------------------------------------------------------------------
int GameLib::Open(int width, int height, const char *title, bool center)
{
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 16384 || height > 16384) return -7;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        return -1;
    }

    _width = width;
    _height = height;
    _title = title;
    _closing = false;

    // Create window
    Uint32 window_flags = SDL_WINDOW_SHOWN;
    if (center) {
        window_flags |= SDL_WINDOWPOS_CENTERED;
    }
    _window = SDL_CreateWindow(title,
                              center ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED,
                              center ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED,
                              width, height, window_flags);
    if (!_window) {
        SDL_Quit();
        return -2;
    }

    // Create renderer (prefer hardware acceleration with VSync)
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_renderer) {
        // Fallback to software renderer if hardware fails
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_SOFTWARE);
        if (!_renderer) {
            SDL_DestroyWindow(_window);
            SDL_Quit();
            return -3;
        }
    }

    // Create texture to represent our software-rendered frame buffer
    _screenTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!_screenTexture) {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
        return -4;
    }

    // Allocate software frame buffer
    _framebuffer = new(std::nothrow) uint32_t[width * height];
    if (!_framebuffer) {
        SDL_DestroyTexture(_screenTexture);
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
        return -5;
    }
    Clear(COLOR_BLACK); // Initialize with black

    // Initialize time
    _timeStart = SDL_GetPerformanceCounter();
    _timePrev = _timeStart;
    _fpsTime = SDL_GetTicks();
    _fpsAccum = 0.0f;
    _fps = 0.0f;
    _deltaTime = 0.0f;

    // Initialize input
    memset(_keys, 0, sizeof(_keys));
    memset(_keys_prev, 0, sizeof(_keys_prev));
    memset(_mouseButtons, 0, sizeof(_mouseButtons));
    memset(_mouseButtons_prev, 0, sizeof(_mouseButtons_prev));

    return 0;
}


//---------------------------------------------------------------------
// IsClosed
//---------------------------------------------------------------------
bool GameLib::IsClosed() const
{
    return _closing;
}


//---------------------------------------------------------------------
// Update: flush frame buffer to window, then process messages and update input
//---------------------------------------------------------------------
void GameLib::Update()
{
    if (!_window || !_framebuffer || !_screenTexture) return;

    // Update texture with current frame buffer data
    void* pixels;
    int pitch;
    if (SDL_LockTexture(_screenTexture, nullptr, &pixels, &pitch) == 0) {
        memcpy(pixels, _framebuffer, _width * _height * sizeof(uint32_t));
        SDL_UnlockTexture(_screenTexture);
    }

    // Render the texture to the screen
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _screenTexture, nullptr, nullptr);
    SDL_RenderPresent(_renderer);

    // Save previous frame key/mouse state (for edge detection)
    memcpy(_keys_prev, _keys, sizeof(_keys));
    memcpy(_mouseButtons_prev, _mouseButtons, sizeof(_mouseButtons));

    // Dispatch messages
    _DispatchMessages();

    // Update time
    Uint64 now = SDL_GetPerformanceCounter();
    double delta_ticks = (double)(now - _timePrev);
    _deltaTime = (float)(delta_ticks / _perfFreq);
    _timePrev = now;

    // Update FPS
    _fpsAccum += 1.0f;
    Uint32 now_ms = SDL_GetTicks();
    Uint32 fpsDelta = now_ms - _fpsTime;
    if (fpsDelta >= 1000) {
        _fps = _fpsAccum * 1000.0f / fpsDelta;
        _fpsAccum = 0.0f;
        _fpsTime = now_ms;
        _UpdateTitleFps();
    }
}


//---------------------------------------------------------------------
// WaitFrame: frame rate control
//---------------------------------------------------------------------
void GameLib::WaitFrame(int fps)
{
    if (fps <= 0) fps = 60;
    Uint32 frameTimeMs = 1000 / fps;
    Uint32 currentTimeMs = SDL_GetTicks();
    Uint64 lastFrameTimeMs = _timePrev * 1000 / _perfFreq; // Convert ticks to ms for comparison
    Uint64 elapsed = currentTimeMs - lastFrameTimeMs;
    if (elapsed < frameTimeMs) {
        SDL_Delay(frameTimeMs - (Uint32)elapsed);
    }
    // Note: This simple delay might not be as precise as the original due to OS scheduler.
    // The renderer's VSync (if enabled) also affects timing.
}


//---------------------------------------------------------------------
// GetDeltaTime / GetFPS / GetTime / GetWidth / GetHeight
//---------------------------------------------------------------------
float GameLib::GetDeltaTime() const { return _deltaTime; }
float GameLib::GetFPS() const { return _fps; }
float GameLib::GetTime() const
{
    Uint64 now = SDL_GetPerformanceCounter();
    return (float)((double)(now - _timeStart) / _perfFreq);
}
int GameLib::GetWidth() const { return _width; }
int GameLib::GetHeight() const { return _height; }


//---------------------------------------------------------------------
// SetTitle
//---------------------------------------------------------------------
void GameLib::SetTitle(const char *title)
{
    _title = title;
    if (_window) {
        SDL_SetWindowTitle(_window, title);
    }
}


//---------------------------------------------------------------------
// ShowFps: whether to show FPS in title bar
//---------------------------------------------------------------------
void GameLib::ShowFps(bool show)
{
    _showFps = show;
    if (!show && _window) {
        // Restore original title when turned off
        SetTitle(_title.c_str());
    }
}


//---------------------------------------------------------------------
// _UpdateTitleFps: update title bar FPS display (internal method)
//---------------------------------------------------------------------
void GameLib::_UpdateTitleFps()
{
    if (!_showFps || !_window) return;
    char buf[256];
    snprintf(buf, sizeof(buf), "%s (FPS: %.1f)", _title.c_str(), _fps);
    SDL_SetWindowTitle(_window, buf);
}


//=====================================================================
// Frame Buffer Operations
//=====================================================================

void GameLib::Clear(uint32_t color)
{
    if (!_framebuffer) return;
    size_t count = (size_t)_width * _height;
    for (size_t i = 0; i < count; i++) _framebuffer[i] = color;
}

void GameLib::SetPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        _framebuffer[y * _width + x] = color;
    }
}

uint32_t GameLib::GetPixel(int x, int y) const
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        return _framebuffer[y * _width + x];
    }
    return 0;
}

void GameLib::_SetPixelFast(int x, int y, uint32_t color)
{
    _framebuffer[y * _width + x] = color;
}


//=====================================================================
// Drawing Functions
//=====================================================================

//---------------------------------------------------------------------
// DrawLine: Bresenham's algorithm
//---------------------------------------------------------------------
void GameLib::DrawLine(int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        SetPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}


//---------------------------------------------------------------------
// Horizontal line (internal use, with clipping)
//---------------------------------------------------------------------
void GameLib::_DrawHLine(int x1, int x2, int y, uint32_t color)
{
    if (y < 0 || y >= _height) return;
    if (x1 > x2) { int t = x1; x1 = x2; x2 = t; }
    if (x1 < 0) x1 = 0;
    if (x2 >= _width) x2 = _width - 1;
    for (int x = x1; x <= x2; x++) {
        _SetPixelFast(x, y, color);
    }
}


//---------------------------------------------------------------------
// DrawRect / FillRect
//---------------------------------------------------------------------
void GameLib::DrawRect(int x, int y, int w, int h, uint32_t color)
{
    if (w <= 0 || h <= 0) return;
    _DrawHLine(x, x + w - 1, y, color);
    _DrawHLine(x, x + w - 1, y + h - 1, color);
    // Vertical edges (skip corners already drawn by _DrawHLine)
    for (int j = y + 1; j < y + h - 1; j++) {
        SetPixel(x, j, color);
        SetPixel(x + w - 1, j, color);
    }
}

void GameLib::FillRect(int x, int y, int w, int h, uint32_t color)
{
    if (!_framebuffer) return;
    int x1 = x, y1 = y, x2 = x + w, y2 = y + h;
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 > _width) x2 = _width;
    if (y2 > _height) y2 = _height;
    for (int j = y1; j < y2; j++) {
        uint32_t *row = _framebuffer + j * _width;
        for (int i = x1; i < x2; i++) {
            row[i] = color;
        }
    }
}


//---------------------------------------------------------------------
// DrawCircle: midpoint circle algorithm
//---------------------------------------------------------------------
void GameLib::DrawCircle(int cx, int cy, int r, uint32_t color)
{
    int x = 0, y = r;
    int d = 1 - r;
    while (x <= y) {
        SetPixel(cx + x, cy + y, color);
        SetPixel(cx - x, cy + y, color);
        SetPixel(cx + x, cy - y, color);
        SetPixel(cx - x, cy - y, color);
        SetPixel(cx + y, cy + x, color);
        SetPixel(cx - y, cy + x, color);
        SetPixel(cx + y, cy - x, color);
        SetPixel(cx - y, cy - x, color);
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}


//---------------------------------------------------------------------
// FillCircle
//---------------------------------------------------------------------
void GameLib::FillCircle(int cx, int cy, int r, uint32_t color)
{
    int x = 0, y = r;
    int d = 1 - r;
    while (x <= y) {
        _DrawHLine(cx - x, cx + x, cy + y, color);
        _DrawHLine(cx - x, cx + x, cy - y, color);
        _DrawHLine(cx - y, cx + y, cy + x, color);
        _DrawHLine(cx - y, cx + y, cy - x, color);
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}


//---------------------------------------------------------------------
// DrawTriangle
//---------------------------------------------------------------------
void GameLib::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x3, y3, color);
    DrawLine(x3, y3, x1, y1, color);
}


//---------------------------------------------------------------------
// FillTriangle: scanline fill
//---------------------------------------------------------------------
void GameLib::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    // Sort by y: y1 <= y2 <= y3
    if (y1 > y2) { int t; t=x1; x1=x2; x2=t; t=y1; y1=y2; y2=t; }
    if (y1 > y3) { int t; t=x1; x1=x3; x3=t; t=y1; y1=y3; y3=t; }
    if (y2 > y3) { int t; t=x2; x2=x3; x3=t; t=y2; y2=y3; y3=t; }

    if (y3 == y1) {
        int minX = x1, maxX = x1;
        if (x2 < minX) minX = x2;
        if (x2 > maxX) maxX = x2;
        if (x3 < minX) minX = x3;
        if (x3 > maxX) maxX = x3;
        _DrawHLine(minX, maxX, y1, color);
        return;
    }

    for (int y = y1; y <= y3; y++) {
        int xa, xb;
        // y3 != y1 is always true here (degenerate case returned above)
        xa = x1 + (int)((int64_t)(x3 - x1) * (y - y1) / (y3 - y1));
        if (y < y2) {
            if (y2 != y1) {
                xb = x1 + (int)((int64_t)(x2 - x1) * (y - y1) / (y2 - y1));
            } else {
                xb = x1;
            }
        } else {
            if (y3 != y2) {
                xb = x2 + (int)((int64_t)(x3 - x2) * (y - y2) / (y3 - y2));
            } else {
                xb = x2;
            }
        }
        _DrawHLine(xa, xb, y, color);
    }
}


//=====================================================================
// Text Rendering
//=====================================================================

void GameLib::DrawText(int x, int y, const char *text, uint32_t color)
{
    if (!text) return;
    int ox = x;
    for (const char *p = text; *p; p++) {
        unsigned char ch = (unsigned char)*p;
        if (ch == '\n') {
            x = ox;
            y += 10;  // 8 pixels high + 2 pixels line spacing
            continue;
        }
        if (ch < 32 || ch > 126) continue;
        const unsigned char *glyph = _gamelib_font8x8[ch - 32];
        for (int row = 0; row < 8; row++) {
            unsigned char bits = glyph[row];
            for (int col = 0; col < 8; col++) {
                if (bits & (0x80 >> col)) {
                    SetPixel(x + col, y + row, color);
                }
            }
        }
        x += 8;
    }
}

void GameLib::DrawNumber(int x, int y, int number, uint32_t color)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", number);
    DrawText(x, y, buf, color);
}

void GameLib::DrawTextScale(int x, int y, const char *text, uint32_t color, int scale)
{
    if (!text || scale <= 0) return;
    int ox = x;
    for (const char *p = text; *p; p++) {
        unsigned char ch = (unsigned char)*p;
        if (ch == '\n') {
            x = ox;
            y += (8 + 2) * scale;
            continue;
        }
        if (ch < 32 || ch > 126) continue;
        const unsigned char *glyph = _gamelib_font8x8[ch - 32];
        for (int row = 0; row < 8; row++) {
            unsigned char bits = glyph[row];
            for (int col = 0; col < 8; col++) {
                if (bits & (0x80 >> col)) {
                    FillRect(x + col * scale, y + row * scale, scale, scale, color);
                }
            }
        }
        x += 8 * scale;
    }
}

void GameLib::DrawPrintf(int x, int y, uint32_t color, const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    buf[sizeof(buf) - 1] = '\0';
    DrawText(x, y, buf, color);
}


//=====================================================================
// Sprite System
//=====================================================================

int GameLib::_AllocSpriteSlot()
{
    for (size_t i = 0; i < _sprites.size(); i++) {
        if (!_sprites[i].used) return (int)i;
    }
    GameSprite spr;
    spr.width = 0;
    spr.height = 0;
    spr.pixels = nullptr;
    spr.used = false;
    _sprites.push_back(spr);
    return (int)(_sprites.size() - 1);
}

int GameLib::CreateSprite(int width, int height)
{
    if (width <= 0 || height <= 0 || width > 16384 || height > 16384) return -1;
    int id = _AllocSpriteSlot();
    uint32_t *pixels = (uint32_t*)malloc((size_t)width * height * sizeof(uint32_t));
    if (!pixels) return -1;
    memset(pixels, 0, (size_t)width * height * sizeof(uint32_t));
    _sprites[id].width = width;
    _sprites[id].height = height;
    _sprites[id].pixels = pixels;
    _sprites[id].used = true;
    return id;
}

// --- Simplified BMP Loading (same as original) ---
int GameLib::LoadSpriteBMP(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;

    unsigned char header[54];
    if (fread(header, 1, 54, fp) != 54) { fclose(fp); return -1; }
    if (header[0] != 'B' || header[1] != 'M') { fclose(fp); return -1; }

    // Read header fields using memcpy to avoid strict aliasing / unaligned access
    int dataOffset; memcpy(&dataOffset, &header[10], 4);
    int dibSize;    memcpy(&dibSize,    &header[14], 4);
    int width;      memcpy(&width,      &header[18], 4);
    int height;     memcpy(&height,     &header[22], 4);
    short bppShort; memcpy(&bppShort,   &header[28], 2);
    int bpp = bppShort;
    int colorsUsed; memcpy(&colorsUsed, &header[46], 4);

    if (bpp != 8 && bpp != 24 && bpp != 32) { fclose(fp); return -1; }

    bool bottomUp = (height > 0);
    if (height < 0) height = -height;

    // Validate dimensions to prevent integer overflow and unreasonable allocations
    if (width <= 0 || height <= 0 || width > 16384 || height > 16384) {
        fclose(fp); return -1;
    }

    // Read palette for 8-bit indexed BMP
    uint32_t palette[256];
    memset(palette, 0, sizeof(palette));

    if (bpp == 8) {
        int paletteCount = colorsUsed > 0 ? colorsUsed : 256;
        if (paletteCount > 256) paletteCount = 256;

        // Palette starts right after the DIB header (at offset 14 + dibSize)
        fseek(fp, 14 + dibSize, SEEK_SET);

        unsigned char palData[256 * 4];
        int palBytes = paletteCount * 4;
        if (fread(palData, 1, palBytes, fp) != (size_t)palBytes) {
            fclose(fp); return -1;
        }
        // BMP palette entries are BGRX (4 bytes each: Blue, Green, Red, Reserved)
        for (int i = 0; i < paletteCount; i++) {
            unsigned char b = palData[i * 4 + 0];
            unsigned char g = palData[i * 4 + 1];
            unsigned char r = palData[i * 4 + 2];
            palette[i] = COLOR_ARGB(0xFF, r, g, b);
        }
    }

    fseek(fp, dataOffset, SEEK_SET);

    int bytesPerPixel = bpp / 8;  // 1 for 8-bit, 3 for 24-bit, 4 for 32-bit
    int rowSize = ((width * bytesPerPixel + 3) / 4) * 4;
    unsigned char *rowData = (unsigned char*)malloc(rowSize);
    if (!rowData) { fclose(fp); return -1; }

    int id = CreateSprite(width, height);
    if (id < 0) { free(rowData); fclose(fp); return -1; }

    for (int y = 0; y < height; y++) {
        if (fread(rowData, 1, rowSize, fp) != (size_t)rowSize) break;
        int destY = bottomUp ? (height - 1 - y) : y;
        uint32_t *destRow = _sprites[id].pixels + destY * width;
        if (bpp == 8) {
            // 8-bit indexed: each byte is a palette index
            for (int x = 0; x < width; x++) {
                destRow[x] = palette[rowData[x]];
            }
        } else {
            for (int x = 0; x < width; x++) {
                unsigned char b = rowData[x * bytesPerPixel + 0];
                unsigned char g = rowData[x * bytesPerPixel + 1];
                unsigned char r = rowData[x * bytesPerPixel + 2];
                unsigned char a = (bpp == 32) ? rowData[x * bytesPerPixel + 3] : 0xFF;
                destRow[x] = COLOR_ARGB(a, r, g, b);
            }
        }
    }

    free(rowData);
    fclose(fp);
    return id;
}

// --- Placeholder for advanced image loading ---
// This version uses the basic BMP loader for simplicity.
// To support PNG/JPG, integrate SDL_image library and IMG_Load.
int GameLib::LoadSprite(const char *filename)
{
    // For now, just try to load as BMP
    return LoadSpriteBMP(filename);
    // Future: Replace with IMG_Load if SDL_image is linked
    /*
    SDL_Surface* surf = IMG_Load(filename);
    if (!surf) return -1;
    if (surf->format->format != SDL_PIXELFORMAT_ARGB8888) {
        // Convert to ARGB8888 if needed
        SDL_Surface* converted_surf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface(surf);
        surf = converted_surf;
        if (!surf) return -1;
    }
    int id = CreateSprite(surf->w, surf->h);
    if (id < 0) { SDL_FreeSurface(surf); return -1; }
    memcpy(_sprites[id].pixels, surf->pixels, surf->w * surf->h * 4);
    SDL_FreeSurface(surf);
    return id;
    */
}

void GameLib::FreeSprite(int id)
{
    if (id < 0 || id >= (int)_sprites.size()) return;
    if (!_sprites[id].used) return;
    if (_sprites[id].pixels) {
        free(_sprites[id].pixels);
        _sprites[id].pixels = nullptr;
    }
    _sprites[id].width = 0;
    _sprites[id].height = 0;
    _sprites[id].used = false;
}

void GameLib::DrawSprite(int id, int x, int y)
{
    DrawSpriteEx(id, x, y, 0);
}

void GameLib::DrawSpriteEx(int id, int x, int y, int flags)
{
    if (id < 0 || id >= (int)_sprites.size()) return;
    if (!_sprites[id].used) return;
    GameSprite &spr = _sprites[id];
    bool flipH = (flags & SPRITE_FLIP_H) != 0;
    bool flipV = (flags & SPRITE_FLIP_V) != 0;

    // Pre-clip to avoid per-pixel boundary check
    int sx0 = 0, sx1 = spr.width;
    int sy0 = 0, sy1 = spr.height;
    if (x < 0) sx0 = -x;
    if (y < 0) sy0 = -y;
    if (x + sx1 > _width)  sx1 = _width - x;
    if (y + sy1 > _height) sy1 = _height - y;
    if (sx0 >= sx1 || sy0 >= sy1) return;

    if (flags & SPRITE_ALPHA) {
        // ---- Alpha blending path ----
        bool colorKey = (flags & SPRITE_COLORKEY) != 0;
        for (int sy = sy0; sy < sy1; sy++) {
            int srcY = flipV ? (spr.height - 1 - sy) : sy;
            const uint32_t *srcRow = spr.pixels + srcY * spr.width;
            uint32_t *dstRow = _framebuffer + (y + sy) * _width + x;
            for (int sx = sx0; sx < sx1; sx++) {
                int srcX = flipH ? (spr.width - 1 - sx) : sx;
                uint32_t c = srcRow[srcX];
                if (colorKey && c == COLORKEY_DEFAULT) continue;
                uint32_t sa = COLOR_GET_A(c);
                if (sa == 0) continue;
                if (sa == 255) {
                    dstRow[sx] = c;
                } else {
                    uint32_t dc = dstRow[sx];
                    uint32_t ia = 255 - sa;
                    uint32_t or_ = (sa * COLOR_GET_R(c) + ia * COLOR_GET_R(dc)) / 255;
                    uint32_t og = (sa * COLOR_GET_G(c) + ia * COLOR_GET_G(dc)) / 255;
                    uint32_t ob = (sa * COLOR_GET_B(c) + ia * COLOR_GET_B(dc)) / 255;
                    dstRow[sx] = COLOR_ARGB(255, or_, og, ob);
                }
            }
        }
    } else if (flags & SPRITE_COLORKEY) {
        // ---- ColorKey transparent path ----
        for (int sy = sy0; sy < sy1; sy++) {
            int srcY = flipV ? (spr.height - 1 - sy) : sy;
            const uint32_t *srcRow = spr.pixels + srcY * spr.width;
            uint32_t *dstRow = _framebuffer + (y + sy) * _width + x;
            for (int sx = sx0; sx < sx1; sx++) {
                int srcX = flipH ? (spr.width - 1 - sx) : sx;
                uint32_t c = srcRow[srcX];
                if (c != COLORKEY_DEFAULT)
                    dstRow[sx] = c;
            }
        }
    } else {
        // ---- Opaque path (skip alpha=0) ----
        for (int sy = sy0; sy < sy1; sy++) {
            int srcY = flipV ? (spr.height - 1 - sy) : sy;
            const uint32_t *srcRow = spr.pixels + srcY * spr.width;
            uint32_t *dstRow = _framebuffer + (y + sy) * _width + x;
            for (int sx = sx0; sx < sx1; sx++) {
                int srcX = flipH ? (spr.width - 1 - sx) : sx;
                uint32_t c = srcRow[srcX];
                if (COLOR_GET_A(c) > 0)
                    dstRow[sx] = c;
            }
        }
    }
}

void GameLib::DrawSpriteRegion(int id, int x, int y, int sx, int sy, int sw, int sh)
{
    if (id < 0 || id >= (int)_sprites.size()) return;
    if (!_sprites[id].used) return;
    GameSprite &spr = _sprites[id];
    
    // Validate region parameters
    if (sx < 0 || sy < 0 || sw <= 0 || sh <= 0) return;
    if (sx + sw > spr.width || sy + sh > spr.height) return;
    
    for (int j = 0; j < sh; j++) {
        int srcY = sy + j;
        int dy = y + j;
        if (srcY < 0 || srcY >= spr.height) continue;
        if (dy < 0 || dy >= _height) continue;
        for (int i = 0; i < sw; i++) {
            int srcX = sx + i;
            int dx = x + i;
            if (srcX < 0 || srcX >= spr.width) continue;
            if (dx < 0 || dx >= _width) continue;
            uint32_t c = spr.pixels[srcY * spr.width + srcX];
            if (COLOR_GET_A(c) > 0) {
                _framebuffer[dy * _width + dx] = c;
            }
        }
    }
}

void GameLib::SetSpritePixel(int id, int x, int y, uint32_t color)
{
    if (id < 0 || id >= (int)_sprites.size()) return;
    if (!_sprites[id].used) return;
    if (x < 0 || x >= _sprites[id].width) return;
    if (y < 0 || y >= _sprites[id].height) return;
    _sprites[id].pixels[y * _sprites[id].width + x] = color;
}

uint32_t GameLib::GetSpritePixel(int id, int x, int y) const
{
    if (id < 0 || id >= (int)_sprites.size()) return 0;
    if (!_sprites[id].used) return 0;
    if (x < 0 || x >= _sprites[id].width) return 0;
    if (y < 0 || y >= _sprites[id].height) return 0;
    return _sprites[id].pixels[y * _sprites[id].width + x];
}

int GameLib::GetSpriteWidth(int id) const
{
    if (id < 0 || id >= (int)_sprites.size()) return 0;
    if (!_sprites[id].used) return 0;
    return _sprites[id].width;
}

int GameLib::GetSpriteHeight(int id) const
{
    if (id < 0 || id >= (int)_sprites.size()) return 0;
    if (!_sprites[id].used) return 0;
    return _sprites[id].height;
}


//=====================================================================
// Input System
//=====================================================================

bool GameLib::IsKeyDown(int key) const
{
    // Ensure key is within bounds for SDL scancode array
    if (key >= 0 && key < SDL_NUM_SCANCODES) {
        return _keys[key] != 0;
    }
    return false;
}

bool GameLib::IsKeyPressed(int key) const
{
    if (key >= 0 && key < SDL_NUM_SCANCODES) {
        return (_keys[key] != 0) && (_keys_prev[key] == 0);
    }
    return false;
}

int GameLib::GetMouseX() const { return _mouseX; }
int GameLib::GetMouseY() const { return _mouseY; }

bool GameLib::IsMouseDown(int button) const
{
    if (button < 0 || button > 2) return false;
    return _mouseButtons[button] != 0;
}

bool GameLib::IsMousePressed(int button) const
{
    if (button < 0 || button > 2) return false;
    return (_mouseButtons[button] != 0) && (_mouseButtons_prev[button] == 0);
}


//=====================================================================
// Sound (Placeholder implementations)
// To implement fully, integrate SDL_mixer library.
//=====================================================================

void GameLib::PlayBeep(int frequency, int duration)
{
    // SDL2 has no built-in beep. On Linux, you might use system("beep").
    // For cross-platform, SDL_mixer is recommended.
    // This is a placeholder.
    printf("Beep not implemented without SDL_mixer.\n");
}

void GameLib::PlayWAV(const char *filename, bool loop)
{
    // Requires SDL_mixer
    printf("PlayWAV not implemented without SDL_mixer.\n");
}

void GameLib::StopWAV()
{
    // Requires SDL_mixer
    printf("StopWAV not implemented without SDL_mixer.\n");
}

void GameLib::PlayMusic(const char *filename, bool loop)
{
    // Requires SDL_mixer
    printf("PlayMusic not implemented without SDL_mixer.\n");
}

void GameLib::StopMusic()
{
    // Requires SDL_mixer
    printf("StopMusic not implemented without SDL_mixer.\n");
}


//=====================================================================
// Helper Functions
//=====================================================================

int GameLib::Random(int minVal, int maxVal)
{
    if (minVal > maxVal) { int t = minVal; minVal = maxVal; maxVal = t; }
    if (minVal == maxVal) return minVal;
    // Fixed: corrected range calculation to include maxVal in possible results
    return minVal + rand() % (maxVal - minVal + 1);
}

bool GameLib::RectOverlap(int x1, int y1, int w1, int h1,
                          int x2, int y2, int w2, int h2)
{
    return !(x1 + w1 <= x2 || x2 + w2 <= x1 || y1 + h1 <= y2 || y2 + h2 <= y1);
}

bool GameLib::CircleOverlap(int cx1, int cy1, int r1,
                            int cx2, int cy2, int r2)
{
    int64_t dx = cx1 - cx2;
    int64_t dy = cy1 - cy2;
    int64_t distSq = dx * dx + dy * dy;
    int64_t rSum = r1 + r2;
    return distSq <= rSum * rSum;
}

bool GameLib::PointInRect(int px, int py, int x, int y, int w, int h)
{
    return px >= x && px < x + w && py >= y && py < y + h;
}

float GameLib::Distance(int x1, int y1, int x2, int y2)
{
    float dx = (float)(x1 - x2);
    float dy = (float)(y1 - y2);
    return sqrtf(dx * dx + dy * dy);
}


//=====================================================================
// Grid Helpers
//=====================================================================

void GameLib::DrawGrid(int x, int y, int rows, int cols, int cellSize, uint32_t color)
{
    for (int r = 0; r <= rows; r++) {
        DrawLine(x, y + r * cellSize, x + cols * cellSize, y + r * cellSize, color);
    }
    for (int c = 0; c <= cols; c++) {
        DrawLine(x + c * cellSize, y, x + c * cellSize, y + rows * cellSize, color);
    }
}

void GameLib::FillCell(int gridX, int gridY, int row, int col, int cellSize, uint32_t color)
{
    FillRect(gridX + col * cellSize + 1, gridY + row * cellSize + 1,
             cellSize - 1, cellSize - 1, color);
}


//=====================================================================
// Tilemap System
//=====================================================================

int GameLib::_AllocTilemapSlot()
{
    for (size_t i = 0; i < _tilemaps.size(); i++) {
        if (!_tilemaps[i].used) return (int)i;
    }
    GameTilemap tm;
    tm.cols = 0;
    tm.rows = 0;
    tm.tileSize = 0;
    tm.tilesetId = -1;
    tm.tilesetCols = 0;
    tm.tiles = nullptr;
    tm.used = false;
    _tilemaps.push_back(tm);
    return (int)(_tilemaps.size() - 1);
}

int GameLib::CreateTilemap(int cols, int rows, int tileSize, int tilesetId)
{
    if (cols <= 0 || rows <= 0 || tileSize <= 0) return -1;
    if (tilesetId < 0 || tilesetId >= (int)_sprites.size()) return -1;
    if (!_sprites[tilesetId].used) return -1;

    if (cols > 4096 || rows > 4096) return -1; // prevent overflow
    int id = _AllocTilemapSlot();
    int *tiles = (int*)malloc((size_t)cols * rows * sizeof(int));
    if (!tiles) return -1;
    for (int i = 0; i < cols * rows; i++) tiles[i] = -1;

    _tilemaps[id].cols = cols;
    _tilemaps[id].rows = rows;
    _tilemaps[id].tileSize = tileSize;
    _tilemaps[id].tilesetId = tilesetId;
    _tilemaps[id].tilesetCols = _sprites[tilesetId].width / tileSize;
    _tilemaps[id].tiles = tiles;
    _tilemaps[id].used = true;

    return id;
}

void GameLib::FreeTilemap(int mapId)
{
    if (mapId < 0 || mapId >= (int)_tilemaps.size()) return;
    if (!_tilemaps[mapId].used) return;
    if (_tilemaps[mapId].tiles) {
        free(_tilemaps[mapId].tiles);
        _tilemaps[mapId].tiles = nullptr;
    }
    _tilemaps[mapId].used = false;
}

void GameLib::SetTile(int mapId, int col, int row, int tileId)
{
    if (mapId < 0 || mapId >= (int)_tilemaps.size()) return;
    if (!_tilemaps[mapId].used) return;
    if (col < 0 || col >= _tilemaps[mapId].cols) return;
    if (row < 0 || row >= _tilemaps[mapId].rows) return;
    _tilemaps[mapId].tiles[row * _tilemaps[mapId].cols + col] = tileId;
}

int GameLib::GetTile(int mapId, int col, int row) const
{
    if (mapId < 0 || mapId >= (int)_tilemaps.size()) return -1;
    if (!_tilemaps[mapId].used) return -1;
    if (col < 0 || col >= _tilemaps[mapId].cols) return -1;
    if (row < 0 || row >= _tilemaps[mapId].rows) return -1;
    return _tilemaps[mapId].tiles[row * _tilemaps[mapId].cols + col];
}

void GameLib::DrawTilemap(int mapId, int x, int y, int flags)
{
    if (mapId < 0 || mapId >= (int)_tilemaps.size()) return;
    if (!_tilemaps[mapId].used) return;

    GameTilemap &tm = _tilemaps[mapId];
    int tsId = tm.tilesetId;
    if (tsId < 0 || tsId >= (int)_sprites.size()) return;
    if (!_sprites[tsId].used) return;

    GameSprite &tset = _sprites[tsId];
    int ts = tm.tileSize;
    int tsCols = tm.tilesetCols;
    if (tsCols <= 0) return;

    // Calculate visible tile range on screen, avoid traversing the whole map
    int col0 = (-x) / ts;
    int row0 = (-y) / ts;
    int col1 = (-x + _width - 1) / ts + 1;
    int row1 = (-y + _height - 1) / ts + 1;
    if (col0 < 0) col0 = 0;
    if (row0 < 0) row0 = 0;
    if (col1 > tm.cols) col1 = tm.cols;
    if (row1 > tm.rows) row1 = tm.rows;

    bool useAlpha    = (flags & SPRITE_ALPHA) != 0;
    bool useColorKey = (flags & SPRITE_COLORKEY) != 0;

    for (int r = row0; r < row1; r++) {
        for (int c = col0; c < col1; c++) {
            int tid = tm.tiles[r * tm.cols + c];
            if (tid < 0) continue;

            // Pixel start position of this tile in tileset
            int srcCol = tid % tsCols;
            int srcRow = tid / tsCols;
            int srcX0 = srcCol * ts;
            int srcY0 = srcRow * ts;

            // Screen destination position
            int dstX0 = x + c * ts;
            int dstY0 = y + r * ts;

            // Clip within tile
            int ix0 = 0, iy0 = 0, ix1 = ts, iy1 = ts;
            if (dstX0 < 0) ix0 = -dstX0;
            if (dstY0 < 0) iy0 = -dstY0;
            if (dstX0 + ix1 > _width)  ix1 = _width - dstX0;
            if (dstY0 + iy1 > _height) iy1 = _height - dstY0;

            if (useAlpha) {
                // ---- Alpha blending path ----
                for (int iy = iy0; iy < iy1; iy++) {
                    int sy = srcY0 + iy;
                    if (sy < 0 || sy >= tset.height) continue;
                    const uint32_t *srcRow_ = tset.pixels + sy * tset.width;
                    uint32_t *dstRow_ = _framebuffer + (dstY0 + iy) * _width;
                    for (int ix = ix0; ix < ix1; ix++) {
                        int sx = srcX0 + ix;
                        if (sx < 0 || sx >= tset.width) continue;
                        uint32_t sc = srcRow_[sx];
                        if (useColorKey && sc == COLORKEY_DEFAULT) continue;
                        uint32_t sa = COLOR_GET_A(sc);
                        if (sa == 0) continue;
                        int dx = dstX0 + ix;
                        if (sa == 255) {
                            dstRow_[dx] = sc;
                        } else {
                            uint32_t dc = dstRow_[dx];
                            uint32_t ia = 255 - sa;
                            uint32_t or_ = (sa * COLOR_GET_R(sc) + ia * COLOR_GET_R(dc)) / 255;
                            uint32_t og = (sa * COLOR_GET_G(sc) + ia * COLOR_GET_G(dc)) / 255;
                            uint32_t ob = (sa * COLOR_GET_B(sc) + ia * COLOR_GET_B(dc)) / 255;
                            dstRow_[dx] = COLOR_ARGB(255, or_, og, ob);
                        }
                    }
                }
            } else if (useColorKey) {
                // ---- ColorKey transparent path ----
                for (int iy = iy0; iy < iy1; iy++) {
                    int sy = srcY0 + iy;
                    if (sy < 0 || sy >= tset.height) continue;
                    const uint32_t *srcRow_ = tset.pixels + sy * tset.width;
                    uint32_t *dstRow_ = _framebuffer + (dstY0 + iy) * _width;
                    for (int ix = ix0; ix < ix1; ix++) {
                        int sx = srcX0 + ix;
                        if (sx < 0 || sx >= tset.width) continue;
                        uint32_t sc = srcRow_[sx];
                        if (sc != COLORKEY_DEFAULT) {
                            dstRow_[dstX0 + ix] = sc;
                        }
                    }
                }
            } else {
                // ---- Opaque path (skip alpha=0) ----
                for (int iy = iy0; iy < iy1; iy++) {
                    int sy = srcY0 + iy;
                    if (sy < 0 || sy >= tset.height) continue;
                    const uint32_t *srcRow_ = tset.pixels + sy * tset.width;
                    uint32_t *dstRow_ = _framebuffer + (dstY0 + iy) * _width;
                    for (int ix = ix0; ix < ix1; ix++) {
                        int sx = srcX0 + ix;
                        if (sx < 0 || sx >= tset.width) continue;
                        uint32_t sc = srcRow_[sx];
                        if (COLOR_GET_A(sc) > 0) {
                            dstRow_[dstX0 + ix] = sc;
                        }
                    }
                }
            }
        }
    }
}


#endif // GAMELIB_IMPLEMENTATION

#endif // GAMELIB_SDL2_H
