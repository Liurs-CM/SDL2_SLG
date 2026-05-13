#include "Shape.h"
#include "TextureManager.h"
#include "GameLib.h"
#include <SDL2_gfxPrimitives.h>

Shape* Shape::s_pInstance = 0;

void Shape::Shape_genrate()
{
    // Grid
    //int gridW = GRID_COLS * CELL_SIZE;
    //int gridH = GRID_ROWS * CELL_SIZE;
    TheTextureManager::Instance()->drawGrid(GRID_X, GRID_Y, GRID_ROWS, GRID_COLS, CELL_SIZE, COLOR_DARK_GRAY);
    // --- Row 1: Pixels and Lines ---
    TheTextureManager::Instance()->drawText(20, 15, "Pixels", COLOR_WHITE);
    SDL_SetRenderDrawColor(pRenderer, 
            COLOR_GET_R(COLOR_BLACK), COLOR_GET_G(COLOR_BLACK), 
            COLOR_GET_B(COLOR_BLACK), COLOR_GET_A(COLOR_BLACK));
    //SDL_SetRenderDrawColor(pRenderer, 0,0,0,255);
    SDL_Rect rect = {420, 330, 80, 60};
    SDL_RenderFillRect(pRenderer, &rect); 
    for (int i = 0; i < 50; i++) {
        int px = 420 + GameLib::Random(0, 80);
        int py = 330 + GameLib::Random(0, 60);
        Uint8 r= GameLib::Random(100,255);
        Uint8 g= GameLib::Random(100,255);
        Uint8 b= GameLib::Random(100,255);
        SDL_SetRenderDrawColor(pRenderer, r, g, b, 255);
        SDL_RenderDrawPoint(pRenderer, px, py);
    }

    TheTextureManager::Instance()->drawText(140, 315, "Lines", COLOR_WHITE);
    lineColor(pRenderer, 140, 335, 230, 335, COLOR_RED);
    lineColor(pRenderer, 140, 335, 230, 385, COLOR_GREEN);
    lineColor(pRenderer, 140, 335, 185, 385, COLOR_BLUE);
    lineColor(pRenderer, 140, 360, 230, 360, COLOR_YELLOW);

    TheTextureManager::Instance()->drawTextScale(100,10, "Hi, windows->SDL2!", COLOR_BLACK, 3);
    TheTextureManager::Instance()->drawText(10, 180, "hi, test drawtext", COLOR_BLUE);
    TheTextureManager::Instance()->drawPrintf(10, 200, COLOR_YELLOW, "hi, test printf Time: %2.f s", SDL_GetTicks() / 1000.0f);
    TheTextureManager::Instance()->drawCircle(75, 300, 50);
    TheTextureManager::Instance()->drawCircle(55, 300, 30);
    TheTextureManager::Instance()->drawCircle(70, 300, 10, true);
    circleColor(pRenderer, 105, 300, 20, COLOR_BLACK);
    filledCircleColor(pRenderer, 100, 300, 10, COLOR_BLACK);
    arcColor(pRenderer, 75, 300, 40, 50, 110, COLOR_BLACK);

    TheTextureManager::Instance()->draw("cat", Vector2D(20,20), 36, 36);
    SDL_SetRenderDrawColor(pRenderer, 255,0,255,255);

    initHealthBarSystem();
    drawHealthBar(204, 104, 20, 4, 500, 1000);
    drawHealthBar(204, 128, 50, 5, 5 * m_currentAnim.globalFrame_, 1000);
    drawHealthBar(204, 152, 80, 8, 1000 - 5 * m_currentAnim.globalFrame_, 1000);
}

void Shape::initHealthBarSystem() 
{
    // 创建 256x1 渐变纹理（红→黄→绿）
    SDL_Surface* s = SDL_CreateRGBSurface(0, 256, 1, 32, 0, 0, 0, 0);
    for (int x = 0; x < 256; ++x) {
        float t = x / 255.0f;
        Uint8 r = (t < 0.5f) ? 255 : (Uint8)(255 * (1 - (t - 0.5f) * 2));
        Uint8 g = (t < 0.5f) ? (Uint8)(255 * t * 2) : 255;
        Uint8 b = 0;
        ((Uint32*)s->pixels)[x] = SDL_MapRGB(s->format, r, g, b);
    }
    gHealthGradient = SDL_CreateTextureFromSurface(pRenderer, s);
    SDL_FreeSurface(s);
}


void Shape::drawHealthBar(
        int x, int y,               // 屏幕位置
        int width, int height,      // 血条尺寸
        int currentHP, int maxHP)   // 绝对血量值
{
    SDL_Renderer* pRenderer = RenderContext::get();
    const int SEG_SIZE = 100;
    const int MAX_SEG = 10;

    // 1. 计算段数（按绝对值）
    int curSeg = std::min(currentHP / SEG_SIZE, MAX_SEG);
    int maxSeg = std::min(maxHP / SEG_SIZE, MAX_SEG);
    if (maxSeg == 0) maxSeg = 1;
    if (currentHP > 0 && curSeg == 0) curSeg = 1;

    float ratio = (float)curSeg / maxSeg;

    // 2. 背景（可选）
    SDL_SetRenderDrawColor(pRenderer, 127, 127, 127, 255);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(pRenderer, &rect);

    // 3. 渐变前景
    if (ratio > 0.0f) {
        SDL_Rect src = {0, 0, (int)(256 * ratio), 1};
        SDL_Rect dst = {x, y, (int)(width * ratio), height};
        SDL_RenderCopy(pRenderer, gHealthGradient, &src, &dst);
    }

    // 4. ★ 1像素黑边框 ★
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(pRenderer, &rect);

    // 5. ★ >1000 高亮（白边）★
    if (maxHP >= 1000) {
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
        rect = {x - 1, y - 1, width + 2, height + 2};
        SDL_RenderDrawRect(pRenderer, &rect);
    }
    
    // 6. ★ 1像素黑点倒角 ★
    SDL_SetRenderDrawColor(pRenderer, 127, 127, 127, 255);
    SDL_RenderDrawPoint(pRenderer, x - 1, y - 1);
    SDL_RenderDrawPoint(pRenderer, x - 1, y + height);
    SDL_RenderDrawPoint(pRenderer, x + width, y - 1);
    SDL_RenderDrawPoint(pRenderer, x + width, y + height);
}

