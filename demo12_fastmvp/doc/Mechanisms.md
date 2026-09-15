# 木制物品耐久与疲劳系统设计方案

## 一、设计目标与核心原则

本方案旨在为 EnTT ECS 架构设计一套完整的木制物品耐久系统，模拟真实材料的疲劳损耗与不可逆损伤。核心设计理念借鉴电池充放电循环模型，但通过累计磨损量的连续模型实现更优雅的工程实践。

### 核心原则

1. **数据局部性优先**：组件按变化频率和读写系统分离
2. **连续优于离散**：用累计磨损量替代循环次数，消除边界情况
3. **阶段化体验**：通过品质阶段提供清晰的玩家反馈和决策点
4. **材质驱动**：所有数值由 MaterialData 驱动，实现极致复用

---

## 二、组件设计

### 2.1 组件划分与颗粒度

```cpp
// 1. 材质组件（静态，只读，所有木制品共享）
struct MaterialComponent {
    enum class Type { Wood, Stone, Iron } type;
    const MaterialData* data;  // 指向全局材质数据
};

// 材质静态数据（全局注册表）
struct MaterialData {
    float hardness;              // 硬度：影响攻击力和基础耐久
    float density;               // 密度：影响重量
    float flammability;          // 可燃性
    float sharpness;             // 锋利度保持性
    float total_wear_capacity;   // 终身抗磨损总量（核心新增）
    float repair_penalty;        // 修理惩罚系数（保留给特殊机制）
};

// 2. 当前耐久组件（高频变化，可逆）
struct DurabilityComponent {
    float current;  // 当前剩余耐久
};

// 3. 材料疲劳组件（单调递增，不可逆）
struct MaterialFatigueComponent {
    float total_wear_capacity;              // 终身磨损容量（制造时确定）
    float accumulated_wear;                 // 已发生的累计磨损
    MaterialCondition condition;            // 当前品质阶段
    
    // 辅助函数：获取当前最大耐久比例
    float get_durability_ratio() const {
        float wear_ratio = accumulated_wear / total_wear_capacity;
        
        if (wear_ratio < 0.15f) return 1.0f;   // 崭新
        if (wear_ratio < 0.40f) return 0.8f;   // 磨损
        if (wear_ratio < 0.65f) return 0.55f;  // 损坏
        if (wear_ratio < 0.85f) return 0.35f;  // 破败
        return 0.25f;                           // 报废边缘
    }
};

// 4. 品相枚举
enum class MaterialCondition {
    Pristine,    // 崭新：100% 耐久
    Worn,        // 磨损：80% 耐久
    Damaged,     // 损坏：55% 耐久
    Ruined,      // 破败：35% 耐久
    BeyondRepair // 报废：25% 耐久（不可修复）
};

// 5. 功能组件
struct ToolComponent {
    enum class ToolType { Axe, Shovel, Sword, Bowl } type;
    float efficiency_multiplier = 1.0f;  // 受品相影响
};

// 6. 状态标签
struct NeedsRepairTag {};
struct NearingBreakageTag {};
struct BrokenBeyondRepairTag {};

// 7. 事件
struct RepairEvent { 
    float quality_factor = 1.0f;  // 修理质量系数
};
```

### 2.2 组件组合示例

**树木（Tree）**：
```cpp
entity
  ├── Position
  ├── Mesh
  ├── MaterialComponent(Wood)
  ├── Health
  └── Growable
```

**木剑（Wooden Sword）**：
```cpp
entity
  ├── Position
  ├── Mesh
  ├── MaterialComponent(Wood)
  ├── DurabilityComponent
  ├── MaterialFatigueComponent
  ├── ToolComponent(Sword)
  └── DamageComponent
```

---

## 三、系统实现

### 3.1 制造系统

负责初始化物品的疲劳属性和初始耐久。

```cpp
void System_CraftWoodenItem(registry& reg) {
    auto view = reg.view<CraftedEvent, MaterialComponent>();
    
    for (auto entity : view) {
        auto& mat = view.get<MaterialComponent>(entity);
        
        // 计算终身磨损容量
        float base_capacity = mat.data->total_wear_capacity;
        
        // 工具类型影响容量
        if (reg.all_of<ToolComponent>(entity)) {
            auto& tool = reg.get<ToolComponent>(entity);
            float tool_factor = get_tool_wear_factor(tool.type);
            base_capacity *= tool_factor;
        }
        
        // 初始化疲劳组件
        reg.emplace<MaterialFatigueComponent>(entity, 
            base_capacity,                    // total_wear_capacity
            0.0f,                             // accumulated_wear
            MaterialCondition::Pristine       // condition
        );
        
        // 设置初始耐久
        float initial_max = calculate_max_durability(reg, entity);
        reg.emplace<DurabilityComponent>(entity, initial_max);
    }
}

float get_tool_wear_factor(ToolComponent::ToolType type) {
    switch(type) {
        case ToolComponent::ToolType::Sword: return 1.0f;  // 高磨损
        case ToolComponent::ToolType::Axe:   return 0.8f;
        case ToolComponent::ToolType::Shovel:return 0.7f;
        case ToolComponent::ToolType::Bowl:  return 0.3f;  // 低磨损
    }
}
```

### 3.2 耐久计算系统

动态计算当前最大耐久度，基于累计磨损量和品相阶段。

```cpp
float calculate_max_durability(registry& reg, entt::entity entity) {
    auto& fatigue = reg.get<MaterialFatigueComponent>(entity);
    auto& mat = reg.get<MaterialComponent>(entity);
    
    float wear_ratio = fatigue.accumulated_wear / fatigue.total_wear_capacity;
    
    // 更新品相阶段
    fatigue.condition = evaluate_condition(wear_ratio);
    
    // 获取品相对应的耐久比例
    float ratio = fatigue.get_durability_ratio();
    
    // 理论最大耐久基于材质硬度
    float theoretical_max = mat.data->hardness * 2.0f;  // 示例公式
    
    return theoretical_max * ratio;
}

MaterialCondition evaluate_condition(float wear_ratio) {
    if (wear_ratio < 0.15f) return MaterialCondition::Pristine;
    if (wear_ratio < 0.40f) return MaterialCondition::Worn;
    if (wear_ratio < 0.65f) return MaterialCondition::Damaged;
    if (wear_ratio < 0.85f) return MaterialCondition::Ruined;
    return MaterialCondition::BeyondRepair;
}
```

### 3.3 使用与磨损系统

处理物品使用时的耐久消耗和疲劳累积。

```cpp
void System_UseWoodenItem(registry& reg) {
    auto view = reg.view<DurabilityComponent, MaterialFatigueComponent>();
    
    for (auto entity : view) {
        // 检查是否已报废
        if (reg.all_of<BrokenBeyondRepairTag>(entity)) {
            continue;
        }
        
        // 触发使用逻辑（假设通过事件或系统调用）
        float wear_amount = calculate_wear_amount(reg, entity);
        
        auto& fatigue = view.get<MaterialFatigueComponent>(entity);
        auto& dur = view.get<DurabilityComponent>(entity);
        
        // 应用材质磨损系数
        float material_factor = reg.get<MaterialComponent>(entity).data->hardness;
        float actual_wear = wear_amount / material_factor;
        
        // 特效影响
        if (reg.all_of<OilCoatingTag>(entity)) {
            actual_wear *= 0.5f;  // 桐油涂层减半磨损
        }
        
        // 累积终身磨损（不可逆）
        fatigue.accumulated_wear += actual_wear;
        
        // 扣除当前耐久
        dur.current -= actual_wear;
        
        // 检查损坏状态
        if (dur.current <= 0.0f) {
            dur.current = 0.0f;
            reg.emplace_or_replace<NeedsRepairTag>(entity);
        }
        
        // 检查是否接近报废
        float wear_ratio = fatigue.accumulated_wear / fatigue.total_wear_capacity;
        if (wear_ratio > 0.85f) {
            reg.emplace_or_replace<NearingBreakageTag>(entity);
        }
        
        // 检查是否完全报废
        if (wear_ratio >= 1.0f) {
            reg.emplace_or_replace<BrokenBeyondRepairTag>(entity);
        }
    }
}

float calculate_wear_amount(registry& reg, entt::entity entity) {
    // 基础磨损量
    float base_wear = 5.0f;
    
    // 工具类型影响
    if (reg.all_of<ToolComponent>(entity)) {
        auto& tool = reg.get<ToolComponent>(entity);
        // 效率越高，磨损越快
        base_wear *= tool.efficiency_multiplier;
    }
    
    return base_wear;
}
```

### 3.4 修理系统

处理修理逻辑，恢复当前耐久但不重置累计磨损。

```cpp
void System_RepairItem(registry& reg) {
    auto view = reg.view<RepairEvent, DurabilityComponent, MaterialFatigueComponent>();
    
    for (auto entity : view) {
        // 检查是否可修复
        if (reg.all_of<BrokenBeyondRepairTag>(entity)) {
            // 已报废，无法修复
            continue;
        }
        
        auto& dur = view.get<DurabilityComponent>(entity);
        auto& fatigue = view.get<MaterialFatigueComponent>(entity);
        
        // 获取修理质量系数
        float quality = view.get<RepairEvent>(entity).quality_factor;
        
        // 计算修复后的最大耐久
        float max_dur = calculate_max_durability(reg, entity);
        
        // 应用修理质量
        max_dur *= quality;
        
        // 恢复当前耐久
        dur.current = max_dur;
        
        // 清除损坏标记
        reg.remove<NeedsRepairTag>(entity);
    }
    
    reg.clear<RepairEvent>();
}
```

### 3.5 品相更新系统

根据品相调整工具性能和其他属性。

```cpp
void System_UpdateToolPerformance(registry& reg) {
    auto view = reg.view<MaterialFatigueComponent, ToolComponent>();
    
    for (auto entity : view) {
        auto& fatigue = view.get<MaterialFatigueComponent>(entity);
        auto& tool = view.get<ToolComponent>(entity);
        
        // 根据品相调整效率
        switch (fatigue.condition) {
            case MaterialCondition::Pristine:
                tool.efficiency_multiplier = 1.0f;
                break;
            case MaterialCondition::Worn:
                tool.efficiency_multiplier = 0.9f;
                break;
            case MaterialCondition::Damaged:
                tool.efficiency_multiplier = 0.7f;
                break;
            case MaterialCondition::Ruined:
                tool.efficiency_multiplier = 0.4f;
                break;
            case MaterialCondition::BeyondRepair:
                tool.efficiency_multiplier = 0.2f;
                break;
        }
    }
}
```

---

## 四、材质数据驱动

### 4.1 不同木材的配置示例

```cpp
// 全局材质注册表
std::unordered_map<MaterialType, MaterialData> material_registry = {
    { MaterialType::Oak, MaterialData{
        .hardness = 50.0f,
        .density = 0.75f,
        .flammability = 1.0f,
        .sharpness = 0.5f,
        .total_wear_capacity = 300.0f,   // 普通橡木，寿命短
        .repair_penalty = 0.15f
    }},
    
    { MaterialType::Ironwood, MaterialData{
        .hardness = 120.0f,
        .density = 1.2f,
        .flammability = 0.5f,
        .sharpness = 0.9f,
        .total_wear_capacity = 2000.0f,  // 百年铁木，可传三代
        .repair_penalty = 0.05f
    }},
    
    { MaterialType::SpiritWood, MaterialData{
        .hardness = 80.0f,
        .density = 0.5f,
        .flammability = 0.0f,            // 不燃
        .sharpness = 1.5f,
        .total_wear_capacity = 5000.0f,  // 神木，几乎不朽
        .repair_penalty = 0.02f
    }}
};
```

### 4.2 材质复用机制

所有木制品共享同一份 MaterialData，通过指针引用：

```cpp
// 创建木剑时
MaterialComponent mat_comp;
mat_comp.type = MaterialType::Ironwood;
mat_comp.data = &material_registry[MaterialType::Ironwood];

// 创建木碗时，复用同一份数据
MaterialComponent bowl_mat;
bowl_mat.type = MaterialType::Ironwood;
bowl_mat.data = &material_registry[MaterialType::Ironwood];  // 同一指针
```

---

## 五、关键机制详解

### 5.1 部分磨损结算（未完全耗尽就修复）

**核心思想**：累计磨损量与修理行为完全解耦。

- 使用系统实时累积 `accumulated_wear`，与当前耐久无关
- 修理系统只恢复 `current`，不重置 `accumulated_wear`
- 无论何时修理，累计磨损都在持续增长

**示例**：
```
初始状态：total_capacity=500, accumulated=0, current=100
使用20点耐久：accumulated=20, current=80
修理：current恢复但accumulated不变=20
再次使用30点：accumulated=50, current=70
```

### 5.2 品相阶段与耐久下限

**问题**：线性衰减会导致后期"砍一刀修一次"的糟糕体验。

**解决方案**：阶段化耐久比例 + 软下限。

```
磨损比例范围    品相      耐久比例
0% - 15%      崭新      100%
15% - 40%     磨损      80%
40% - 65%     损坏      55%
65% - 85%     破败      35%
85% - 100%    报废      25%（下限）
```

每个阶段内耐久稳定，阶段跃迁时玩家获得清晰的"变差了"信号。

### 5.3 报废机制

当 `wear_ratio >= 1.0f` 时，物品完全报废：

```cpp
void System_HandleBrokenItems(registry& reg) {
    auto view = reg.view<BrokenBeyondRepairTag>();
    
    for (auto entity : view) {
        // 选项1：自动分解为基础材料
        if (reg.all_of<MaterialComponent>(entity)) {
            auto& mat = reg.get<MaterialComponent>(entity);
            // 返还部分材料
            spawn_material_items(mat.type, 0.3f);  // 回收30%
        }
        
        // 选项2：标记为垃圾
        reg.emplace<JunkTag>(entity);
        
        // 选项3：触发事件供UI显示
        reg.emplace<ItemDestroyedEvent>(entity);
    }
}
```

---

## 六、系统交互流程

### 6.1 完整生命周期

```
制造 → 崭新阶段 → 使用中累积磨损 → 磨损阶段 → 
继续使用 → 损坏阶段 → 修理维持 → 破败阶段 → 
决策：继续凑合/更换新装备 → 报废或分解
```

### 6.2 系统执行顺序

```cpp
void game_update(registry& reg) {
    System_CraftWoodenItem(reg);      // 处理制造事件
    System_UseWoodenItem(reg);        // 处理使用逻辑
    System_UpdateToolPerformance(reg); // 更新品相影响
    System_RepairItem(reg);           // 处理修理事件
    System_HandleBrokenItems(reg);    // 处理报废
}
```

---

## 七、扩展与优化建议

### 7.1 修理技能系统

```cpp
struct RepairSkillComponent {
    float skill_level;  // 0.0 - 1.0
};

// 在修理系统中
if (reg.all_of<RepairSkillComponent>(entity)) {
    float skill = reg.get<RepairSkillComponent>(entity).skill_level;
    quality_factor = 0.8f + skill * 0.4f;  // 技能影响修理质量
}
```

### 7.2 附魔与特效

```cpp
// 通过 Tag 或组件检测特效
if (reg.all_of<UnbreakableEnchantment>(entity)) {
    // 完全不累积磨损
    continue;
}
```

### 7.3 环境因素

```cpp
// 下雨天木材受潮
void System_RainEffect(registry& reg) {
    auto view = reg.view<MaterialComponent, MaterialFatigueComponent>();
    
    for (auto entity : view) {
        auto& mat = view.get<MaterialComponent>(entity);
        if (mat.type == MaterialComponent::Type::Wood && is_raining) {
            // 受潮加速磨损
            fatigue.accumulated_wear += 0.1f * delta_time;
        }
    }
}
```

---

## 八、总结

本方案通过以下核心设计实现了优雅的木制物品耐久系统：

1. **累计磨损模型**：用连续量替代离散循环次数，消除边界问题
2. **品相阶段系统**：提供清晰的玩家反馈和决策点
3. **材质数据驱动**：实现极致的组件复用和数值调优便利性
4. **软下限设计**：避免后期频繁修复的糟糕体验
5. **报废机制**：给物品生命周期一个明确的终点

整个系统在 EnTT ECS 架构下运行高效，组件颗粒度适中，既避免了过度设计，又保持了良好的扩展性。
