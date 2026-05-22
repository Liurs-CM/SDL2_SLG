#include "TextureManager.h"
#include "RenderContext.h"
#include "SDL_image.h"
//#include "SDL_ttf.h"
#include <SDL2_gfxPrimitives.h>
#include <vector>
#include <iostream>

TextureManager* TextureManager::s_pInstance = 0;

bool TextureManager::load(std::string fileName,std::string id)
{ 
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str()); 
    if(pTempSurface == 0)
    {
        std::cout << "Error can't load img: " << fileName;
        return false;
    } 
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(RenderContext::get(), pTempSurface);
    SDL_FreeSurface(pTempSurface);
    if( pTexture != 0 )
    {
        m_textureMap[id] = pTexture;
        return true;
    }
    return false;
}

void TextureManager::draw(std::string id, vec pos, uint16_t width, uint16_t height, SDL_RendererFlip flip)
{
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    int x = (Uint32)pos.x;
    int y = (Uint32)pos.y;
    srcRect = {0, 0, width, height};
    dstRect = {x, y, width, height};
    SDL_RenderCopyEx(RenderContext::get(), m_textureMap[id], &srcRect, &dstRect, 0, 0, flip);
}

void TextureManager::drawRotat(std::string id, vec pos, int width, int height, int currentRow, int currentFrame, double angle, SDL_RendererFlip flip)
{
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    SDL_Point center;
    int x = (Uint32)pos.x;
    int y = (Uint32)pos.y;
    srcRect = {width * currentFrame, height * (currentRow - 1), width, height};
    dstRect = {x, y, width, height};
    //center = {x, y};
    //center = {x + width / 2, y + height / 2};
    center = {width / 2, height / 2};
    SDL_RenderCopyEx(RenderContext::get(), m_textureMap[id], &srcRect, &dstRect, angle, &center, flip);
}

void TextureManager::drawFrame(std::string id, vec pos, uint16_t width, uint16_t height, uint8_t currentRow, uint8_t currentFrame, int alpha, SDL_RendererFlip flip)
{
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    int x = (Uint32)pos.x;
    int y = (Uint32)pos.y;
    srcRect = {width * currentFrame, height * (currentRow - 1), width, height};
    dstRect = {x, y, width, height};
    SDL_SetTextureAlphaMod(m_textureMap[id], alpha);
    SDL_RenderCopyEx(RenderContext::get(), m_textureMap[id], &srcRect, &dstRect, 0, 0, flip);
}

void TextureManager::clearTextureMap()
{
    m_textureMap.clear();
}

void TextureManager::clearFromTextureMap(std::string id)
{
    m_textureMap.erase(id);
}

void TextureManager::drawTile(std::string id, uint8_t margin, uint8_t spacing, int x, int y, uint16_t width, uint16_t height, uint8_t currentRow, uint8_t currentFrame)
{
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    srcRect = {
        margin + (spacing + width) * currentFrame, 
        margin + (spacing + height) * currentRow, 
        width, height };
    dstRect = {x, y, width, height};
    SDL_RenderCopyEx(RenderContext::get(), m_textureMap[id], &srcRect, &dstRect, 0, 0, SDL_FLIP_NONE);
}

void TextureManager::drawLine(int x1, int y1, int x2, int y2, uint32_t color)
{
    lineColor(RenderContext::get(), x1, y1, x2, y2, color);
}

//=====================================================================
// Text Rendering
//=====================================================================

void TextureManager::drawText(int x, int y, const char *text, uint32_t color)
{
    if (!text) return;
    int ox = x;
    std::vector<SDL_Point> points;
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
                    points.push_back({ x + col, y + row});
                }
            }
        }
        const int pt_count = points.size();
        if( pt_count > 0) {
            Uint8 r = (color >> 16) & 0xFF;
            Uint8 g = (color >> 8)  & 0xFF;
            Uint8 b = color & 0xFF;
            SDL_Renderer* pRenderer = RenderContext::get();
            SDL_SetRenderDrawColor(pRenderer, r, g, b, 255);
            SDL_RenderDrawPoints(pRenderer, points.data(), pt_count);
        }
        x += 8;
    }
}

void TextureManager::drawTextScale(int x, int y, const char *text, uint32_t color, int scale)
{
    if (!text || scale <= 0) return;
    std::vector<SDL_Rect> rects;
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
                    rects.push_back({ x + col * scale, y + row * scale, scale, scale });
                }
            }
        }
        const int rect_count = rects.size();
        if (rect_count > 0) { 
            Uint8 r = (color >> 16) & 0xFF;
            Uint8 g = (color >> 8)  & 0xFF;
            Uint8 b = color & 0xFF;
            SDL_Renderer* pRenderer = RenderContext::get();
            SDL_SetRenderDrawColor(pRenderer, r, g, b, 255);
            SDL_RenderFillRects(pRenderer, rects.data(), rect_count); 
        }
        x += 8 * scale;
    }
}

void TextureManager::drawPrintf(int x, int y, uint32_t color, const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    buf[sizeof(buf) - 1] = '\0';
    TextureManager::drawText(x, y, buf, color);
}

//千问推荐Triangle Fan
void TextureManager::drawCircle(int cx, int cy, int radius, bool Fill, uint32_t color)
{
    if(radius <= 0) return;
    if(Fill) {
        // 动态计算分段数：保证每段弧长约 ≤ 2 像素
        const int segments = std::max(8, static_cast<int>(M_PI * radius / 2.0f));
        // 分配顶点（中心 + 圆周）
        const int numVertices = segments + 2; // 中心 + segments+1 个圆周点
        const int numIndices = segments * 3;  // 每个三角形3个索引
        std::vector<SDL_Vertex> vertices(numVertices);
        std::vector<int> indices(numIndices);
        // 中心点
        vertices[0].position = { static_cast<float>(cx), static_cast<float>(cy) };
        Uint8 r = (color >> 16) & 0xFF;
        Uint8 g = (color >> 8)  & 0xFF;
        Uint8 b = color & 0xFF;
        vertices[0].color = { r, g, b, 255 };
        // 圆周点（闭合扇形）
        const float step = 2.0f * static_cast<float>(M_PI) / segments;
        for (int i = 0; i <= segments; ++i) {
            float angle = i * step;
            vertices[i + 1].position = {
                static_cast<float>(cx + radius * std::cos(angle)),
                static_cast<float>(cy + radius * std::sin(angle))
            };
            vertices[i + 1].color = { r, g, b, 255 };
        }
        // 构建索引：每个三角形由 (0, i+1, i+2) 构成，共 segments 个三角形
        // 总索引数 = segments * 3
        for (int i = 0; i < segments; ++i) {
            indices[i * 3 + 0] = 0;           // 中心
            indices[i * 3 + 1] = i + 1;       // 当前圆周点
            indices[i * 3 + 2] = i + 2;       // 下一个圆周点（自动闭合）
        }
        // 单次 GPU 调用绘制整个圆
        SDL_RenderGeometry(RenderContext::get(), nullptr, vertices.data(), numVertices, indices.data(), numIndices);
    }

    else {
        const int segments = std::max(16, static_cast<int>(M_PI * radius / 1.5f));
        const int numPoints = segments + 1; // 中心 + segments+1 个圆周点
        std::vector<SDL_Point> points(numPoints);
        const float step = 2.0f * static_cast<float>(M_PI) / segments;
        for (int i = 0; i <= segments; ++i) {
            float angle = i * step;
            points[i] = {
                static_cast<int>(cx + radius * std::cos(angle)),
                static_cast<int>(cy + radius * std::sin(angle))
            };
        }
        Uint8 r = (color >> 16) & 0xFF;
        Uint8 g = (color >> 8)  & 0xFF;
        Uint8 b = color & 0xFF;
        SDL_Renderer* pRenderer = RenderContext::get();
        SDL_SetRenderDrawColor(pRenderer, r, g, b, 255);
        SDL_RenderDrawLines(pRenderer, points.data(), numPoints);
    }
}

void TextureManager::drawGrid(int x, int y, int rows, int cols, int cellSize, uint32_t color)
{
    for (int r = 0; r <= rows; r++) {
        drawLine(x, y + r * cellSize, x + cols * cellSize, y + r * cellSize, color);
    }
    for (int c = 0; c <= cols; c++) {
        drawLine(x + c * cellSize, y, x + c * cellSize, y + rows * cellSize, color);
    }
}

void TextureManager::initHealthBarTexture()
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
    m_textureMap["HealthGradient"] = SDL_CreateTextureFromSurface(RenderContext::get(), s);
    SDL_FreeSurface(s);
}

void TextureManager::drawHealthBar(
        vec pos,               // 屏幕位置
        int width, int height,      // 血条尺寸
        int currentHP, int maxHP)   // 绝对血量值
{
    SDL_Renderer* pRenderer = RenderContext::get();
    int x = (Uint32)pos.x;
    int y = (Uint32)pos.y;
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
        SDL_RenderCopy(pRenderer, m_textureMap["HealthGradient"], &src, &dst);
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

