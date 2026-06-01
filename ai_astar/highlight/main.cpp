#include <SDL2/SDL.h>
#include <vector>
#include <queue>
#include <unordered_set>
#include <iostream>

const int TILE_SIZE = 64;
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 8;
const int SCREEN_WIDTH = MAP_WIDTH * TILE_SIZE;
const int SCREEN_HEIGHT = MAP_HEIGHT * TILE_SIZE;

// 地图：0=空地，1=障碍
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};

struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
    struct Hash { size_t operator()(const Point& p) const { return p.x * 1000 + p.y; } };
};

// BFS 计算可达移动范围（最大步数）
std::vector<Point> getMovementRange(const Point& start, int maxSteps) {
    std::vector<Point> result;
    if (map[start.y][start.x] == 1) return result;

    std::queue<std::pair<Point, int>> q;
    std::unordered_set<Point, Point::Hash> visited;
    q.push({start, 0});
    visited.insert(start);

    const int dx[4] = {0, 0, -1, 1};
    const int dy[4] = {-1, 1, 0, 0};

    while (!q.empty()) {
        auto [cur, steps] = q.front(); q.pop();
        if (steps > 0) result.push_back(cur); // 起点不加入（或可加入）
        if (steps == maxSteps) continue;

        for (int i = 0; i < 4; ++i) {
            Point next = {cur.x + dx[i], cur.y + dy[i]};
            if (next.x < 0 || next.x >= MAP_WIDTH || next.y < 0 || next.y >= MAP_HEIGHT)
                continue;
            if (map[next.y][next.x] == 1) continue;
            if (visited.count(next)) continue;
            visited.insert(next);
            q.push({next, steps + 1});
        }
    }
    return result;
}

// 攻击范围：曼哈顿距离 <= range
std::vector<Point> getAttackRange(const Point& center, int range) {
    std::vector<Point> result;
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (abs(x - center.x) + abs(y - center.y) <= range) {
                result.push_back({x, y});
            }
        }
    }
    return result;
}

// 绘制半透明高亮
void drawHighlight(SDL_Renderer* renderer, const std::vector<Point>& cells, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 100) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect rect;
    for (const auto& p : cells) {
        rect = {p.x * TILE_SIZE, p.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("SLG Demo", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Point selectedUnit = {2, 2}; // 初始选中单位位置
    bool showMoveRange = false;
    bool showAttackRange = false;

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_m: showMoveRange = !showMoveRange; break;
                    case SDLK_a: showAttackRange = !showAttackRange; break;
                    case SDLK_UP:    if (selectedUnit.y > 0) selectedUnit.y--; break;
                    case SDLK_DOWN:  if (selectedUnit.y < MAP_HEIGHT - 1) selectedUnit.y++; break;
                    case SDLK_LEFT:  if (selectedUnit.x > 0) selectedUnit.x--; break;
                    case SDLK_RIGHT: if (selectedUnit.x < MAP_WIDTH - 1) selectedUnit.x++; break;
                }
            }
        }

        // 清屏
        SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
        SDL_RenderClear(ren);

        // 绘制地图
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                SDL_SetRenderDrawColor(ren, map[y][x] ? 100 : 60, map[y][x] ? 100 : 60, map[y][x] ? 100 : 60, 255);
                SDL_Rect r = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderFillRect(ren, &r);
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                SDL_RenderDrawRect(ren, &r);
            }
        }

        // 绘制选中单位
        SDL_SetRenderDrawColor(ren, 0, 200, 0, 255);
        SDL_Rect unitRect = {selectedUnit.x * TILE_SIZE, selectedUnit.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(ren, &unitRect);

        // 高亮范围
        if (showMoveRange) {
            auto moveRange = getMovementRange(selectedUnit, 3);
            drawHighlight(ren, moveRange, 0, 100, 255, 120); // 蓝色
        }
        if (showAttackRange) {
            auto attackRange = getAttackRange(selectedUnit, 2);
            drawHighlight(ren, attackRange, 255, 0, 0, 100); // 红色
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
