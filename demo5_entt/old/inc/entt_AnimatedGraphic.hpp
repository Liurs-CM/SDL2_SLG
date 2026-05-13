// entt_AnimatedGraphic.hpp
#pragma once
#include <cstdint>
#include <string>
// 假设 Texture 是您项目中已有的纹理类型
// #include "Texture.h" 

struct entt_AnimatedGraphic{
    // 动画配置
    uint8_t animSpeed;      // 对应原 m_animSpeed
    uint8_t numFrames;      // 对应原 m_numFrames
    // 动画状态
    uint8_t currentFrame;   // 当前帧索引
    // 关联资源
    // Texture* texture;    // 可以存储纹理指针
    // std::string texturePath; // 或者存储纹理路径
};
