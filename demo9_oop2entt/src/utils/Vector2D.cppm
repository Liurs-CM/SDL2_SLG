module;
#include <cmath>
#include <type_traits>
#include <iostream>

export module utils.Vector2D;

/// @brief 4方向枚举（SLG常用），NONE 表示零向量或静止状态
export enum class Direction {
    NONE = 0,
         UP,
         DOWN,
         LEFT,
         RIGHT
};

/// @brief 轻量级 2D 向量模板（纯数学结构，零外部依赖，ECS友好）
export template<typename T = int>
struct vec2d {
    static_assert(std::is_arithmetic_v<T>, "vec2d<T> requires an arithmetic type (int, float, double, etc.).");

    T x{T{}};
    T y{T{}};

    constexpr vec2d() noexcept = default;
    constexpr vec2d(T x, T y) noexcept : x{x}, y{y} {}

    // 显式类型转换构造，防止隐式截断
    template<typename U>
        constexpr explicit vec2d(const vec2d<U>& other) noexcept
        : x{static_cast<T>(other.x)}, y{static_cast<T>(other.y)} {}

    template<typename U>
        constexpr vec2d& operator=(const vec2d<U>& other) noexcept {
            x = static_cast<T>(other.x);
            y = static_cast<T>(other.y);
            return *this;
        }

    // --- 静态常量 ---
    inline static constexpr vec2d zero{0, 0};
    inline static constexpr vec2d unit_x{1, 0};
    inline static constexpr vec2d unit_y{0, 1};
    inline static constexpr vec2d up{0, -1};
    inline static constexpr vec2d down{0, 1};
    inline static constexpr vec2d left{-1, 0};
    inline static constexpr vec2d right{1, 0};
    inline static constexpr vec2d left_up{-1, -1};
    inline static constexpr vec2d right_up{1, -1};
    inline static constexpr vec2d left_down{-1, 1};
    inline static constexpr vec2d right_down{1, 1};

    // --- 算术运算符 ---
    [[nodiscard]] friend constexpr vec2d operator-(const vec2d& v) noexcept { return {-v.x, -v.y}; }
    [[nodiscard]] friend constexpr vec2d operator+(const vec2d& a, const vec2d& b) noexcept { return {a.x + b.x, a.y + b.y}; }
    [[nodiscard]] friend constexpr vec2d operator-(const vec2d& a, const vec2d& b) noexcept { return {a.x - b.x, a.y - b.y}; }
    [[nodiscard]] friend constexpr vec2d operator*(const vec2d& v, T s) noexcept { return {v.x * s, v.y * s}; }
    [[nodiscard]] friend constexpr vec2d operator*(T s, const vec2d& v) noexcept { return v * s; }
    [[nodiscard]] friend constexpr vec2d operator/(const vec2d& v, T s) noexcept { return {v.x / s, v.y / s}; }

    friend constexpr vec2d& operator+=(vec2d& a, const vec2d& b) noexcept { a.x += b.x; a.y += b.y; return a; }
    friend constexpr vec2d& operator-=(vec2d& a, const vec2d& b) noexcept { a.x -= b.x; a.y -= b.y; return a; }
    friend constexpr vec2d& operator*=(vec2d& v, T s) noexcept { v.x *= s; v.y *= s; return v; }
    friend constexpr vec2d& operator/=(vec2d& v, T s) noexcept { v.x /= s; v.y /= s; return v; }

    // --- 比较运算符 ---
    [[nodiscard]] friend constexpr bool operator==(const vec2d& a, const vec2d& b) noexcept { return a.x == b.x && a.y == b.y; }
    [[nodiscard]] friend constexpr bool operator!=(const vec2d& a, const vec2d& b) noexcept { return !(a == b); }

    // --- 数学工具函数 ---
    /// @brief 平方长度 / 平方距离（避免 sqrt，性能最优）
    [[nodiscard]] friend constexpr T length_sq(const vec2d& v) noexcept { return v.x * v.x + v.y * v.y; }
    [[nodiscard]] friend constexpr T length_sq(const vec2d& a, const vec2d& b) noexcept { return length_sq(a - b); }

    /// @brief 欧氏长度 / 距离（注：std::sqrt 在 C++20 非 constexpr，故移除 constexpr 修饰）
    [[nodiscard]] friend float length(const vec2d& v) noexcept { return std::sqrt(static_cast<float>(length_sq(v))); }
    [[nodiscard]] friend float distance(const vec2d& a, const vec2d& b) noexcept { return std::sqrt(static_cast<float>(length_sq(a, b))); }

    /// @brief 归一化（返回 float 精度向量，零向量安全）
    [[nodiscard]] friend vec2d<float> normalize(const vec2d& v) noexcept {
        float len = length(v);
        if (len == 0.0f) return vec2d<float>::zero;
        return vec2d<float>{v.x / len, v.y / len};
    }

    /// @brief 线性插值
    [[nodiscard]] friend constexpr vec2d lerp(const vec2d& a, const vec2d& b, float t) noexcept {
        if (t <= 0.0f) return a;
        if (t >= 1.0f) return b;
        return a + t * (b - a);
    }

    /// @brief 符号向量（返回 {-1, 0, 1}）
    [[nodiscard]] constexpr vec2d<int> sign() const noexcept {
        return {
            (x > 0) ? 1 : (x < 0 ? -1 : 0),
                (y > 0) ? 1 : (y < 0 ? -1 : 0)
        };
    }

    /// @brief 获取主方向（Y轴优先，适配 2D SLG 四向移动）
    [[nodiscard]] constexpr Direction get_direction() const noexcept {
        if (y < 0) return Direction::UP;
        if (y > 0) return Direction::DOWN;
        if (x < 0) return Direction::LEFT;
        if (x > 0) return Direction::RIGHT;
        return Direction::NONE;
    }

    // 调试输出
    friend std::ostream& operator<<(std::ostream& os, const vec2d& v) {
        return os << '{' << v.x << ", " << v.y << '}';
    }
};

// 常用类型别名
export using vec  = vec2d<int>;
export using vec2 = vec2d<float>;
