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
    int yOff = 90;
};

class UIMsgSystem 
{
    public:
        UIMsgSystem() noexcept { m_slots.reserve(MAX_VISIBLE_MSGS); };
         ~UIMsgSystem() = default;
         ///@brief 简易文本换行工具函数
         ///@warning 仅适用于等宽字体或纯ASCII。生产环境请使用 ICU 或 Freetype 度量
         //[[nodiscard]] static std::vector<std::string> wrapText(const std::string& text, size_t maxChars) {
         //    std::vector<std::string> lines;
         //    if (maxChars == 0 || text.empty()) return {text};
         //    lines.reserve((text.size() + maxChars - 1) / maxChars);
         //    for (size_t offset = 0; offset < text.size(); offset += maxChars) {
         //        const size_t len = std::min(maxChars, text.size() - offset);
         //        lines.emplace_back(text.substr(offset, len));
         //    }
         //    return lines;
         //}
         [[nodiscard]] static std::vector<std::string> wrapText(const std::string& text, size_t maxChars) {
             if (text.empty() || maxChars == 0) return {text};
             std::vector<std::string> lines;
             lines.reserve(text.size() / maxChars + 1);
             size_t start = 0;
             while (start < text.size()) {
                 // 计算当前行的理论终点
                 size_t end = start + maxChars;
                 // 情况1: 剩余文本不足 maxChars，直接取完
                 if (end >= text.size()) {
                     // 去除尾部空白后入列
                     size_t trimEnd = text.size();
                     while (trimEnd > start && text[trimEnd - 1] == ' ') --trimEnd;
                     if (trimEnd > start) {
                         lines.emplace_back(text.begin() + static_cast<std::ptrdiff_t>(start),
                                 text.begin() + static_cast<std::ptrdiff_t>(trimEnd));
                     }
                     break;
                 }
                 // 情况2: end 处恰好是空格 → 完美断点
                 // 情况3: end 处不是空格 → 向前找最后一个空格作为断点
                 size_t cutPos = end;
                 if (text[end] != ' ') {
                     // 从 end-1 向前搜索空格，但不越过 start
                     while (cutPos > start && text[cutPos - 1] != ' ') --cutPos;
                 }
                 // 情况4: 整个 [start, end) 区间无空格 → 超长词强制截断
                 if (cutPos == start) {
                     cutPos = end; // 强制在 maxChars 处截断
                 } else {
                     // cutPos 指向空格位置或 end；实际内容终点需去除尾部空白
                     // 若 cutPos == end 且 text[end]==' '，内容终点就是 end
                     // 若 cutPos < end，说明在空格处断开，cutPos 本身就是空格
                 }
                 // 确定实际内容终点（去除尾部空白）
                 size_t contentEnd = cutPos;
                 while (contentEnd > start && text[contentEnd - 1] == ' ') --contentEnd;
                 if (contentEnd > start) {
                     lines.emplace_back(text.begin() + static_cast<std::ptrdiff_t>(start),
                             text.begin() + static_cast<std::ptrdiff_t>(contentEnd));
                 }
                 // 下一行起点：跳过断点处的所有空格
                 start = cutPos;
                 while (start < text.size() && text[start] == ' ') ++start;
             }
             if (lines.empty()) lines.emplace_back();
             return lines;
         }
         void setInfoMsg(const std::string& text){
            constexpr int MAX_CHARS_PER_LINE = 80; 
            // 限制队列长度，防止内存爆炸
            if (!m_slots.empty() && m_slots.back().text == text) return;
            // 2. 预处理换行（避免在持有锁或关键路径中做复杂字符串操作）
            const auto cachedLines = wrapText(text, MAX_CHARS_PER_LINE);
            if (cachedLines.empty()) return;
            const size_t requiredSpace = cachedLines.size();
            while (m_slots.size() + requiredSpace > MAX_VISIBLE_MSGS && !m_slots.empty()) {
                m_slots.erase(m_slots.begin()); // ⚠️ 性能警告：见下方说明
            }
            // 4. 如果单条消息本身就超过最大容量，只保留尾部可见部分
            //    （防止一条超长消息永远无法被后续消息挤出）
            size_t startIdx = 0;
            if (requiredSpace > MAX_VISIBLE_MSGS) {
                startIdx = requiredSpace - MAX_VISIBLE_MSGS;
            }
            Color tint = toggle2even ? Color::WHITE : Color::BLACK;
            toggle2even = !toggle2even;
            //m_slots.reserve(m_slots.size() + cachedLines.size());
            //for (size_t i = startIdx; i < cachedLines.size(); ++i) {
            //    m_slots.push_back({cachedLines[i], tint, 1.f, CELL_SIZE});
            //}
            for (size_t i = cachedLines.size(); i-- > startIdx; ) { // 安全的倒序遍历
                m_slots.push_back({cachedLines[i], tint, 1.f, CELL_SIZE});
            }
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
        /// @brief 渲染日志消息列表（自上向下版）
        /// @note 最新消息在顶部，支持多行换行、透明度衰减与视口裁剪
        void render() const {
            //if(UIMsgSystem::Instance().hasMsg()) { }
            const size_t count = std::min(m_slots.size(), static_cast<size_t>(MAX_VISIBLE_MSGS));
            for (size_t i = 0; i < count; ++i) {
                const auto& s = m_slots[i];
                // 计算Y位置（从上向底排列）
                int y = UI_LOG_Y  + static_cast<int>( (count - i) * LINE_H) + static_cast<int>(s.yOff);
                uint32_t alpha = (256 / MAX_VISIBLE_MSGS * ( MAX_VISIBLE_MSGS - count + i + 1 )) << 24;
                uint32_t rgb = s.tint.rgb();
                //draw.drawPrintf(UI_LOG_X + 2, y, Color( alpha | Color::BLACK.rgb()), s.text.c_str());
                draw.drawText(UI_LOG_X, y, s.text.c_str(), Color( alpha | rgb));
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
