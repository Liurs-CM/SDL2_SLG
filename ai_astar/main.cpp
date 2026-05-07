#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "astar.h"

// --- 常量定义 ---
const int TILE_SIZE = 32;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;
const int SCREEN_WIDTH = MAP_WIDTH * TILE_SIZE;
const int SCREEN_HEIGHT = MAP_HEIGHT * TILE_SIZE;

// --- 颜色定义 ---
const SDL_Color COLOR_BG = {240, 240, 240, 255};
const SDL_Color COLOR_OBSTACLE = {50, 50, 50, 255};
const SDL_Color COLOR_PATH = {200, 50, 50, 255};
const SDL_Color COLOR_START = {50, 200, 50, 255};
const SDL_Color COLOR_GOAL = {50, 50, 200, 255};

// --- 渲染函数 ---
void renderMap(SDL_Renderer* renderer, const std::vector<std::vector<bool>>& obstacles, 
        const std::vector<Point>& path, const Point& start, const Point& goal) {
    SDL_SetRenderDrawColor(renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, COLOR_BG.a);
    SDL_RenderClear(renderer);

    // 绘制障碍物
    SDL_SetRenderDrawColor(renderer, COLOR_OBSTACLE.r, COLOR_OBSTACLE.g, COLOR_OBSTACLE.b, COLOR_OBSTACLE.a);
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (obstacles[y][x]) {
                SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // 绘制路径
    SDL_SetRenderDrawColor(renderer, COLOR_PATH.r, COLOR_PATH.g, COLOR_PATH.b, COLOR_PATH.a);
    for (const auto& p : path) {
        SDL_Rect rect = {p.x * TILE_SIZE, p.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // 绘制起点
    SDL_Rect startRect = {start.x * TILE_SIZE, start.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, COLOR_START.r, COLOR_START.g, COLOR_START.b, COLOR_START.a);
    SDL_RenderFillRect(renderer, &startRect);

    // 绘制终点
    SDL_Rect goalRect = {goal.x * TILE_SIZE, goal.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, COLOR_GOAL.r, COLOR_GOAL.g, COLOR_GOAL.b, COLOR_GOAL.a);
    SDL_RenderFillRect(renderer, &goalRect);

    SDL_RenderPresent(renderer);
}

// --- 辅助函数：将屏幕坐标转换为网格坐标 ---
Point screenToGrid(int screenX, int screenY) {
    return Point(screenX / TILE_SIZE, screenY / TILE_SIZE);
}

int main(int argc, char* argv[]) {
    // --- SDL2 初始化 ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("A* Pathfinding Demo", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // --- 游戏逻辑 ---
    AStar astar(MAP_WIDTH, MAP_HEIGHT);

    // 定义障碍物
    std::vector<std::vector<bool>> obstacles(MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));
    for (int x = 5; x < 15; ++x) obstacles[7][x] = true;
    for (int y = 3; y < 10; ++y) obstacles[y][10] = true;

    Point start(2, 2);
    Point goal(17, 12);

    // 拖拽状态
    bool isDragging = false;      // 是否正在拖拽
    bool isDraggingStart = false; // true: 拖拽起点, false: 拖拽终点

    // 初始寻路
    auto path = astar.findPath(start, goal, obstacles);

    // --- 主循环 ---
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // --- 鼠标事件处理 ---
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                Point gridPos = screenToGrid(e.button.x, e.button.y);

                // 检查是否点击了起点或终点
                if (gridPos == start) {
                    isDragging = true;
                    isDraggingStart = true;
                } else if (gridPos == goal) {
                    isDragging = true;
                    isDraggingStart = false;
                }
            }

            if (e.type == SDL_MOUSEMOTION && isDragging) {
                Point gridPos = screenToGrid(e.motion.x, e.motion.y);

                // 确保新位置在地图范围内且不是障碍物
                if (gridPos.x >= 0 && gridPos.x < MAP_WIDTH && 
                        gridPos.y >= 0 && gridPos.y < MAP_HEIGHT &&
                        !obstacles[gridPos.y][gridPos.x]) {

                    if (isDraggingStart) {
                        start = gridPos;
                    } else {
                        goal = gridPos;
                    }

                    // 实时重新寻路
                    path = astar.findPath(start, goal, obstacles);
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                isDragging = false;
            }
        }

        // 渲染
        if (path) {
            renderMap(renderer, obstacles, *path, start, goal);
        } else {
            renderMap(renderer, obstacles, {}, start, goal);
        }

        SDL_Delay(16); // 约 60 FPS
    }

    // --- 清理资源 ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
