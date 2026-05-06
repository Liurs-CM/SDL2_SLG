#ifndef __ENTT_GAME__
#define __ENTT_GAME__

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>

// 游戏配置常量
constexpr uint8_t FPS = 165;
constexpr uint16_t DELAY_TIME_MS = 1000 / FPS;

constexpr uint16_t SCR_W = 640;
constexpr uint16_t SCR_H = 480;
#define SCR_X       1000
#define SCR_Y       500
#define MAP_W       3200
#define MAP_H       2400

#define GRID_X      10
#define GRID_Y      30
#define GRID_ROWS   12
#define GRID_COLS   15
#define CELL_SIZE   32
#define MAX_SNAKE   400

// 颜色常量
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

#define COLOR_RGB(r, g, b)     ((uint32_t)(0xFF000000 | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))
#define COLOR_ARGB(a, r, g, b) ((uint32_t)((((a) & 0xFF) << 24) | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))

#define COLOR_GET_A(c)    (((c) >> 24) & 0xFF)
#define COLOR_GET_R(c)    (((c) >> 16) & 0xFF)
#define COLOR_GET_G(c)    (((c) >> 8) & 0xFF)
#define COLOR_GET_B(c)    ((c) & 0xFF)

// 经典8x8位图字体
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

// 向量2D组件
struct Vector2D {
    float x, y;

    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    float length() const {
        return sqrtf(x*x + y*y);
    }

    Vector2D get_normalize() const {
        float len = length();
        if(len == 0) return Vector2D(0, 0);
        return Vector2D(x / len, y / len);
    }

    Vector2D operator*(float scalar) {
        return Vector2D(x * scalar, y * scalar);
    }
    Vector2D& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vector2D operator/(float scalar) {
        return Vector2D(x / scalar, y / scalar);
    }
    Vector2D& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float getX() const { return x; }
    float getY() const { return y; }
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
};

// 输入处理组件
struct InputComponent {
    std::array<bool, SDL_NUM_SCANCODES> keystates;
    std::array<bool, 3> mouseButtonStates; // LEFT, MIDDLE, RIGHT
    Vector2D mousePosition;
    std::unordered_set<SDL_Scancode> keysPressedThisFrame;
    InputComponent() : mouseButtonStates{{false, false, false}} {
        keystates.fill(false);
    }
};

// 渲染上下文
struct RenderContext {
    static SDL_Renderer* s_renderer;
    static void set(SDL_Renderer* renderer) { s_renderer = renderer; }
    static SDL_Renderer* get() { return s_renderer; }
};

// 纹理管理器
class TextureManager {
    private:
        static TextureManager* s_pInstance;
        std::unordered_map<std::string, SDL_Texture*> m_textureMap;

    public:
        static TextureManager* Instance() {
            if(s_pInstance == 0) {
                s_pInstance = new TextureManager();
            }
            return s_pInstance;
        }

        bool load(std::string fileName, std::string id) {
            SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
            if(pTempSurface == 0) {
                std::cout << "Error can't load img: " << fileName;
                return false;
            }
            SDL_Texture* pTexture = SDL_CreateTextureFromSurface(RenderContext::get(), pTempSurface);
            SDL_FreeSurface(pTempSurface);
            if(pTexture != 0) {
                m_textureMap[id] = pTexture;
                return true;
            }
            return false;
        }

        void draw(std::string id, Vector2D pos, uint16_t width, uint16_t height, SDL_RendererFlip flip = SDL_FLIP_NONE) {
            SDL_Rect srcRect = {0, 0, width, height};
            SDL_Rect dstRect = {(int)pos.x, (int)pos.y, width, height};
            SDL_RenderCopyEx(RenderContext::get(), m_textureMap[id], &srcRect, &dstRect, 0, 0, flip);
        }

        void drawFrame(std::string id, Vector2D pos, uint16_t width, uint16_t height, 
                uint8_t currentRow, uint8_t currentFrame, int alpha = 255, 
                SDL_RendererFlip flip = SDL_FLIP_NONE) {
            SDL_Rect srcRect = {width * currentFrame, height * (currentRow - 1), width, height};
            SDL_Rect dstRect = {(int)pos.x, (int)pos.y, width, height};
            SDL_SetTextureAlphaMod(m_textureMap[id], alpha);
            SDL_RenderCopyEx(RenderContext::get(), m_textureMap[id], &srcRect, &dstRect, 0, 0, flip);
        }

        void drawTile(std::string id, uint8_t margin, uint8_t spacing, int x, int y, 
                uint16_t width, uint16_t height, uint8_t currentRow, uint8_t currentFrame) {
            SDL_Rect srcRect = {
                margin + (spacing + width) * currentFrame, 
                margin + (spacing + height) * currentRow, 
                width, height
            };
            SDL_Rect dstRect = {x, y, width, height};
            SDL_RenderCopyEx(RenderContext::get(), m_textureMap[id], &srcRect, &dstRect, 0, 0, SDL_FLIP_NONE);
        }

        void drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
            lineColor(RenderContext::get(), x1, y1, x2, y2, color);
        }

        void drawText(int x, int y, const char *text, uint32_t color) {
            if (!text) return;
            int ox = x;
            std::vector<SDL_Point> points;
            for (const char *p = text; *p; p++) {
                unsigned char ch = (unsigned char)*p;
                if (ch == '\n') {
                    x = ox;
                    y += 10;
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

        void drawPrintf(int x, int y, uint32_t color, const char *fmt, ...) {
            char buf[1024];
            va_list args;
            va_start(args, fmt);
            vsnprintf(buf, sizeof(buf), fmt, args);
            va_end(args);
            buf[sizeof(buf) - 1] = '\0';
            drawText(x, y, buf, color);
        }

        void clearTextureMap() {
            m_textureMap.clear();
        }

        void clearFromTextureMap(std::string id) {
            m_textureMap.erase(id);
        }

        SDL_Texture* getTexture(std::string id) {
            auto it = m_textureMap.find(id);
            if(it != m_textureMap.end()) {
                return it->second;
            }
            return nullptr;
        }
};

// 声音管理器
enum sound_type { SOUND_MUSIC, SOUND_SFX };

class SoundManager {
    private:
        static SoundManager* s_pInstance;
        std::unordered_map<std::string, Mix_Music*> m_music;
        std::unordered_map<std::string, Mix_Chunk*> m_sfxs;

    public:
        static SoundManager* Instance() {
            if(s_pInstance == 0) {
                s_pInstance = new SoundManager();
            }
            return s_pInstance;
        }

        SoundManager() {
            Mix_OpenAudio(22050, AUDIO_S16, 2, (4096 / 2));
        }

        ~SoundManager() {
            Mix_CloseAudio();
        }

        bool load(std::string fileName, std::string id, sound_type type) {
            if(type == SOUND_MUSIC) {
                Mix_Music* pMusic = Mix_LoadMUS(fileName.c_str());
                if(pMusic == 0) {
                    std::cout << "Could not load music: ERROR - " << Mix_GetError() << std::endl;
                    return false;
                }
                m_music[id] = pMusic;
                return true;
            }
            else if(type == SOUND_SFX) {
                Mix_Chunk* pChunk = Mix_LoadWAV(fileName.c_str());
                if(pChunk == 0) {
                    std::cout << "Could not load SFX: ERROR - " << Mix_GetError() << std::endl;
                    return false;
                }
                m_sfxs[id] = pChunk;
                return true;
            }
            return false;
        }

        void playMusic(std::string id, int loop) {
            Mix_PlayMusic(m_music[id], loop);
        }

        void playSound(std::string id, int loop) {
            Mix_PlayChannel(-1, m_sfxs[id], loop);
        }
};

// 相机组件
struct CameraComponent {
    Vector2D position;
    uint16_t width, height;
    float shift_speed = 0.5f;

    CameraComponent() : position(0, 0), width(SCR_W), height(SCR_H) {}

    void follow(const Vector2D& target) {
        position = Vector2D(target.x - width / 2, target.y - height / 2);
        position.x = std::max(0.0f, std::min(position.x, float(MAP_W - width)));
        position.y = std::max(0.0f, std::min(position.y, float(MAP_H - height)));
    }
};

// 游戏对象基本组件
struct PositionComponent {
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

    PositionComponent(float x = 0, float y = 0) : position(x, y), velocity(0, 0), acceleration(0, 0) {}
};

struct TransformComponent {
    Vector2D worldPosition;
    Vector2D screenPosition;

    TransformComponent() : worldPosition(0, 0), screenPosition(0, 0) {}
};

struct RenderableComponent {
    std::string textureID;
    uint16_t width, height;
    uint8_t currentRow, currentFrame;
    float angle;
    int alpha;
    bool visible;

    RenderableComponent(const std::string& texID = "", uint16_t w = 0, uint16_t h = 0) 
        : textureID(texID), width(w), height(h), currentRow(1), currentFrame(0), 
        angle(0), alpha(255), visible(true) {}
};

struct AnimationComponent {
    uint8_t numFrames;
    uint8_t animSpeed;
    uint8_t delayFrame;
    uint8_t animTimer;
    uint8_t globalFrame;

    AnimationComponent(uint8_t frames = 1, uint8_t speed = 4) 
        : numFrames(frames), animSpeed(speed), delayFrame(FPS / speed), 
        animTimer(0), globalFrame(0) {}
};

struct HealthComponent {
    int currentHP;
    int maxHP;
    HealthComponent(int hp = 100) : currentHP(hp), maxHP(hp) {}
    void takeDamage(int damage) {
        currentHP = std::max(0, currentHP - damage);
    }
    void heal(int amount) {
        currentHP = std::min(maxHP, currentHP + amount);
    }
};

struct PlayerComponent {
    int lives;
    bool isMoving;
    Vector2D targetPosition;
    enum Direction { UP, DOWN, LEFT, RIGHT } currentDirection;
    PlayerComponent() : lives(3), isMoving(false), targetPosition(0, 0), currentDirection(RIGHT) {}
};

struct EnemyComponent {
    int health;
    Vector2D homePosition;
    EnemyComponent(int h = 1) : health(h) {}
};

struct CollidableComponent {
    bool isCollidable;
    std::vector<int> collisionTiles;
    CollidableComponent() : isCollidable(true) {}
};

struct UIComponent {
    bool isUI;
    int zIndex;
    UIComponent(bool ui = true, int z = 0) : isUI(ui), zIndex(z) {}
};

// 系统类
class System {
    public:
        virtual ~System() = default;
        virtual void update(entt::registry& registry, float deltaTime) = 0;
        virtual void render(entt::registry& registry, SDL_Renderer* renderer) = 0;
};

// 输入系统
class InputSystem : public System {
    private:
        InputComponent inputComp;
    public:
        InputSystem() {
            // 初始化键盘状态
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            for(int i = 0; i < SDL_NUM_SCANCODES; i++) {
                inputComp.keystates[i] = keystate[i];
            }
        }
        void update(entt::registry& registry, float deltaTime) override 
        {
            inputComp.keysPressedThisFrame.clear();
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                switch(event.type) {
                    case SDL_QUIT:
                        registry.ctx().emplace<bool>(false); // 标记游戏结束
                        break;
                    case SDL_MOUSEMOTION:
                        inputComp.mousePosition = Vector2D(event.motion.x, event.motion.y);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if(event.button.button == SDL_BUTTON_LEFT) inputComp.mouseButtonStates[0] = true;
                        if(event.button.button == SDL_BUTTON_MIDDLE) inputComp.mouseButtonStates[1] = true;
                        if(event.button.button == SDL_BUTTON_RIGHT) inputComp.mouseButtonStates[2] = true;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        if(event.button.button == SDL_BUTTON_LEFT) inputComp.mouseButtonStates[0] = false;
                        if(event.button.button == SDL_BUTTON_MIDDLE) inputComp.mouseButtonStates[1] = false;
                        if(event.button.button == SDL_BUTTON_RIGHT) inputComp.mouseButtonStates[2] = false;
                        break;
                    case SDL_KEYDOWN:
                        inputComp.keystates[event.key.keysym.scancode] = true;
                        if(event.key.repeat == 0) {
                            inputComp.keysPressedThisFrame.insert(static_cast<SDL_Scancode>(event.key.keysym.scancode));
                        }
                        break;
                    case SDL_KEYUP:
                        inputComp.keystates[event.key.keysym.scancode] = false;
                        break;
                    default:
                        break;
                }
            }
            // 将输入组件存储到注册表中
            registry.ctx().emplace<InputComponent>(inputComp);
        }
        void render(entt::registry& registry, SDL_Renderer* renderer) override {}
        bool isKeyDown(SDL_Scancode key) const {
            return inputComp.keystates[key];
        }
        bool isKeyPressed(SDL_Scancode key) const {
            return inputComp.keysPressedThisFrame.count(key) > 0;
        }
        Vector2D getMousePosition() const {
            return inputComp.mousePosition;
        }
        bool getMouseButtonState(int button) const {
            return inputComp.mouseButtonStates[button];
        }
};

// 物理系统
class PhysicsSystem : public System {
    public:
        void update(entt::registry& registry, float deltaTime) override {
            auto cameraView = registry.view<CameraComponent>();
            Vector2D cameraPos(0, 0);
            if(!cameraView.empty()) {
                auto entity = cameraView.front();
                cameraPos = cameraView.get<CameraComponent>(entity).position;
            }
            auto view = registry.view<PositionComponent, TransformComponent>();
            for(auto entity : view) {
                auto& pos = view.get<PositionComponent>(entity);
                auto& transform = view.get<TransformComponent>(entity);
                // 更新物理位置
                pos.velocity += pos.acceleration;
                pos.position += pos.velocity;
                // 计算屏幕位置
                transform.screenPosition = pos.position - cameraPos;
                transform.worldPosition = pos.position;
            }
        }
        void render(entt::registry& registry, SDL_Renderer* renderer) override {}
};

// 动画系统
class AnimationSystem : public System {
    public:
        void update(entt::registry& registry, float deltaTime) override {
            auto view = registry.view<AnimationComponent, RenderableComponent>();
            for(auto entity : view) {
                auto& anim = view.get<AnimationComponent>(entity);
                auto& renderable = view.get<RenderableComponent>(entity);
                anim.animTimer++;
                if(anim.animTimer >= anim.delayFrame) {
                    anim.animTimer = 0;
                    anim.globalFrame = (anim.globalFrame + 1) % FPS;
                    renderable.currentFrame = (renderable.currentFrame + 1) % anim.numFrames;
                }
            }
        }
        void render(entt::registry& registry, SDL_Renderer* renderer) override {}
};

// 渲染系统
class RenderSystem : public System {
    public:
        void update(entt::registry& registry, float deltaTime) override {}
        void render(entt::registry& registry, SDL_Renderer* renderer) override {
            // 清除屏幕
            SDL_RenderClear(renderer);
            // 按照z-index排序渲染UI
            std::vector<entt::entity> uiEntities;
            registry.view<UIComponent, RenderableComponent>().each([&](auto entity, auto& ui, auto& renderable) {
                    uiEntities.push_back(entity);
                    });
            std::sort(uiEntities.begin(), uiEntities.end(), [&](entt::entity a, entt::entity b) {
                    return registry.get<UIComponent>(a).zIndex < registry.get<UIComponent>(b).zIndex;
                    });
            // 渲染非UI实体
            auto renderableView = registry.view<RenderableComponent>(entt::exclude<UIComponent>);
            for(auto entity : renderableView) {
                auto& renderable = renderableView.get<RenderableComponent>(entity);
                if(!renderable.visible) continue;
                auto& transform = registry.get<TransformComponent>(entity);
                TextureManager::Instance()->drawFrame(renderable.textureID, transform.screenPosition, 
                        renderable.width, renderable.height, 
                        renderable.currentRow, renderable.currentFrame, 
                        renderable.alpha);
            }
            // 渲染UI实体
            for(auto entity : uiEntities) {
                auto& renderable = registry.get<RenderableComponent>(entity);
                if(!renderable.visible) continue;
                auto& transform = registry.get<TransformComponent>(entity);
                TextureManager::Instance()->drawFrame(renderable.textureID, transform.screenPosition, 
                        renderable.width, renderable.height, 
                        renderable.currentRow, renderable.currentFrame, 
                        renderable.alpha);
            }
            // 提交渲染
            SDL_RenderPresent(renderer);
        }
};

// 玩家控制处理系统
class PlayerControlSystem : public System {
    public:
        void update(entt::registry& registry, float deltaTime) override {
            auto playerView = registry.view<PlayerComponent, PositionComponent, AnimationComponent, RenderableComponent>();
            if(std::ranges::empty(playerView)) return;

            auto entity = playerView.front();
            auto& player = playerView.get<PlayerComponent>(entity);
            auto& pos = playerView.get<PositionComponent>(entity);
            auto& anim = playerView.get<AnimationComponent>(entity);
            auto& renderable = playerView.get<RenderableComponent>(entity);
            // 获取输入
            auto& input = registry.ctx().get<InputComponent>();
            if(input.keystates[SDL_SCANCODE_W]) {
                player.targetPosition = pos.position + Vector2D(0, -CELL_SIZE);
                player.currentDirection = PlayerComponent::UP;
                player.isMoving = true;
            }
            if(input.keystates[SDL_SCANCODE_S]) {
                player.targetPosition = pos.position + Vector2D(0, CELL_SIZE);
                player.currentDirection = PlayerComponent::DOWN;
                player.isMoving = true;
            }
            if(input.keystates[SDL_SCANCODE_A]) {
                player.targetPosition = pos.position + Vector2D(-CELL_SIZE, 0);
                player.currentDirection = PlayerComponent::LEFT;
                player.isMoving = true;
            }
            if(input.keystates[SDL_SCANCODE_D]) {
                player.targetPosition = pos.position + Vector2D(CELL_SIZE, 0);
                player.currentDirection = PlayerComponent::RIGHT;
                player.isMoving = true;
            }
            // 处理移动
            if(player.isMoving) {
                Vector2D dist = player.targetPosition - pos.position;
                if(dist.length() < 1.0f) {
                    pos.position = player.targetPosition;
                    player.isMoving = false;
                } else {
                    pos.position += dist.get_normalize() * 2.0f; // 移动速度
                }
            }
            // 设置动画帧
            renderable.currentRow = static_cast<uint8_t>(player.currentDirection) + 1;
            anim.animTimer++;
            if(anim.animTimer >= anim.delayFrame) {
                anim.animTimer -= anim.delayFrame;
                if(player.isMoving) {
                    anim.globalFrame = (anim.globalFrame + 1) % anim.numFrames;
                    renderable.currentFrame = anim.globalFrame % anim.numFrames;
                    renderable.currentFrame = (renderable.currentFrame >= 2) ? -1 : renderable.currentFrame;
                    renderable.currentFrame += 1;
                    renderable.currentFrame = (renderable.currentFrame == -1) ? 2 : renderable.currentFrame;
                } else {
                    renderable.currentFrame = (renderable.currentFrame == 0) ? 3 : 0; // 静止帧
                }
            }
            // 鼠标点击事件
            if(input.mouseButtonStates[0]) { // 左键
                SoundManager::Instance()->playSound("shoot", 0);
            }
        }

        void render(entt::registry& registry, SDL_Renderer* renderer) override {}
};

// 相机跟随系统
class CameraSystem : public System {
    public:
        void update(entt::registry& registry, float deltaTime) override {
            auto cameraView = registry.view<CameraComponent>();
            if(cameraView.empty()) return;

            auto entity = cameraView.front();
            auto& camera = cameraView.get<CameraComponent>(entity);

            // 寻找玩家实体
            auto playerView = registry.view<PlayerComponent, PositionComponent>();
            if(!std::ranges::empty(playerView)) {
                auto playerEntity = playerView.front();
                auto& playerPos = playerView.get<PositionComponent>(playerEntity);
                camera.follow(playerPos.position);
            }
        }

        void render(entt::registry& registry, SDL_Renderer* renderer) override {}
};

// 敌人AI系统
class EnemySystem : public System {
    public:
        void update(entt::registry& registry, float deltaTime) override {
            auto enemyView = registry.view<EnemyComponent, PositionComponent, AnimationComponent, RenderableComponent>();
            for(auto entity : enemyView) {
                //auto& enemy = enemyView.get<EnemyComponent>(entity);
                auto& pos = enemyView.get<PositionComponent>(entity);
                auto& anim = enemyView.get<AnimationComponent>(entity);
                auto& renderable = enemyView.get<RenderableComponent>(entity);

                // 简单的AI：左右移动
                if(pos.position.x > (SCR_W - renderable.width)) {
                    pos.velocity.x = -0.2f;
                } else if(pos.position.x < 0) {
                    pos.velocity.x = 0.2f;
                }

                // 更新动画
                anim.globalFrame = (anim.globalFrame + 1) % anim.numFrames;
                renderable.currentFrame = anim.globalFrame % anim.numFrames;
            }
        }

        void render(entt::registry& registry, SDL_Renderer* renderer) override {}
};

// 状态机系统
enum class GameStateType {
    MENU,
    PLAY,
    PAUSE,
    GAME_OVER
};

struct GameState {
    GameStateType type;
    bool initialized;

    GameState(GameStateType t) : type(t), initialized(false) {}
};

class GameStateMachine {
    private:
        std::vector<GameStateType> m_states;
        bool m_changeStateFlag;
        bool m_popStateFlag;
        GameStateType m_newState;

    public:
        GameStateMachine() : m_changeStateFlag(false), m_popStateFlag(false) {}

        void pushState(GameStateType state) {
            m_states.push_back(state);
        }

        void popState() {
            if(!m_states.empty()) {
                m_states.pop_back();
            }
        }

        void changeState(GameStateType state) {
            if(!m_states.empty() && m_states.back() == state) {
                return;
            }

            if(!m_states.empty()) {
                m_states.pop_back();
            }

            m_states.push_back(state);
        }

        GameStateType getCurrentState() const {
            if(m_states.empty()) return GameStateType::MENU;
            return m_states.back();
        }

        void update() {
            if(m_changeStateFlag) {
                changeState(m_newState);
                m_changeStateFlag = false;
            }
            else if(m_popStateFlag) {
                popState();
                m_popStateFlag = false;
            }
        }

        void render(entt::registry& registry, SDL_Renderer* renderer) {
            // 根据当前状态执行不同的渲染逻辑
            switch(getCurrentState()) {
                case GameStateType::PLAY:
                    // 游戏主渲染
                    break;
                case GameStateType::MENU:
                    // 菜单渲染
                    break;
                case GameStateType::PAUSE:
                    // 暂停菜单渲染
                    break;
                case GameStateType::GAME_OVER:
                    // 游戏结束菜单渲染
                    break;
            }
        }
};

// 主游戏类
class Game {
    private:
        static Game* s_pInstance;
        SDL_Window* m_pWindow;
        SDL_Renderer* m_pRenderer;
        bool m_bRunning;
        entt::registry m_registry;
        std::vector<std::unique_ptr<System>> m_systems;
        GameStateMachine m_stateMachine;
        Uint32 frameStart;
        Uint32 frameTime;

    public:
        static Game* Instance() {
            if(s_pInstance == 0) {
                s_pInstance = new Game();
            }
            return s_pInstance;
        }

        Game() : m_pWindow(nullptr), m_pRenderer(nullptr), m_bRunning(false) {
            // 添加系统
            m_systems.push_back(std::make_unique<InputSystem>());
            m_systems.push_back(std::make_unique<PhysicsSystem>());
            m_systems.push_back(std::make_unique<AnimationSystem>());
            m_systems.push_back(std::make_unique<PlayerControlSystem>());
            m_systems.push_back(std::make_unique<CameraSystem>());
            m_systems.push_back(std::make_unique<EnemySystem>());
            m_systems.push_back(std::make_unique<RenderSystem>());
        }

        ~Game() {
            clean();
        }

        bool init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen) {
            if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                std::cout << "SDL init fail!\nSDL ERROR: " << SDL_GetError() << std::endl;
                return false;
            }

            int flags = 0;
            if(fullscreen) flags = SDL_WINDOW_FULLSCREEN;

            m_pWindow = SDL_CreateWindow(title, x_pos, y_pos, width, height, flags);
            if(m_pWindow == nullptr) {
                std::cout << "window init fail\n";
                return false;
            }

            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
            if(m_pRenderer == nullptr) {
                std::cout << "renderer init fail\n";
                return false;
            }

            RenderContext::set(m_pRenderer);
            SDL_SetRenderDrawColor(m_pRenderer, 128, 128, 128, 255);

            // 初始化子系统
            IMG_Init(IMG_INIT_PNG);
            TTF_Init();
            Mix_Init(MIX_INIT_OGG);

            // 加载声音
            SoundManager::Instance()->load("assets/DST_ElecRock.ogg", "music1", SOUND_MUSIC);
            SoundManager::Instance()->load("assets/boom.wav", "explode", SOUND_SFX);
            SoundManager::Instance()->load("assets/phaser.wav", "shoot", SOUND_SFX);
            SoundManager::Instance()->playMusic("music1", -1);

            // 创建相机实体
            auto cameraEntity = m_registry.create();
            m_registry.emplace<CameraComponent>(cameraEntity);

            // 创建玩家实体
            createPlayer();

            // 创建一些敌人实体
            createEnemies();

            // 设置初始状态
            m_stateMachine.pushState(GameStateType::PLAY);

            m_bRunning = true;
            std::cout << "game initing success!\n";
            return true;
        }

        void createPlayer() {
            auto playerEntity = m_registry.create();
            m_registry.emplace<PositionComponent>(playerEntity, GRID_X + CELL_SIZE * 5, GRID_Y + CELL_SIZE * 5);
            m_registry.emplace<TransformComponent>(playerEntity);
            m_registry.emplace<RenderableComponent>(playerEntity, "player", 32, 32);
            m_registry.emplace<AnimationComponent>(playerEntity, 4, 8); // 4帧，8速
            m_registry.emplace<PlayerComponent>(playerEntity);
            m_registry.emplace<HealthComponent>(playerEntity, 100);

            // 加载玩家纹理
            TextureManager::Instance()->load("assets/player.png", "player");
        }

        void createEnemies() {
            for(int i = 0; i < 5; i++) {
                auto enemyEntity = m_registry.create();
                m_registry.emplace<PositionComponent>(enemyEntity, 100 + i * 80, 200);
                m_registry.emplace<TransformComponent>(enemyEntity);
                m_registry.emplace<RenderableComponent>(enemyEntity, "cat", 36, 36);
                m_registry.emplace<AnimationComponent>(enemyEntity, 2, 4);
                m_registry.emplace<EnemyComponent>(enemyEntity, 1);
                m_registry.emplace<HealthComponent>(enemyEntity, 10);

                // 加载猫纹理
                TextureManager::Instance()->load("assets/cat.png", "cat");
            }
        }

        void handleEvents() {
            // 输入系统会处理事件
        }

        void update() {
            float deltaTime = frameTime / 1000.0f; // 简化的delta time

            // 更新所有系统
            for(auto& system : m_systems) {
                system->update(m_registry, deltaTime);
            }

            m_stateMachine.update();
        }

        void render() {
            // 渲染所有系统
            for(auto& system : m_systems) {
                system->render(m_registry, m_pRenderer);
            }

            m_stateMachine.render(m_registry, m_pRenderer);
        }

        void syncFPS() {
            frameTime = SDL_GetTicks() - frameStart;
            if(frameTime < DELAY_TIME_MS) {
                SDL_Delay((int)(DELAY_TIME_MS - frameTime));
            }
            frameStart = SDL_GetTicks();
        }

        bool running() {
            frameStart = SDL_GetTicks();
            auto* runningVar = m_registry.ctx().find<bool>();
            return m_bRunning && (runningVar ? *runningVar : true);
        }

        void quit() {
            m_bRunning = false;
            m_registry.ctx().emplace<bool>(false);
        }

        void clean() {
            if(m_pRenderer) {
                SDL_DestroyRenderer(m_pRenderer);
                m_pRenderer = nullptr;
            }
            if(m_pWindow) {
                SDL_DestroyWindow(m_pWindow);
                m_pWindow = nullptr;
            }

            TextureManager::Instance()->clearTextureMap();
            IMG_Quit();
            TTF_Quit();
            Mix_Quit();
            SDL_Quit();

            std::cout << "cleaning game\n";
        }

        entt::registry& getRegistry() { return m_registry; }
        GameStateMachine* getStateMachine() { return &m_stateMachine; }
};

// 静态成员初始化
Game* Game::s_pInstance = nullptr;
SDL_Renderer* RenderContext::s_renderer = nullptr;
TextureManager* TextureManager::s_pInstance = nullptr;
SoundManager* SoundManager::s_pInstance = nullptr;

// 主函数
int main(int argc, char* argv[]) {
    std::cout << "game initing...\n";
    if(Game::Instance()->init("SDL2 EnTT Tiled World", SCR_X, SCR_Y, SCR_W, SCR_H, false)) {
        while(Game::Instance()->running()) {
            Game::Instance()->handleEvents();
            Game::Instance()->update();
            Game::Instance()->render();
            Game::Instance()->syncFPS();
        }
    } else {
        std::cout << "game init failure - " << SDL_GetError();
        return -1;
    }

    std::cout << "game closing...\n";
    Game::Instance()->clean();
    return 0;
}

#endif // __ENTT_GAME__
