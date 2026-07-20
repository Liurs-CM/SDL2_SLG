#pragma once
#include "core/GameConfig.h"
#include "util/Vector2D.h"
#include <entt/entt.hpp>
#include <SDL.h>
#include <string>
#include <vector>
#include <cstdint>

template<typename T, typename Tag>
struct Tagged : T {
    using Base = T;
    using T::T;
    explicit constexpr operator T() const { return static_cast<const T&>(*this); }
};
//隐蔽的类型泄漏问题:
// 基类的 operator+ 返回的是 Vec2，不是 Position！
// auto result = a + b;
// result 的类型是 vec，不是 Position ❌
// 这意味着链式运算会丢失类型标签
//#define DEFINED_TAGGED_TYPE(Name, BaseType) \
//    struct _Name {}; \
//    using Name = Tagged<BaseType, _Name>
//// 位置组件
//DEFINED_TAGGED_TYPE(Position, vec);
struct _Position {};
using Position = Tagged<vec, _Position>;

/// @brief 精灵渲染数据
/// @note 仅存储渲染所需参数，纹理资源由 TextureManager 统一管理
struct SpriteComponent {
    std::string textureID;     // 纹理资源键
    uint8_t width      = 0;        // 渲染区域宽
    uint8_t height     = 0;        // 渲染区域高
    uint8_t numFrames  = 1;        // 动画帧数
    uint8_t callbackID = 0;        // 回调ID
    uint8_t animSpeed  = 4;        // 动画播放速度 (帧/秒)
    uint8_t curFrame   = 1;        // 当前帧
};

struct ObjectComponent {
    std::string tilename;     // 纹理资源键
    uint32_t firstTileID    = 0;        // tileID
    uint8_t numFrames       = 1;        // 动画帧数
    uint8_t curFrame        = 1;        // 当前帧
    uint8_t animSpeed       = 4;        // 动画播放速度 (帧/秒)
    uint8_t callbackID      = 0;        // 回调ID
    //uint8_t width           = 0;        // 渲染区域宽
    //uint8_t height          = 0;        // 渲染区域高
};


/// @brief 玩家实体标记组件
/// @note 空结构体，零内存占用，仅用于 EnTT 视图过滤
struct PlayerTag {};
/// @note 定义一个标签组件，零内存占用，用于标记待销毁实体
struct PendingDestroy {};
//double moveDelay;

/// @brief 预计算的瓦片渲染信息（解析阶段生成，渲染阶段零分配）
struct TileRenderInfo {
    int  srcX = 0;
    int  srcY = 0;     // 纹理源矩形左上角（像素）
    int GridX = 0; // 纹理矩形坐标
    int GridY = 0;
    int  dstX = 0;
    int  dstY = 0;     // 纹理目标矩形左上角（像素）
    bool visible = false; // id==0 时标记为 false，避免运行时分支
};

struct TilesetRange {
    entt::entity mapEntity;
    std::vector<TileRenderInfo> tileInfos;
    //int gidMin, gidMax;
    //std::vector<int> sparseIndices;  // 记录该 Tileset 出现在哪些 gids 索引上
};

// 精灵表信息
//struct SpriteSheet {
//    uint8_t margin = 0;
//    uint8_t spacing = 0;
//    uint8_t currentRow = 0;
//    uint8_t currentFrame = 0;
//};

/// @brief 瓦片图集配置
/// @note 解析时从 XML 提取，运行时用于 GID -> UV 坐标映射
struct TilesetComponent {
    std::string name;          // 图集标识符
    uint32_t firstGid    = 0;  // 起始全局 ID
    uint16_t tileWidth   = 0;  // 单瓦片宽
    uint16_t tileHeight  = 0;  // 单瓦片高
    uint16_t spacing     = 0;  // 瓦片间距
    uint16_t margin      = 0;  // 边缘留白
    uint32_t tilecount   = 0;  // 瓦片数量
    uint32_t imageWidth  = 0;  // 图集纹理总宽
    uint32_t imageHeight = 0;  // 图集纹理总高
    uint16_t numColumns  = 0;  // 列数 = imageWidth  / (tileWidth + spacing)
    uint16_t numRows     = 0;  // 行数 = imageHeight / (tileHeight + spacing)
    std::vector<TilesetRange> tsRanges;
};

/// @brief 瓦片图层渲染数据（核心组件）
struct TileMap {
    // 访问方式: tiles[y * width_num + x]
    std::vector<uint32_t> tileIDs;
};

/// @brief 标记该层参与物理碰撞检测
struct CollidableLayer {};
/// @brief 标记该层为对象层
struct ObjLayer {};

/// @brief 瓦片地图渲染数据（核心组件）
struct TileLayerGrid {
    int width_num{0};
    int height_num{0};
    int grid_size{0};
};

/// @brief 关卡地图数据（数据合集）
struct LevelMap {
    const TileLayerGrid tlg;
    const std::vector<TilesetComponent()> tcs;
    const std::vector<TileMap> tms;
    std::vector<entt::entity> objs;
};

/// @brief 元数据：保留原始层级名称，便于调试或运行时查找
struct LayerName {
    std::string value;
};

// 移动组件
struct InputState {
    int dx;
    int dy;
};

// 输入控制组件, 用于标记可以接受输入的实体
//struct InputControllable { };

/// @brief 玩家资源统计
struct Sources {
    int gold = 10;
};

/// @brief 玩家背包物品
struct ContainedIn { 
    entt::entity container;
    uint8_t slotIndex; 
};

/// @brief 玩家装备
struct EquippedBy { 
    entt::entity wearer;
    //SlotType slot; 
};

/// @brief 对象ID枚举
enum objID {
    Chest = 198,
    Coin = 200,
    Cat = 234,
};
