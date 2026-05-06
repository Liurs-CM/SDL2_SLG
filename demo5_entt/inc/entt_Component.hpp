// Components.hpp
#pragma once
#include "Vector2D.h" // 假设你保留了这个数学库
#include <string>
#include <entt/entt.hpp>

// 1. 变换组件：存储位置、速度、加速度
struct TransformComponent {
    Vector2D position{0, 0};
    Vector2D velocity{0, 0};
    Vector2D acceleration{0, 0};
};

// 2. 渲染组件：存储与绘图相关的数据
struct RenderComponent {
    std::string textureID;
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t currentRow = 1;
    uint8_t currentFrame = 0;
    uint8_t numFrames = 1;
    uint8_t animSpeed = 1;
    int alpha = 255;
    // 屏幕位置通常由系统计算，不一定要存这里，但为了对应原逻辑暂时保留
    Vector2D screenPosition{0, 0}; 
};

// 3. 动画组件：如果需要更复杂的动画状态机，可以单独拆分
struct AnimationComponent {
    uint8_t animSpeed;      // 对应原 m_animSpeed
    uint8_t numFrames;      // 对应原 m_numFrames
    uint8_t currentFrame;   // 当前帧索引
    // 关联资源
    // Texture* texture;    // 可以存储纹理指针
    // std::string texturePath; // 或者存储纹理路径
};

