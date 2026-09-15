#pragma once
#include <cmath>
#include <type_traits>
#include <iostream>
#include <cstdint>

enum class Direction : uint8_t
{ 
    NORMAL = 0, 
    UP = 1, 
    DOWN = 2, 
    LEFT = 3, 
    RIGHT = 4,
};

/// @brief 2D vector template for arithmetic types (int, float, etc.)
template<typename T = int>
struct vec2d {
    static_assert(std::is_arithmetic_v<T>, 
            "vec2d<T> requires an arithmetic type (e.g., int, float).");
    T x{}, y{};
    constexpr vec2d() = default;
    constexpr vec2d(T x, T y) : x{x}, y{y} {}
    inline static const vec2d<T> zero      = {0, 0};
    inline static const vec2d<T> unit_x    = {1, 0};
    inline static const vec2d<T> unit_y    = {0, 1};
    inline static const vec2d<T> up        = {0, -1};
    inline static const vec2d<T> down      = unit_y;
    inline static const vec2d<T> left      = {-1, 0};
    inline static const vec2d<T> right     = unit_x;
    inline static const vec2d<T> leftUp    = {-1, -1};
    inline static const vec2d<T> rightUp   = {1, -1};
    inline static const vec2d<T> leftDown  = {-1, 1};
    inline static const vec2d<T> rightDown = {1, 1};
    template<typename U>
        constexpr vec2d(const vec2d<U>& other)
        : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}
    template<typename U>
        constexpr vec2d& operator=(const vec2d<U>& other) {
            x = static_cast<T>(other.x);
            y = static_cast<T>(other.y);
            return *this;
        }

    // --- Arithmetic operators ---
    [[nodiscard]] friend constexpr vec2d operator-(const vec2d& v) {
        return {-v.x, -v.y};
    }

    [[nodiscard]] friend constexpr vec2d operator+(const vec2d& v1, const vec2d& v2) {
        return {v1.x + v2.x, v1.y + v2.y};
    }

    friend constexpr vec2d& operator+=(vec2d& v1, const vec2d& v2) {
        v1.x += v2.x;
        v1.y += v2.y;
        return v1;
    }

    [[nodiscard]] friend constexpr vec2d operator-(const vec2d& v1, const vec2d& v2) {
        return {v1.x - v2.x, v1.y - v2.y};
    }

    friend constexpr vec2d& operator-=(vec2d& v1, const vec2d& v2) {
        v1.x -= v2.x;
        v1.y -= v2.y;
        return v1;
    }

    [[nodiscard]] friend constexpr vec2d operator*(const vec2d& v, T scalar) {
        return {v.x * scalar, v.y * scalar};
    }

    [[nodiscard]] friend constexpr vec2d operator*(T scalar, const vec2d& v) {
        return v * scalar;
    }

    friend constexpr vec2d& operator*=(vec2d& v, T scalar) {
        v.x *= scalar;
        v.y *= scalar;
        return v;
    }

    [[nodiscard]] friend constexpr vec2d operator/(const vec2d& v, T scalar) {
        return {v.x / scalar, v.y / scalar};
    }

    friend constexpr vec2d& operator/=(vec2d& v, T scalar) {
        v.x /= scalar;
        v.y /= scalar;
        return v;
    }

    // --- Comparisons ---
    [[nodiscard]] friend constexpr bool operator==(const vec2d& v1, const vec2d& v2) {
        return v1.x == v2.x && v1.y == v2.y;
    }

    [[nodiscard]] friend constexpr bool operator!=(const vec2d& v1, const vec2d& v2) {
        return !(v1 == v2);
    }

    // --- Utility functions ---
    /// @brief Squared Euclidean distance (avoids sqrt, efficient)
    [[nodiscard]] friend constexpr T distance2(const vec2d& v) noexcept {
        return v.x * v.x + v.y * v.y;
    }
    [[nodiscard]] friend constexpr T distance2(const vec2d& v1, const vec2d& v2) noexcept {
        return distance2(v1 - v2);
    }

    [[nodiscard]] constexpr float distance() const noexcept {
        return std::sqrt(static_cast<float>(distance2(*this)));
    }
    [[nodiscard]] constexpr float distance(const vec2d& v) const noexcept {
        return std::sqrt(static_cast<float>(distance2(v - *this)));
    }
    friend float distance(const vec2d& v) noexcept {
        return std::sqrt(static_cast<float>(distance2(v)));
    }
    friend float distance(const vec2d& v1, const vec2d& v2) noexcept {
        return std::sqrt(static_cast<float>(distance2(v1, v2)));
    }

    // 2D vector normalized
    [[nodiscard]] constexpr vec2d<float> normalize() const noexcept{
        if(*this == vec2d{})  return {};
        return *this / distance(*this);
    }
    [[nodiscard]] friend vec2d<float> normalize(const vec2d& v){
        if(v == vec2d{})  return {};
        return vec2d<float>(v) / v.distance();
    }

    /// @brief Linear interpolation between a and b with parameter t in [0,1]
    [[nodiscard]] friend constexpr vec2d lerp(const vec2d &v1, const vec2d &v2, T t) {
        if (t <= T{0}) return v1;
        if (t >= T{1}) return v2;
        return v1 + t * (v2 - v1);
    }

    friend std::ostream& operator<<(std::ostream& os, const vec2d& v) {
        return os << '{' << v.x << ", " << v.y << '}';
    }

    [[nodiscard]] constexpr vec2d<int> sign() const noexcept{
        int dx = (x > 0) ? 1 : (x < 0 ? -1 : 0);
        int dy = (y > 0) ? 1 : (y < 0 ? -1 : 0);
        return {dx, dy};
    }

    static constexpr Direction DIR_MAP[] = {
        Direction::UP,    // ( -1, -1)
        Direction::UP,    // (  0, -1)
        Direction::UP,    // (  1, -1)
        Direction::LEFT,  // ( -1,  0)
        Direction::DOWN,  // (  0,  0) - 零向量
        Direction::RIGHT, // (  1,  0)
        Direction::DOWN,  // ( -1,  1)
        Direction::DOWN,  // (  0,  1)
        Direction::DOWN   // (  1,  1)
    };

    [[nodiscard]] constexpr Direction getVecDir() const noexcept {
        const auto s = sign();
        // 将二维方向映射到一维索引
        const int idx = (s.x + 1) + 3 * (s.y + 1);  // 映射到 0-8
        return DIR_MAP[idx];
    }
};

using vec  = vec2d<int>;
using vec2 = vec2d<float>;

[[nodiscard]] inline constexpr vec Dir2Vec(Direction dir) noexcept {
    switch (dir) {
        case Direction::UP:    return vec::up;
        case Direction::DOWN:  return vec::down;
        case Direction::LEFT:  return vec::left;
        case Direction::RIGHT: return vec::right;
        default:               return vec::zero;
    }
}

