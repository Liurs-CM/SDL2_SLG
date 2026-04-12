#include "Shape.h"
#include "GameLib.h"
#include "TextureManager.h"
#include "RenderContext.h"
#include <SDL2_gfxPrimitives.h>

Shape* Shape::s_pInstance = 0;

void Shape::Shape_genrate()
{
    SDL_Renderer* pRenderer = RenderContext::get();
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

    TheTextureManager::Instance()->draw("animate", 0,0, 128, 82);
    SDL_SetRenderDrawColor(pRenderer, 255,0,255,255);
}
