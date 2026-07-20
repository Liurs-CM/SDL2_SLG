export module utils.Camera;

import utils.Vector2D;
import <cstdint>;

/// @brief 纯数据相机结构体（ECS 友好）
/// 不持有实体、不处理跟随逻辑，仅提供视口参数与坐标转换纯函数。
/// 跟随/平滑逻辑由外部 System 或 State 写回 position 字段。
export struct Camera
{
    vec2 position{0.0f, 0.0f};       // 相机世界坐标（视口左上角）
    vec2 target{0.0f, 0.0f};         // 跟随目标点（供外部系统读取）
    float follow_speed{5.0f};        // 平滑跟随系数（像素/秒 或 插值权重）
    uint16_t width{800};             // 视口宽度
    uint16_t height{600};            // 视口高度

    /// @brief 世界坐标 -> 屏幕坐标
    [[nodiscard]] constexpr vec2 world_to_screen(vec2 world_pos) const noexcept
    {
        return {world_pos.x - position.x, world_pos.y - position.y};
    }

    /// @brief 屏幕坐标 -> 世界坐标
    [[nodiscard]] constexpr vec2 screen_to_world(vec2 screen_pos) const noexcept
    {
        return {screen_pos.x + position.x, screen_pos.y + position.y};
    }

    /// @brief 视口裁剪检测（可选，用于 RenderSystem 剔除）
    [[nodiscard]] constexpr bool is_in_viewport(vec2 world_pos, float padding = 0.0f) const noexcept
    {
        const float left   = position.x - padding;
        const float right  = position.x + width + padding;
        const float top    = position.y - padding;
        const float bottom = position.y + height + padding;
        return world_pos.x >= left && world_pos.x <= right &&
            world_pos.y >= top  && world_pos.y <= bottom;
    }
};

/// @brief 独立纯函数：处理相机平滑跟随（替代原 update/follow）
/// 设计原则：逻辑与数据分离，便于 System 调度或 State 直接调用
export inline void tick_camera(Camera& cam, float dt) noexcept
{
    // 线性插值平滑跟随：pos += (target - pos) * speed * dt
    const float factor = cam.follow_speed * dt;
    cam.position.x += (cam.target.x - cam.position.x) * factor;
    cam.position.y += (cam.target.y - cam.position.y) * factor;
}
