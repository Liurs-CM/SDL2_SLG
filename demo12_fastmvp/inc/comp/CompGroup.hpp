#pragma once
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
    // 👇 新增：允许从基类值构造 Tagged
    explicit constexpr Tagged(const T& base) : T{base} {}
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

struct _UIPosition {};
using uiPosition = Tagged<vec, _UIPosition>;

// 移动组件
struct _InputState {};
using InputState  = Tagged<vec, _InputState>;

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

/// @brief 玩家实体标记组件, 存储player entity
struct PlayerRef { 
    entt::entity id; 
    constexpr operator entt::entity() const noexcept { return id; }
};
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

// 仅作为"本帧需要移动"的标记，无需任何数据
struct WantsToMove {};

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
/// @brief 其它容器物品
struct ContainedRef { 
    entt::entity id; 
    constexpr operator entt::entity() const noexcept { return id; }
};

/// @brief 玩家装备
struct EquippedBy { 
    entt::entity wearer;
    //SlotType slot; 
};

/// @brief 对象ID枚举
enum objID {
    Chest = 199,
    ChestOpened = 200,
    Coin = 201,
    Bag = 203,
    Key = 205,
    Cheese = 208,
    DoorClosed = 209,
    DoorOpend = 210,
    TreeApple = 217,
    Cat = 235,
    Goblin = 236,
    Rabbit = 237,
    Sword = 238,
    Sheep  = 239,
    Cow  = 240,
    Wolf  = 241,
    Human  = 242,
};

enum class GoblinState : uint8_t {
    Init,
    WantFood,
};
//Eating,     // 新增：吃奶酪的过渡状态
//Done        // 替代 ALIVE_DEAD，延迟销毁

struct GoblinAI {
    GoblinState state{GoblinState::Init};
};

/// @brief UI ID枚举
enum uiID {
    Box = 397,
    CornerLU = 398,
    CornerU = 399,
    CornerL = 416,
    BoxCur = 415,
};

// 物品栏组件（用于玩家）
struct Inventory {
    static constexpr int MAX_SLOTS = 4;
    std::array<entt::entity, MAX_SLOTS> slots;
    std::array<entt::entity, MAX_SLOTS> slotBoxs;
    int gold{0};
    size_t selectedSlot{0};
    Inventory() { slots.fill(entt::null); }
    [[nodiscard]] bool isFull() const noexcept {
        return std::all_of(slots.begin(), slots.end(), 
                [](entt::entity e) { return e != entt::null; });
    }
    [[nodiscard]] int getFreeSlot() const noexcept {
        for (int i = 0; i < MAX_SLOTS; ++i) {
            if (slots[i] == entt::null) return i;
        }
        return 255;
    }
    [[nodiscard]] bool hasItem(entt::entity item) const noexcept {
        return std::find(slots.begin(), slots.end(), item) != slots.end();
    }
    [[nodiscard]] bool hasItem(entt::registry& reg, objID id) const noexcept {
        return std::any_of(slots.begin(), slots.end(), [&reg, id](entt::entity ent) {
                // try_get 自动跳过空槽位(entt::null)和缺失组件的实体
                const auto* obj = reg.try_get<ObjectComponent>(ent);
                return obj && obj->firstTileID == id;
                });
    }
    [[nodiscard]] entt::entity getItem(entt::registry& reg, objID id) const noexcept {
        auto it = std::find_if(slots.begin(), slots.end(), [&reg, id](entt::entity ent) {
                // try_get 自动跳过空槽位(entt::null)和缺失组件的实体
                const auto* obj = reg.try_get<ObjectComponent>(ent);
                return obj && obj->firstTileID == id;
                });
        return (it != slots.end()) ? *it : entt::null;
    }
    [[nodiscard]] size_t findSlotIndex(entt::registry& reg, objID id) const noexcept {
        for (size_t i = 0; i < slots.size(); ++i) {
            if (const auto* obj = reg.try_get<ObjectComponent>(slots[i]); obj && obj->firstTileID == id) {
                return i; // 直接返回索引
            }
        }
        return static_cast<size_t>(-1); // 或 std::nullopt
    }
    [[nodiscard]] entt::entity getCurrentSlotBox() const noexcept {
        //if (slotBoxs[selectedSlot] == entt::null) return entt::null;
        return slotBoxs[selectedSlot];
    }
    void cycleSlot(int direction) noexcept {
        constexpr int N = static_cast<int>(MAX_SLOTS);
        selectedSlot = static_cast<size_t>( ((static_cast<int>(selectedSlot) + direction) % N + N) % N);
    }
};

template <typename Tag, typename T = int>
struct Stat {
    static_assert(std::is_arithmetic_v<T>, "Stat<T> requires arithmetic type");
    T max{};   // 上限值
    T cur{};   // 当前值
               // ── 构造 ──
    constexpr Stat() noexcept = default;
    constexpr Stat(T maximum, T current = T{0}) noexcept
        : max{normalize_max(maximum)}, cur{clamp_to_range(current, normalize_max(maximum))} {}
    // ── 比率查询 ──
    [[nodiscard]] constexpr float ratio() const noexcept {
        return (max != T{0}) ? static_cast<float>(cur) / static_cast<float>(max) : 0.f;
    }
    [[nodiscard]] constexpr bool is_full()  const noexcept { return cur >= max && max > T{0}; }
    [[nodiscard]] constexpr bool is_empty() const noexcept { return cur <= T{0}; }
    [[nodiscard]] constexpr bool is_alive() const noexcept { return cur > T{0}; }
    // ── 修改（安全钳制） ──
    constexpr void set(T value) noexcept { cur = clamp_to_range(value, max); }
    constexpr void modify(T delta) noexcept { 
        if constexpr (std::is_floating_point_v<T>) {
            set(cur + delta);
        } else {
            // 整数版：避免直接 cur + delta 溢出
            if (delta >= T{0}) {
                const T room = max - cur;
                cur = (delta > room) ? max : static_cast<T>(cur + delta);
            } else {
                const T down = cur - T{0};
                const T step = static_cast<T>(-delta);
                cur = (step > down) ? T{0} : static_cast<T>(cur - step);
            }
        }
    }
    constexpr void set_max(T new_max, bool fill = false) noexcept {
        max = normalize_max(new_max); // 防止 max=0
        cur = fill ? max : std::min(cur, max);
    }
    [[nodiscard]] constexpr T remaining() const noexcept { return max - cur; }
    // ── 比较 ──
    [[nodiscard]] constexpr bool operator==(const Stat&) const noexcept = default;
private:
    static constexpr T normalize_max(T m) noexcept {
        return (m > T{0}) ? m : T{0};
    }
    static constexpr T clamp_to_range(T value, T upper) noexcept {
        if (value < T{0}) return T{0};
        if (value > upper) return upper;
        return value;
    }
};

//struct HP {
//    uint8_t max;
//    uint8_t cur;
//};
//struct Age_ {}; // 生长速度 上限 掉落价值 恢复能力
//struct Age : Stat<Age_, uint8_t> {using Stat::Stat; };
//struct HP_ {};
//struct HP : Stat<HP_, uint8_t> {using Stat::Stat; };
//enum Mass {
//    Dead,
//    ExThin,
//    Thin,
//    Normal,
//    Fat,
//    Strong,
//};
//struct MassBuf_ {};
//struct MassBuf : Stat<MassBuf_, uint8_t> {using Stat::Stat; };
struct Mass_ {};
struct Mass : Stat<Mass_, uint8_t> {using Stat::Stat; };
struct Satiety_ {};
struct Satiety : Stat<Satiety_, uint8_t> {using Stat::Stat; };
struct AP_ {};
struct AP : Stat<AP_, uint8_t> {using Stat::Stat; };

