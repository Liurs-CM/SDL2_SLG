// Example: GDI Text Demo (Chinese text support)
// Demonstrates the new DrawTextGDI function for rendering system fonts
//
// Compile: g++ -o 15_gdi_text.exe 15_gdi_text.cpp -mwindows

#include "../GameLib.h"

int main()
{
    GameLib game;
    game.Open(640, 480, "GDI Text Demo - 中文支持", true);
    game.ShowFps(true);

    int y = 10;

    while (!game.IsClosed()) {
        game.Clear(COLOR_BLACK);

        // Built-in 8x8 font (ASCII only)
        game.DrawText(10, y, "Built-in 8x8 Font (English only):", COLOR_WHITE);

        // GDI text with system fonts (supports Chinese!)
        game.DrawTextGDI(10, y + 30, "GDI 系统字体 - 支持中文！", COLOR_YELLOW, 24);
        game.DrawTextGDI(10, y + 60, "日本語も表示できます", COLOR_CYAN, "MS Gothic", 20);
        game.DrawTextGDI(10, y + 90, "Different sizes: 16px 20px 24px 32px", COLOR_GREEN, 16);
        game.DrawTextGDI(10, y + 110, "Different sizes: 16px 20px 24px 32px", COLOR_GREEN, 20);
        game.DrawTextGDI(10, y + 135, "Different sizes: 16px 20px 24px 32px", COLOR_GREEN, 24);
        game.DrawTextGDI(10, y + 165, "Different sizes: 16px 20px 24px 32px", COLOR_GREEN, 32);

        // Score display example
        int score = 12345;
        game.DrawTextGDI(10, y + 210, "分数:", COLOR_GOLD, 28);
        char scoreText[32];
        sprintf(scoreText, "%d", score);
        game.DrawTextGDI(100, y + 210, scoreText, COLOR_GOLD, 28);

        // Instructions
        game.DrawTextGDI(10, 440, "按 ESC 退出", COLOR_GRAY, 16);

        if (game.IsKeyDown(KEY_ESCAPE)) break;

        game.Update();
        game.WaitFrame(60);
    }

    return 0;
}
