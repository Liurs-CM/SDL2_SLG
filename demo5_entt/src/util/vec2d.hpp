#pragma once
#include <cmath>
#include <type_traits>
#include <ostream>

// 二维vector模板，支持 int、float 等算术类型
template<typename T>
struct vec2d {
    static_assert(std::is_arithmetic_v<T>, 
            "vec2d<T> requires an arithmetic type (e.g., int, float).");
    T x{}, y{};
    constexpr vec2d() = default;
    constexpr vec2d(T x, T y) : x{x}, y{y} {}
    template<typename U>
        constexpr vec2d(const vec2d<U>& other)
        : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}
    template<typename U>
        constexpr vec2d& operator=(const vec2d<U>& other) {
            x = static_cast<T>(other.x);
            y = static_cast<T>(other.y);
            return *this;
        }
};

template<typename T>
constexpr vec2d<T> operator-(const vec2d<T>& v) {
    return {-v.x, -v.y};
}

template<typename T>
constexpr vec2d<T> operator+(const vec2d<T>& v1, const vec2d<T>& v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

template<typename T>
constexpr vec2d<T>& operator+=(vec2d<T>& v1, const vec2d<T>& v2) {
    return v1 = v1 + v2;
}

template<typename T>
constexpr vec2d<T> operator-(const vec2d<T>& v1, const vec2d<T>& v2) {
    return v1 + (- v2);
}

template<typename T>
constexpr vec2d<T>& operator-=(vec2d<T>& v1, const vec2d<T>& v2) {
    return v1 = v1 - v2;
}

template<typename T>
constexpr vec2d<T> operator*(const vec2d<T>& v, T scaler) {
    return {v.x * scaler, v.y * scaler};
}

template<typename T>
constexpr vec2d<T> operator*(T scaler, const vec2d<T>& v) {
    return v * scaler; // 利用交换律复用
}

template<typename T>
constexpr vec2d<T>& operator*=(vec2d<T>& v, T scaler) {
    return v = scaler * v;
}

template<typename T>
constexpr vec2d<T> operator/(const vec2d<T>& v, T scalar) {
    return {v.x / scalar, v.y / scalar};
}

template<typename T>
constexpr vec2d<T>& operator/=(vec2d<T>& v, T scalar) {
    return v = v / scalar;
}

template<typename T>
constexpr bool operator==(const vec2d<T>& v1, const vec2d<T>& v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

template<typename T>
constexpr bool operator!=(const vec2d<T>& v1, const vec2d<T>& v2) {
    return !(v1 == v2);
}

// ——————— 工具函数 ———————
// 返回平方欧氏距离（避免开方，高效）
template<typename T>
constexpr T distance2(const vec2d<T>& v) noexcept {
    return v.x * v.x + v.y * v.y;
}

template<typename T>
constexpr T distance2(const vec2d<T>& v1, const vec2d<T>& v2) noexcept {
    return distance2(v1 - v2);
}

// 返回欧氏距离（统一返回 float，适合游戏逻辑）
template<typename T>
float distance(const vec2d<T>& v) noexcept {
    return std::sqrt(static_cast<float>(distance2(v)));
}
template<typename T>
float distance(const vec2d<T>& v1, const vec2d<T>& v2) noexcept {
    return std::sqrt(static_cast<float>(distance2(v1, v2)));
}

// 2D向量归一化
template<typename T>
vec2d<float> normalize(const vec2d<T>& v){
    if(v == vec2d<T>{}) { return {}; }
    return vec2d<float>(v) / distance(v);
}

// 线性插值
template<typename T>
constexpr vec2d<T> lerp(const vec2d<T> &v1, const vec2d<T> &v2, T scaler) {
    return v1 + scaler * (v2 - v1);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const vec2d<T>& v) {
    return os << '{' << v.x << ", " << v.y << '}';
}

