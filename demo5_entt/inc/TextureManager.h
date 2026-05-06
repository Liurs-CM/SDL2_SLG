#ifndef __TextureManager__
#define __TextureManager__
#include "Vector2D.h"
#include "GameLib.h"
#include "SDL.h"
#include <map>
#include <string>
#include <cmath>
#include <algorithm>

class TextureManager
{
    public:
        bool load(std::string fileName,std::string id);
        void draw(std::string id, Vector2D pos, uint16_t width, uint16_t height, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void drawRotat(std::string id, Vector2D pos, int width, int height, int currentRow, int currentFrame, double angle, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void drawFrame(std::string id, Vector2D pos, uint16_t width, uint16_t height, uint8_t currentRow, uint8_t currentFrame, int alpha = 255, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void drawLine(int x1, int y1, int x2, int y2, uint32_t color = COLOR_BLACK);
        void drawText(int x, int y, const char *text, uint32_t color = COLOR_BLACK);
        void drawTextScale(int x, int y, const char *text, uint32_t color = COLOR_BLACK, int scale = 1);
        void drawPrintf(int x, int y, uint32_t color, const char *fmt, ...);
        void drawCircle(int cx, int cy, int radius, bool Fill = false, uint32_t color = COLOR_BLACK);
        void drawTile(std::string id, uint8_t margin, uint8_t spacing, int x, int y, uint16_t width, uint16_t height, uint8_t currentRow, uint8_t currentFrame);
        void drawGrid(int x, int y, int rows, int cols, int cellSize, uint32_t color);
        void clearTextureMap();
        void clearFromTextureMap(std::string id);
        std::map<std::string, SDL_Texture*> getTextureMap() { return m_textureMap; }
        static TextureManager* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new TextureManager();
                return s_pInstance;
            }
            return s_pInstance;
        }

    private:
        TextureManager() {}
        std::map<std::string, SDL_Texture*> m_textureMap;
        static TextureManager* s_pInstance;
};

typedef TextureManager TheTextureManager;

#endif /* defined(__TextureManager__) */
