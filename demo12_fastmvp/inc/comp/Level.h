#pragma once
#include <entt/entt.hpp>

class Level {
    public:
        entt::registry& getRegistry() { return m_registry; }
        void setPlayerEntity(entt::entity e) { m_playerEntity = e; }
        // ... 原有接口保持不变 ...
    private:
        entt::registry m_registry;  // 可选：Level 拥有 registry
        entt::entity m_playerEntity = entt::null;
};
