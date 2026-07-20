#pragma once
#include "comp/CompGroup.hpp"
#include <entt/entt.hpp>
#include <tinyxml2.h>
#include <string_view>
#include <functional>
#include <unordered_map>

/**
 * @brief 地图解析系统 (ECS 纯函数式)
 * 
 * 【设计原则】
 * 1. 无状态设计: 不持有任何关卡运行时数据，所有数据直接写入 Registry
 * 2. 纯函数倾向: parseLevel 为入口，内部辅助方法均为 static，保证线程安全
 * 3. 零拷贝字符串: Handler Key 使用 constexpr string_view 指向字面量
 */
class MapParserSystem final
{ 
    public:
        explicit MapParserSystem() = default;
        ~MapParserSystem() = default;
        // 禁止拷贝/移动
        MapParserSystem(const MapParserSystem&) = delete;
        MapParserSystem& operator=(const MapParserSystem&) = delete;
        MapParserSystem(MapParserSystem&&) = delete;
        MapParserSystem& operator=(MapParserSystem&&) = delete;

        /**
         * @brief 解析关卡文件并填充到 ECS Registry
         * @param registry 目标 ECS 注册表
         * @param levelFile XML 文件路径
         * @return 成功返回 Level Entity，失败返回 entt::null
         */
        static void parseLevel(entt::registry& registry, std::string_view levelFile);
        static void precomputeTileUVs(entt::registry& reg);

    private:
        using ElementHandler = std::function<void(tinyxml2::XMLElement*, entt::registry&)>;
        // ✅ constexpr string_view 安全指向字面量
        static constexpr std::string_view TAG_PROPERTIES  = "properties";
        static constexpr std::string_view TAG_PROPERTY    = "property";
        static constexpr std::string_view TAG_TILESET     = "tileset";
        static constexpr std::string_view TAG_OBJECTGROUP = "objectgroup";
        static constexpr std::string_view TAG_LAYER       = "layer";
        static constexpr std::string_view TAG_OBJECT      = "object";
        static constexpr std::string_view TAG_DATA        = "data";
        // ✅ 返回 const ref 避免每次调用都构造 map
        static const std::unordered_map<std::string_view, ElementHandler>& getHandlers();
        // --- 元素解析器 (Static Pure Functions) ---
        static void handleProperties(tinyxml2::XMLElement* elem, entt::registry& reg);
        static void handleTileset(tinyxml2::XMLElement* elem, entt::registry& reg);
        static void handleObjectGroup(tinyxml2::XMLElement* elem, entt::registry& reg);
        static void handleLayer(tinyxml2::XMLElement* elem, entt::registry& reg);
        // --- 辅助工具 ---
        static void parseTextures(tinyxml2::XMLElement* root, entt::registry& reg);
        static void parseTilesets(tinyxml2::XMLElement* root, entt::registry& reg);
        static void parseObjectLayer(tinyxml2::XMLElement* elem, entt::registry& reg);
        static void parseTileLayer(tinyxml2::XMLElement* elem, entt::registry& reg);
};
