#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "astar.h"

// --- 常量定义 ---
const int TILE_SIZE = 32;      // 每个格子的像素大小
const int MAP_WIDTH = 20;      // 地图宽度（格子数）
const int MAP_HEIGHT = 15;     // 地图高度（格子数）
const int SCREEN_WIDTH = MAP_WIDTH * TILE_SIZE;
const int SCREEN_HEIGHT = MAP_HEIGHT * TILE_SIZE;

// --- 颜色定义 ---
const SDL_Color COLOR_BG = {240, 240, 240, 255};       // 背景色
const SDL_Color COLOR_OBSTACLE = {50, 50, 50, 255};    // 障碍物颜色
const SDL_Color COLOR_PATH = {200, 50, 50, 255};       // 路径颜色
const SDL_Color COLOR_START = {50, 200, 50, 255};      // 起点颜色
const SDL_Color COLOR_GOAL = {50, 50, 200, 255};       // 终点颜色

// --- 渲染函数 ---
void renderMap(SDL_Renderer* renderer, const std::vector<std::vector<bool>>& obstacles, const std::vector<Point>& path, const Point& start, const Point& goal) {
    // 清空渲染器
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

    // 绘制起点和终点
    SDL_Rect startRect = {start.x * TILE_SIZE, start.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, COLOR_START.r, COLOR_START.g, COLOR_START.b, COLOR_START.a);
    SDL_RenderFillRect(renderer, &startRect);

    SDL_Rect goalRect = {goal.x * TILE_SIZE, goal.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, COLOR_GOAL.r, COLOR_GOAL.g, COLOR_GOAL.b, COLOR_GOAL.a);
    SDL_RenderFillRect(renderer, &goalRect);

    // 更新屏幕
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    // --- SDL2 初始化 ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("A* Pathfinding Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    // 定义障碍物 (true 表示障碍物)
    std::vector<std::vector<bool>> obstacles(MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));
    // 创建一些简单的障碍物
    for (int x = 5; x < 15; ++x) obstacles[7][x] = true;
    for (int y = 3; y < 10; ++y) obstacles[y][10] = true;

    Point start(2, 2);
    Point goal(17, 12);

    // 执行寻路
    auto path = astar.findPath(start, goal, obstacles);

    // --- 主循环 ---
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // 渲染
        if (path) {
            renderMap(renderer, obstacles, *path, start, goal);
        } else {
            // 如果没有路径，只渲染地图
            renderMap(renderer, obstacles, {}, start, goal);
        }

        SDL_Delay(100); // 控制帧率
    }

    // --- 清理资源 ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
