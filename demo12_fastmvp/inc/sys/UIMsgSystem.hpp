#pragma once
#include "core/GameConfig.h"
#include "util/Draw.hpp"
#include <SDL.h>
#include <string>

// 消息槽位：纯状态，零资源所有权
struct MsgSlot {
    std::string text;    // TM 中的纹理 ID
    Color tint;     // 运行时着色（复用同一白色纹理）
    float alpha = 1.f;
    int yOff = 100;
};

class UIMsgSystem 
{
    public:
        UIMsgSystem() noexcept { m_slots.reserve(MAX_VISIBLE_MSGS); };
         ~UIMsgSystem() = default;
        void setInfoMsg(const std::string& text){
            // 限制队列长度，防止内存爆炸
            if (!m_slots.empty()) {
                auto& back = m_slots.back();
                if (back.text == text) return;
            }
            if (m_slots.size() >= MAX_VISIBLE_MSGS) m_slots.erase(m_slots.begin());
            Color tint = toggle2even ? Color::WHITE : Color::BLACK;
            toggle2even = !toggle2even;
            m_slots.push_back({text, tint, 1.f, CELL_SIZE});
            msg_update = true;
        }
        // 新增带格式化的 setInfoMsg 重载
        void setInfoMsg(const char* fmt, ...) {
            char buf[1024];
            va_list args;
            va_start(args, fmt);
            vsnprintf(buf, sizeof(buf), fmt, args);
            va_end(args);
            buf[sizeof(buf) - 1] = '\0';
            setInfoMsg(std::string(buf));  // 复用原有去重+入队逻辑
        }
        void doneMsg(){ msg_update = false; }
        bool hasMsg() { return msg_update; }
        void render() const {
            //if(UIMsgSystem::Instance().hasMsg()) { }
            const size_t count = std::min(m_slots.size(), static_cast<size_t>(MAX_VISIBLE_MSGS));
            for (size_t i = 0; i < count; ++i) {
                const auto& s = m_slots[i];
                // 计算Y位置（从上向底排列）
                int y = UI_LOG_Y  + static_cast<int>( (count - i) * LINE_H) + static_cast<int>(s.yOff);
                uint32_t alpha = (256 / MAX_VISIBLE_MSGS * ( MAX_VISIBLE_MSGS - count + i )) << 24;
                uint32_t rgb = s.tint.rgb();
                //draw.drawPrintf(UI_LOG_X + 2, y, Color( alpha | Color::BLACK.rgb()), s.text.c_str());
                draw.drawText(UI_LOG_X + 2, y, s.text.c_str(), Color( alpha | rgb));
                //std::cout << "msg[" << i << "/" << count << "] Draw text: " << s.text << " at " << UI_LOG_X << ", " << y << "\n";
            }
        }
        static UIMsgSystem& Instance() noexcept {
            static UIMsgSystem instance;
            return instance;
        }
        UIMsgSystem(const UIMsgSystem&) = delete;
        UIMsgSystem& operator=(const UIMsgSystem&) = delete;

    private:
        inline static SDL_Renderer* m_pRenderer = RenderContext::get();
        static constexpr size_t MAX_VISIBLE_MSGS = 10;
        static constexpr size_t LINE_H = 12;
        std::vector<MsgSlot> m_slots;
        //fontlib::font_atlas& m_atlas = font;
        bool msg_update = false;
        Draw& draw = Draw::instance();
        bool toggle2even = false;
};
