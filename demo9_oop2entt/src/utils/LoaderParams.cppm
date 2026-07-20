export module utils.LoaderParams;

import utils.Vector2D;
import <string>;
import <cstdint>;

/// @brief Prefab 实例化参数契约（纯数据聚合结构）
/// @note 替代原 OOP LoaderParams 类，移除 Getter/Setter 与私有封装，全面转向 ECS 数据驱动
export struct LoaderParams {
    vec2 position{0.0f, 0.0f};      // 替代原 int x/y，支持浮点插值与物理计算
    uint16_t width{0};              // 精灵宽度
    uint16_t height{0};             // 精灵高度
    std::string texture_id{};       // 纹理注册表键值（对接 TextureManager）
    uint8_t num_frames{1};          // 动画总帧数
    uint8_t anim_speed{4};          // 帧切换速率（默认 4，单位由 AnimationSystem 解释）
    int callback_id{0};             // 兼容旧逻辑，ECS 中建议后续迁移至事件/脚本组件
};
