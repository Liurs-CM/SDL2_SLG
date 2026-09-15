#pragma once
#include "util/Draw.hpp"
#include "core/GameConfig.h"

class UIHudSystem
{
    public:
        UIHudSystem() noexcept { };
        ~UIHudSystem() = default;
        UIHudSystem(const UIHudSystem&) = delete;
        UIHudSystem& operator=(const UIHudSystem&) = delete;
        static UIHudSystem& Instance() noexcept {
            static UIHudSystem instance;
            return instance;
        }
        std::string setInfo(const char* fmt, ...) {
            char buf[1024];
            va_list args;
            va_start(args, fmt);
            vsnprintf(buf, sizeof(buf), fmt, args);
            va_end(args);
            buf[sizeof(buf) - 1] = '\0';
            return (std::string(buf));
        }
        void render(entt::registry* m_registry) {
            auto view = m_registry->view<Sources>();
            auto gold = m_registry->get<Sources>(*view.begin()).gold;
            std::string text = setInfo("Coins: %d", gold);
            draw.drawText(UI_TIME_X + CELL_SIZE + 2, UI_TIME_Y + 2, text.c_str(), Color::BLACK);
        }
    private:
        static constexpr uint8_t NUM_INVENTORY_SLOTS = 8;
        inline static SDL_Renderer* m_pRenderer = RenderContext::get();
        Draw& draw = Draw::instance();
        //entt::registry* m_registry;
};

