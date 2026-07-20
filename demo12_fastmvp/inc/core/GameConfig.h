#pragma once
#include <stdint.h>

constexpr uint8_t   FPS             = 60; // 165
constexpr uint16_t  DELAY_TIME_MS   = 1000 / FPS;
constexpr uint16_t  SCR_W           = 800; //720; 1280; 640
constexpr uint16_t  SCR_H           = 640; //640; 1280; 480
constexpr uint16_t  SCR_X           = 1000;
constexpr uint16_t  SCR_Y           = 500;
constexpr uint16_t  MAP_WIDTH       = 256;
constexpr uint16_t  MAP_HEIGHT      = 256;

// UI size
constexpr uint16_t  UI_MAP_W        = 640; //720; 1280; 640
constexpr uint16_t  UI_MAP_H        = 480; //640; 1280; 480
constexpr uint16_t  UI_LOG_X        = 0;
constexpr uint16_t  UI_LOG_Y        = UI_MAP_H;
constexpr uint16_t  UI_LOG_W        = SCR_W;
constexpr uint16_t  UI_LOG_H        = SCR_H - UI_MAP_H;
constexpr uint16_t  UI_TIME_X       = UI_MAP_W;
constexpr uint16_t  UI_TIME_Y       = 0;
constexpr uint16_t  UI_TIME_W       = SCR_W - UI_MAP_W;
constexpr uint16_t  UI_TIME_H       = SCR_H;
constexpr uint8_t   CELL_SIZE       = 32;
constexpr uint8_t   CELL_X_NUM      = UI_MAP_W / CELL_SIZE;
constexpr uint8_t   CELL_Y_NUM      = UI_MAP_H / CELL_SIZE;


