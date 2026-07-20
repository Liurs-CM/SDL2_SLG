#pragma once
#include "comp/CompGroup.hpp"
#include <entt/entt.hpp>

class DestroySystem
{
    public:
        // 3. 在帧末尾或物理阶段后：统一批量销毁
        void flushDestroyedEntities(entt::registry& registry) {
            // 使用 each 遍历所有带标签的实体
            // 注意：这里可以安全销毁，因为我们不再依赖其他 View 的迭代
            auto view = registry.view<PendingDestroy>();

            // 【关键】先收集 entity ID，再统一销毁
            // EnTT 3.x+ 推荐方式，避免迭代器失效
            std::vector<entt::entity> toDestroy;
            toDestroy.reserve(view.size()); // 预分配，减少重分配

            for (auto entity : view) {
                toDestroy.push_back(entity);
            }

            // 批量销毁比逐个销毁快得多（内部优化了稀疏集压缩）
            registry.destroy(toDestroy.begin(), toDestroy.end());
        }
};
