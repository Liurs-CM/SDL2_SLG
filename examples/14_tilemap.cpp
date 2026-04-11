// 14_tilemap.cpp - Tilemap Two-Layer Scrolling
//
// Two-layer scrolling demo:
//   - Background layer: far view (sky, clouds, hills), half speed for parallax effect
//   - Foreground layer: ground, platforms, bricks, stairs, full speed
//   - Character sprite loaded from assets/, camera follows
//   - Decorative trees loaded from assets/
// Foreground tileset and sprites loaded from assets/ folder (PNG with 0xFF00FF color key).
// Background tileset is still made by code (no matching asset).
//
// Controls: <- -> move character, ESC to quit
// Learn: CreateTilemap, SetTile, DrawTilemap, LoadSprite, DrawSpriteEx,
//        SPRITE_COLORKEY, SPRITE_FLIP_H, parallax scrolling
//
// Compile: g++ -o 14_tilemap.exe 14_tilemap.cpp -mwindows

#include "../GameLib.h"

// Tile size
static const int TS = 16;

// Character sprite dimensions
static const int CHAR_W = 32;
static const int CHAR_FOOT = 44;  // y of boot soles in sprite

// Foreground tile IDs
enum { FG_GRASS = 0, FG_DIRT = 1, FG_BRICK = 2, FG_STONE = 3 };

// Background tile IDs
enum { BG_SKY = 0, BG_CLOUD_L = 1, BG_CLOUD_R = 2, BG_HILL_TOP = 3, BG_HILL = 4 };

// Helper: fill a rectangle inside a sprite
static void spriteFill(GameLib &g, int id, int x0, int y0, int w, int h, uint32_t c)
{
    for (int y = y0; y < y0 + h; y++)
        for (int x = x0; x < x0 + w; x++)
            g.SetSpritePixel(id, x, y, c);
}

// Make background tileset (5 tiles in a row = 80x16 sprite)
static int makeBgTileset(GameLib &game)
{
    int id = game.CreateSprite(5 * TS, TS);
    uint32_t sky = COLOR_RGB(135, 206, 235);
    int ox;

    // Sky
    spriteFill(game, id, BG_SKY * TS, 0, TS, TS, sky);

    // Cloud (left half)
    ox = BG_CLOUD_L * TS;
    spriteFill(game, id, ox, 0, TS, TS, sky);
    spriteFill(game, id, ox + 4, 6, 12, 6, COLOR_WHITE);
    spriteFill(game, id, ox + 6, 3, 8, 5, COLOR_WHITE);

    // Cloud (right half)
    ox = BG_CLOUD_R * TS;
    spriteFill(game, id, ox, 0, TS, TS, sky);
    spriteFill(game, id, ox, 6, 12, 6, COLOR_WHITE);
    spriteFill(game, id, ox + 2, 3, 8, 5, COLOR_WHITE);

    // Hill top (triangle shape)
    ox = BG_HILL_TOP * TS;
    spriteFill(game, id, ox, 0, TS, TS, sky);
    for (int y = 0; y < TS; y++) {
        int hw = 1 + y * 7 / 15;
        for (int x = 8 - hw; x < 8 + hw; x++)
            if (x >= 0 && x < TS)
                game.SetSpritePixel(id, ox + x, y, COLOR_RGB(70, 130, 70));
    }

    // Hill body
    spriteFill(game, id, BG_HILL * TS, 0, TS, TS, COLOR_RGB(70, 130, 70));

    return id;
}

// Fill a row of tiles on the map
static void fillRow(GameLib &g, int map, int row, int c0, int c1, int tileId)
{
    for (int c = c0; c < c1; c++) g.SetTile(map, c, row, tileId);
}

// Dig a hole in the ground
static void digGround(GameLib &g, int map, int c0, int c1, int groundRow, int depth)
{
    for (int c = c0; c < c1; c++)
        for (int r = groundRow; r < groundRow + depth; r++)
            g.SetTile(map, c, r, -1);
}

int main()
{
    GameLib game;
    const int SW = 640, SH = 480;
    game.Open(SW, SH, "14 - Tilemap Two-Layer Scrolling", true);
    game.ShowFps(true);

    // ---- Load tilesets and sprites ----
    int fgTS = game.LoadSprite("../assets/tileset.png");      // 64x16, 4 tiles
    int bgTS = makeBgTileset(game);
    int charSpr = game.LoadSprite("../assets/character.png"); // 32x48
    int treeSpr = game.LoadSprite("../assets/tree.png");      // 32x64

    // ---- Create maps ----
    const int FG_C = 80, FG_R = 30;   // Foreground 80x30 = 1280x480
    const int BG_C = 60, BG_R = 30;   // Background 60x30 = 960x480
    int fgMap = game.CreateTilemap(FG_C, FG_R, TS, fgTS);
    int bgMap = game.CreateTilemap(BG_C, BG_R, TS, bgTS);

    // ---- Fill background ----
    for (int r = 0; r < BG_R; r++)
        fillRow(game, bgMap, r, 0, BG_C, BG_SKY);
    // Clouds
    int cloudPos[][2] = { {5,4}, {18,3}, {33,5}, {48,2}, {55,6} };
    for (int i = 0; i < 5; i++) {
        game.SetTile(bgMap, cloudPos[i][0],     cloudPos[i][1], BG_CLOUD_L);
        game.SetTile(bgMap, cloudPos[i][0] + 1, cloudPos[i][1], BG_CLOUD_R);
    }
    // Hills (3 groups at different heights)
    int hills[][3] = { {8, 13, 24}, {22, 30, 22}, {40, 47, 25} }; // {start col, end col, top row}
    for (int i = 0; i < 3; i++) {
        fillRow(game, bgMap, hills[i][2], hills[i][0], hills[i][1], BG_HILL_TOP);
        for (int r = hills[i][2] + 1; r < BG_R; r++)
            fillRow(game, bgMap, r, hills[i][0], hills[i][1], BG_HILL);
    }

    // ---- Fill foreground ----
    // Ground: bottom 3 rows (grass + dirt + dirt)
    fillRow(game, fgMap, FG_R - 3, 0, FG_C, FG_GRASS);
    fillRow(game, fgMap, FG_R - 2, 0, FG_C, FG_DIRT);
    fillRow(game, fgMap, FG_R - 1, 0, FG_C, FG_DIRT);
    // Holes
    digGround(game, fgMap, 20, 23, FG_R - 3, 3);
    digGround(game, fgMap, 48, 51, FG_R - 3, 3);
    // Floating brick platforms
    fillRow(game, fgMap, 23, 12, 18, FG_BRICK);
    fillRow(game, fgMap, 20, 30, 36, FG_BRICK);
    fillRow(game, fgMap, 24, 42, 47, FG_BRICK);
    // Stone stairs (5 steps up from ground)
    for (int step = 0; step < 5; step++)
        fillRow(game, fgMap, FG_R - 4 - step, 60, 61 + step, FG_STONE);
    // High stone platform
    fillRow(game, fgMap, 17, 66, 73, FG_STONE);

    // ---- Trees (decoration) ----
    int treeX[] = { 5*TS, 25*TS, 38*TS, 55*TS, 70*TS };
    int treeCount = 5;
    int treeY = (FG_R - 3) * TS - 61;

    // ---- Player ----
    float playerX = 48.0f;
    float playerY = (float)((FG_R - 3) * TS - CHAR_FOOT); // Stand on ground
    float speed = 180.0f;
    int facing = 1;    // 1=right, -1=left

    while (!game.IsClosed()) {
        float dt = game.GetDeltaTime();
        if (dt > 0.05f) dt = 0.05f; // Prevent first frame jump

        // ---- Input ----
        if (game.IsKeyDown(KEY_RIGHT) || game.IsKeyDown(KEY_D)) {
            playerX += speed * dt;
            facing = 1;
        }
        if (game.IsKeyDown(KEY_LEFT) || game.IsKeyDown(KEY_A)) {
            playerX -= speed * dt;
            facing = -1;
        }
        // Limit player range
        if (playerX < 0) playerX = 0;
        if (playerX > FG_C * TS - CHAR_W) playerX = (float)(FG_C * TS - CHAR_W);

        // ---- Camera follow ----
        float cameraX = playerX - SW / 2 + CHAR_W / 2;
        float maxCam = (float)(FG_C * TS - SW);
        if (cameraX < 0) cameraX = 0;
        if (cameraX > maxCam) cameraX = maxCam;

        // ---- Draw ----
        game.Clear(COLOR_RGB(135, 206, 235));

        // Background layer (half speed = parallax)
        game.DrawTilemap(bgMap, -(int)(cameraX * 0.5f), 0);

        // Foreground layer (full speed)
        int camX = (int)cameraX;
        game.DrawTilemap(fgMap, -camX, 0);

        // ---- Draw trees ----
        for (int i = 0; i < treeCount; i++)
            game.DrawSpriteEx(treeSpr, treeX[i] - camX, treeY, SPRITE_COLORKEY);

        // ---- Draw player ----
        int px = (int)playerX - camX;
        int py = (int)playerY;
        int pflags = SPRITE_COLORKEY | ((facing < 0) ? SPRITE_FLIP_H : 0);
        game.DrawSpriteEx(charSpr, px, py, pflags);

        // ---- HUD ----
        game.FillRect(0, 0, 230, 38, COLOR_RGB(15, 15, 25));
        game.DrawText(8, 6, "<- -> Move    ESC Quit", COLOR_WHITE);
        game.DrawPrintf(8, 20, COLOR_LIGHT_GRAY,
            "Camera: %d   World: %dx%d", camX, FG_C * TS, FG_R * TS);

        if (game.IsKeyPressed(KEY_ESCAPE)) break;
        game.Update();
        game.WaitFrame(60);
    }

    game.FreeTilemap(fgMap);
    game.FreeTilemap(bgMap);
    game.FreeSprite(charSpr);
    game.FreeSprite(treeSpr);
    return 0;
}
