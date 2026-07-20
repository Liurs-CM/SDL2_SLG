#pragma once
#include <SDL2/SDL_render.h>
#include <cstdint>

class Color 
{
    public:
        explicit constexpr Color(uint32_t argb = 0xFF000000) noexcept : value(argb) {}
        // 访问各通道
        constexpr uint8_t alpha() const noexcept { return (value >> 24) & 0xFF; }
        constexpr uint8_t red()   const noexcept { return (value >> 16) & 0xFF; }
        constexpr uint8_t green() const noexcept { return (value >> 8)  & 0xFF; }
        constexpr uint8_t blue()  const noexcept { return value & 0xFF; }
        constexpr uint32_t rgb() const noexcept { return value & 0x00FFFFFF; }
        constexpr uint32_t argb() const noexcept { return value; }
        constexpr operator uint32_t() const noexcept { return value; }
        struct RGBA {uint8_t r, g, b, a; };
        constexpr SDL_Color rgba() const noexcept { return {red(), green(), blue(), alpha()}; }
        // 常用颜色作为静态常量
        static const Color BLACK;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color YELLOW;
        static const Color CYAN;
        static const Color MAGENTA;
        static const Color ORANGE;
        static const Color PINK;
        static const Color PURPLE;
        static const Color GRAY;
        static const Color DARK_GRAY;
        static const Color LIGHT_GRAY;
        static const Color DARK_RED;
        static const Color DARK_GREEN;
        static const Color DARK_BLUE;
        static const Color SKY_BLUE;
        static const Color BROWN;
        static const Color GOLD;
        static const Color TRANSPARENT;
        // 比较运算符
        constexpr bool operator==(const Color& other) const noexcept = default;
        constexpr bool operator!=(const Color& other) const noexcept = default;
    private:
        uint32_t value;
};

// 静态成员定义
inline constexpr Color Color::BLACK      {0xFF000000};
inline constexpr Color Color::WHITE      {0xFFFFFFFF};
inline constexpr Color Color::RED        {0xFFFF0000};
inline constexpr Color Color::GREEN      {0xFF00FF00};
inline constexpr Color Color::BLUE       {0xFF0000FF};
inline constexpr Color Color::YELLOW     {0xFFFFFF00};
inline constexpr Color Color::CYAN       {0xFF00FFFF};
inline constexpr Color Color::MAGENTA    {0xFFFF00FF};
inline constexpr Color Color::ORANGE     {0xFFFF8800};
inline constexpr Color Color::PINK       {0xFFFF88CC};
inline constexpr Color Color::PURPLE     {0xFF8800FF};
inline constexpr Color Color::GRAY       {0xFF888888};
inline constexpr Color Color::DARK_GRAY  {0xFF444444};
inline constexpr Color Color::LIGHT_GRAY {0xFFCCCCCC};
inline constexpr Color Color::DARK_RED   {0xFF880000};
inline constexpr Color Color::DARK_GREEN {0xFF008800};
inline constexpr Color Color::DARK_BLUE  {0xFF000088};
inline constexpr Color Color::SKY_BLUE   {0xFF87CEEB};
inline constexpr Color Color::BROWN      {0xFF8B4513};
inline constexpr Color Color::GOLD       {0xFFFFD700};
inline constexpr Color Color::TRANSPARENT{0x00000000};

