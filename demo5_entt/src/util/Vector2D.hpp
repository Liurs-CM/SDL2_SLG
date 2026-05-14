#pragma once
#include <cmath>
#include <type_traits>
#include <ostream>

/// @brief 2D vector template for arithmetic types (int, float, etc.)
template<typename T>
struct vec2d {
    static_assert(std::is_arithmetic_v<T>, 
            "vec2d<T> requires an arithmetic type (e.g., int, float).");
    T x{}, y{};
    constexpr vec2d() = default;
    constexpr vec2d(T x, T y) : x{x}, y{y} {}
    inline static constexpr vec2d zero      = {0, 0};
    inline static constexpr vec2d unit_x    = {1, 0};
    inline static constexpr vec2d unit_y    = {0, 1};
    inline static constexpr vec2d up        = {0, 1};
    inline static constexpr vec2d down      = {0, -1};
    inline static constexpr vec2d left      = {-1, 0};
    inline static constexpr vec2d right     = {1, 0};
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
    friend constexpr vec2d operator-(const vec2d& v) {
        return {-v.x, -v.y};
    }

    friend constexpr [[nodiscard]] vec2d operator+(const vec2d& v1, const vec2d& v2) {
        return {v1.x + v2.x, v1.y + v2.y};
    }

    friend constexpr vec2d& operator+=(vec2d& v1, const vec2d& v2) {
        v1.x += v2.x;
        v1.y += v2.y;
        return v1;
    }

    friend constexpr vec2d operator-(const vec2d& v1, const vec2d& v2) {
        return {v1.x - v2.x, v1.y - v2.y};
    }

    friend constexpr vec2d& operator-=(vec2d& v1, const vec2d& v2) {
        v1.x -= v2.x;
        v1.y -= v2.y;
        return v1;
    }

    friend constexpr vec2d operator*(const vec2d& v, T scalar) {
        return {v.x * scalar, v.y * scalar};
    }

    friend constexpr vec2d operator*(T scalar, const vec2d& v) {
        return v * scalar;
    }

    friend constexpr vec2d& operator*=(vec2d& v, T scalar) {
        v.x *= scalar;
        v.y *= scalar;
        return v;
    }

    friend constexpr vec2d operator/(const vec2d& v, T scalar) {
        return {v.x / scalar, v.y / scalar};
    }

    friend constexpr vec2d& operator/=(vec2d& v, T scalar) {
        v.x /= scalar;
        v.y /= scalar;
        return v;
    }

    // --- Comparisons ---
    friend constexpr bool operator==(const vec2d& v1, const vec2d& v2) {
        return v1.x == v2.x && v1.y == v2.y;
    }

    friend constexpr bool operator!=(const vec2d& v1, const vec2d& v2) {
        return !(v1 == v2);
    }

    // --- Utility functions ---
    /// @brief Squared Euclidean distance (avoids sqrt, efficient)
    friend constexpr [[nodiscard]] T distance2(const vec2d& v) noexcept {
        return v.x * v.x + v.y * v.y;
    }
    friend constexpr [[nodiscard]] T distance2(const vec2d& v1, const vec2d& v2) noexcept {
        return distance2(v1 - v2);
    }

    friend float distance(const vec2d& v) noexcept {
        return std::sqrt(static_cast<float>(distance2(v)));
    }
    friend float distance(const vec2d& v1, const vec2d& v2) noexcept {
        return std::sqrt(static_cast<float>(distance2(v1, v2)));
    }

    // 2D vector normalized
    friend [[nodiscard]] vec2d<float> normalize(const vec2d& v){
        if(v == vec2d{})  return {};
        return vec2d<float>(v) / distance(v);
    }

    /// @brief Linear interpolation between a and b with parameter t in [0,1]
    friend constexpr vec2d lerp(const vec2d &v1, const vec2d &v2, T t) {
        if (t <= T{0}) return v1;
        if (t >= T{1}) return v2;
        return v1 + t * (v2 - v1);
    }

    friend std::ostream& operator<<(std::ostream& os, const vec2d& v) {
        return os << '{' << v.x << ", " << v.y << '}';
    }
};

