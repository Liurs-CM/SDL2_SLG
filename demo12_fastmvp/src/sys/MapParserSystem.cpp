#include "sys/MapParserSystem.hpp"
#include "core/GameConfig.h"
#include "core/TextureManager.hpp"
#include "comp/CompGroup.hpp"
#include "base64.h"
#include "zlib.h"
#include "tinyxml2.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <string>
#include <unordered_map>

namespace {

    // ============================================================================
    // 匿名命名空间：内部工具函数 (Internal Linkage)
    // ============================================================================

    /// @brief 安全获取整型属性，零拷贝 key 查找
    [[nodiscard]] inline int getIntAttr(
            const tinyxml2::XMLElement* elem, 
            std::string_view name, 
            int defaultVal = 0) noexcept 
    {
        if (!elem) return defaultVal;
        int val = defaultVal;
        elem->QueryIntAttribute(name.data(), &val);
        return val;
    }

    /// @brief 安全获取字符串属性，返回指向 XML 内部缓冲区的 view (零拷贝)
    [[nodiscard]] inline std::string_view getStrAttr(
            const tinyxml2::XMLElement* elem, 
            std::string_view name, 
            std::string_view defaultVal = "") noexcept 
    {
        if (!elem) return defaultVal;
        const char* val = elem->Attribute(name.data());
        return val ? std::string_view(val) : defaultVal;
    }

    /// @brief 标签名匹配 (零拷贝)
    [[nodiscard]] inline bool isTag(const tinyxml2::XMLElement* elem, std::string_view tag) noexcept {
        return elem && std::string_view(elem->Value()) == tag;
    }

    /// @brief Base64 + Zlib 解码瓦片数据
    [[nodiscard]] std::vector<uint32_t> decodeTileData(
            const tinyxml2::XMLElement* dataNode, 
            uint32_t width, 
            uint32_t height) 
    {
        std::string decoded;
        for (auto* node = dataNode->FirstChild(); node; node = node->NextSibling()) {
            if (auto* text = node->ToText()) {
                std::string raw = text->Value();
                //raw.erase(std::remove_if(raw.begin(), raw.end(), ::isspace), raw.end());
                raw.erase(std::remove_if(raw.begin(), raw.end(), [](unsigned char c) { return std::isspace(c); }), raw.end());
                decoded += base64_decode(raw);
            }
        }

        const size_t expectedSize = static_cast<size_t>(width) * height;
        std::vector<uint32_t> ids(expectedSize);
        uLongf destLen = static_cast<uLongf>(ids.size() * sizeof(uint32_t));

        if (uncompress(reinterpret_cast<Bytef*>(ids.data()), &destLen,
                    reinterpret_cast<const Bytef*>(decoded.data()),
                    static_cast<uLong>(decoded.size())) != Z_OK) 
        {
            std::cerr << "[ERROR] Zlib decompression failed for tile layer\n";
            return {};
        }
        return ids;
    }

} // anonymous namespace

// ============================================================================
// Handler 映射表 (Meyers Singleton + Static Function Pointers)
// ============================================================================

const std::unordered_map<std::string_view, MapParserSystem::ElementHandler>& 
MapParserSystem::getHandlers() 
{
    static const std::unordered_map<std::string_view, ElementHandler> handlers = {
        {TAG_PROPERTIES,  [](auto* elem, auto& reg) { handleProperties(elem, reg); }},
        {TAG_TILESET,     [](auto* elem, auto& reg) { handleTileset(elem, reg); }},
        {TAG_OBJECTGROUP, [](auto* elem, auto& reg) { handleObjectGroup(elem, reg); }},
        {TAG_LAYER,       [](auto* elem, auto& reg) { handleLayer(elem, reg); }}
    };
    return handlers;
}

// ============================================================================
// 主入口：解析关卡 -> 填充 ECS Registry
// ============================================================================

void MapParserSystem::parseLevel(entt::registry& registry, std::string_view levelFile) 
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(levelFile.data()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "[ERROR] Failed to load level '" << levelFile 
            << "': " << doc.ErrorStr() << "\n";
    }

    auto* root = doc.RootElement();
    if (!root) {
        std::cerr << "[ERROR] Level file '" << levelFile << "' has no root element\n";
    }

    // 1. 填充唯一基础组件 (替代原有的成员变量状态)
    TileLayerGrid levelComp;
    levelComp.grid_size      = static_cast<int>(getIntAttr(root, "tilewidth"));
    levelComp.width_num     = static_cast<int>(getIntAttr(root, "width"));
    levelComp.height_num    = static_cast<int>(getIntAttr(root, "height"));
    registry.ctx().emplace<TileLayerGrid>(levelComp);

    std::cout << "[INFO] Loading level: " << levelComp.width_num << "x" << levelComp.height_num 
        << " (tile: " << levelComp.grid_size << ")\n";

    // 2. 分发子元素到对应 Handler
    const auto& handlers = getHandlers();
    for (auto* child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const std::string_view tag(child->Value());
        //std::cout << tag << " [tag]\n";
        if (auto it = handlers.find(tag); it != handlers.end()) {
            it->second(child, registry);
        }
    }
}

// ============================================================================
// Element Handlers (Static, Stateless)
// ============================================================================

void MapParserSystem::handleProperties(tinyxml2::XMLElement* elem, entt::registry& reg) 
{
    for (auto* prop = elem->FirstChildElement(); prop; prop = prop->NextSiblingElement()) {
        if (isTag(prop, TAG_PROPERTY)) {
            parseTextures(prop, reg);
        }
    }
}

void MapParserSystem::handleTileset(tinyxml2::XMLElement* elem, entt::registry& reg) 
{
    parseTilesets(elem, reg);
}

void MapParserSystem::handleObjectGroup(tinyxml2::XMLElement* elem, entt::registry& reg) 
{
    parseObjectLayer(elem, reg);
}

void MapParserSystem::handleLayer(tinyxml2::XMLElement* elem, entt::registry& reg) 
{
    auto* firstChild = elem->FirstChildElement();
    //std::cout << firstChild << " [layer]\n";
    if (!firstChild) return;

    const std::string_view firstTag(firstChild->Value());
    //std::cout << firstTag << " [layer tag]\n";
    if (firstTag == TAG_OBJECT) {
        parseObjectLayer(elem, reg);
    } else if (firstTag == TAG_DATA || 
            (elem->FirstChildElement()->NextSiblingElement() != 0 && 
             elem->FirstChildElement()->NextSiblingElement()->Value() == std::string("data"))
            ) {
        std::cout << TAG_DATA << " [Tilelayer]\n";
        parseTileLayer(elem, reg);
    }
}

// ============================================================================
// 辅助解析方法
// ============================================================================

void MapParserSystem::parseTextures(tinyxml2::XMLElement* elem, entt::registry& /*reg*/) 
{
    const auto name  = getStrAttr(elem, "name");
    const auto value = getStrAttr(elem, "value");
    if (!name.empty() && !value.empty()) {
        TheTextureManager::Instance()->load(std::string(value), std::string(name));
        std::cout << "[DEBUG] Registered texture '" << name << "' -> '" << value << "'\n";
    }
}

void MapParserSystem::parseTilesets(tinyxml2::XMLElement* elem, entt::registry& reg) 
{
    auto* imageElem = elem->FirstChildElement("image");
    if (!imageElem) return;
    const auto source = getStrAttr(imageElem, "source");
    const auto name   = getStrAttr(elem, "name");
    // 构建资源路径时才发生堆分配
    std::string path = "assets/" + std::string(source);
    TheTextureManager::Instance()->load(path, std::string(name));
    // 每个 tileset 创建独立 entity
    const auto tilesetEntity = reg.create();
    auto& ts = reg.emplace<TilesetComponent>(tilesetEntity);
    ts.name         = std::string(name);
    ts.firstGid     = static_cast<uint32_t>(getIntAttr(elem, "firstgid"));
    ts.tileWidth    = static_cast<uint16_t>(getIntAttr(elem, "tilewidth"));
    ts.tileHeight   = static_cast<uint16_t>(getIntAttr(elem, "tileheight"));
    ts.spacing      = static_cast<uint16_t>(getIntAttr(elem, "spacing"));
    ts.margin       = static_cast<uint16_t>(getIntAttr(elem, "margin"));
    ts.tilecount    = static_cast<uint32_t>(getIntAttr(elem, "tilecount"));
    ts.imageWidth   = static_cast<uint32_t>(getIntAttr(imageElem, "width"));
    ts.imageHeight  = static_cast<uint32_t>(getIntAttr(imageElem, "height"));
    ts.numColumns   = ts.imageWidth  / (ts.tileWidth + ts.spacing);
    ts.numRows      = ts.imageHeight / (ts.tileHeight + ts.spacing);
    std::cout << "[DEBUG] Loaded tileset '" << name 
        << "' (firstGID=" << ts.firstGid << ")\n";
}

void MapParserSystem::parseObjectLayer(tinyxml2::XMLElement* elem, entt::registry& reg) 
{
    for (auto* obj = elem->FirstChildElement(); obj; obj = obj->NextSiblingElement()) {
        if (!isTag(obj, TAG_OBJECT)) continue;
        const int x = getIntAttr(obj, "x");
        const int y = getIntAttr(obj, "y");
        const auto type = getStrAttr(obj, "type");
        // 解析自定义属性
        uint8_t width = 0, height = 0, numFrames = 1, callbackID = 0, animSpeed = 4;
        std::string textureID;
        for (auto* props = obj->FirstChildElement(); props; props = props->NextSiblingElement()) {
            if (!isTag(props, TAG_PROPERTIES)) continue;
            for (auto* p = props->FirstChildElement(); p; p = p->NextSiblingElement()) {
                if (!isTag(p, TAG_PROPERTY)) continue;
                const auto propName  = getStrAttr(p, "name");
                const auto propValue = getStrAttr(p, "value");
                if (propName == "textureID")          textureID = std::string(propValue);
                else if (propName == "numFrames")     numFrames  = std::stoi(std::string(propValue));
                else if (propName == "textureWidth")  width      = std::stoi(std::string(propValue));
                else if (propName == "textureHeight") height     = std::stoi(std::string(propValue));
                else if (propName == "callbackID")    callbackID = std::stoi(std::string(propValue));
                else if (propName == "animSpeed")     animSpeed  = std::stoi(std::string(propValue));
            }
        }

        // === ECS 实体创建 ===
        const auto entityObj = reg.create();
        reg.emplace<Position>(entityObj, x / CELL_SIZE, y / CELL_SIZE);
        if (!textureID.empty()) {
            reg.emplace<SpriteComponent>(entityObj, textureID, width, height, numFrames, callbackID, animSpeed);
        }
        if (type == "Player") {
            reg.emplace<PlayerTag>(entityObj);
            reg.emplace<InputState>(entityObj);
            reg.emplace<Sources>(entityObj);
            std::cout << "[INFO] Created Player entity at (" << x << ", " << y << ")\n";
        }
    }
}

void MapParserSystem::parseTileLayer(tinyxml2::XMLElement* elem, entt::registry& reg) 
{
    tinyxml2::XMLElement* dataNode = nullptr;
    bool collidable = false;
    bool object = false;
    for (auto* child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const std::string_view tag(child->Value());
        if (tag == TAG_DATA) {
            dataNode = child;
        } else if (tag == TAG_PROPERTIES) {
            for (auto* p = child->FirstChildElement(); p; p = p->NextSiblingElement()) {
                if (isTag(p, TAG_PROPERTY) && getStrAttr(p, "name") == "collidable") {
                    collidable = true;
                }
                if (isTag(p, TAG_PROPERTY) && getStrAttr(p, "name") == "object") {
                    object = true;
                }
            }
        }
    }
    if (!dataNode) {
        std::cerr << "[WARN] Tile layer missing <data> node, skipping\n";
        return;
    }
    const auto levelEntity = reg.create();
    reg.emplace<TileMap>(levelEntity);
    // 处理所有 tilesets
    auto& levelComp = reg.get<TileMap>(levelEntity);
    auto& tlg = reg.ctx().get<TileLayerGrid>();
    auto tileIDs = decodeTileData(dataNode, tlg.width_num, tlg.height_num);
    if (tileIDs.empty()) return;
    levelComp.tileIDs = std::move(tileIDs);
    if (collidable) reg.emplace<CollidableLayer>(levelEntity);
    if (object) reg.emplace<ObjLayer>(levelEntity);
    std::cout << "[DEBUG] Parsed tile layer: " << std::boolalpha << collidable << "\n";
}

void MapParserSystem::precomputeTileUVs(entt::registry& reg)
{
    auto& tlg = reg.ctx().get<TileLayerGrid>();
    auto view_map = reg.view<TileMap>();
    std::cout << "MapParserSystem: TileMap size " << view_map.size() << " \n";
    auto view = reg.view<TilesetComponent>();
    for (auto [tilesetEntity, tc] : view.each()) {
        for (auto [mapEntity, ts] : view_map.each()) {
            auto& map = reg.get<TileMap>(mapEntity);
            std::unordered_map<uint32_t, TileRenderInfo> gidToInfo;
            // 计算当前 Tileset 的 GID 范围
            uint32_t gidMin = tc.firstGid;
            //uint32_t gidMax = ts.firstGid + ts.tilecount - 1;
            for (uint32_t localId = 0; localId < tc.tilecount; localId++) {
                TileRenderInfo info{};
                info.visible = true;
                info.srcX = tc.margin + (localId % tc.numColumns) * (tc.tileWidth + tc.spacing);
                info.srcY = tc.margin + (localId / tc.numColumns) * (tc.tileHeight + tc.spacing);
                const uint32_t globalId = gidMin + localId; // TMX 规范：GID 从 1 开始
                gidToInfo.emplace(globalId, info);
            }
            TilesetRange tsR;
            tsR.mapEntity = mapEntity;
            tsR.tileInfos.reserve(map.tileIDs.size()); // 预分配内存
            for (uint32_t i = 0; i < map.tileIDs.size(); ++i) {
                const uint32_t rawId = map.tileIDs[i];
                if (rawId == 0) { continue; }
                auto it = gidToInfo.find(rawId);
                if (it != gidToInfo.end()) {
                    TileRenderInfo info = it->second;
                    info.GridX = i % tlg.width_num;
                    info.GridY = i / tlg.width_num;
                    info.dstX = info.GridX * CELL_SIZE;
                    info.dstY = info.GridY * CELL_SIZE;
                    info.visible = true;
                    tsR.tileInfos.push_back(info);
                    //tsR.sparseIndices.push_back(i);

                    //std::cout << "parser: [" << i << "] ";
                    //std::cout << localId << "; (";
                    //std::cout << localId % ts.numColumns << ", ";
                    //std::cout << localId / ts.numColumns << ") -> (";
                    //std::cout << info.GridX << ", ";
                    //std::cout << info.GridY << ") ";
                    //std::cout << "visible: " << info.visible << " \n";
                    //std::cout << "parser: " << i << " \n";
                }
            }
            tc.tsRanges.push_back(tsR);
        }
    }
}

