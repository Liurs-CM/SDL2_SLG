// GameObjectFactory.hpp
#pragma once
#include "Components.hpp" // 引入组件定义
#include <entt/entt.hpp>
#include <string>
#include <map>
#include <functional>
#include <iostream>

// 定义构建函数的类型别名
// 该函数接收 registry 和 params，返回创建好的 entity
using EntityBuilder = std::function<entt::entity(entt::registry&, const std::string&)>;

class GameObjectFactory
{
    public:
        // 单例模式保持不变
        static GameObjectFactory* Instance()
        {
            static GameObjectFactory instance; // 使用静态局部变量实现单例 (Meyers Singleton)
            return &instance;
        }

        // 注册类型：现在注册的是一个 Lambda 或 std::function
        // typeID: "player", "enemy" 等
        // builder: 具体的构建逻辑
        void registerType(const std::string& typeID, EntityBuilder builder)
        {
            m_creators[typeID] = builder;
            std::cout << "Registered type: " << typeID << std::endl;
        }

        // 创建实体：替代原来的 createGameObject
        // 返回 entt::entity 而不是 GameObject*
        entt::entity create(entt::registry& registry, const std::string& typeID)
        {
            auto it = m_creators.find(typeID);
            if (it == m_creators.end())
            {
                std::cout << "Error: Could not find type: " << typeID << std::endl;
                return entt::null; // 返回空实体
            }

            // 调用对应的构建函数
            EntityBuilder& builder = it->second;
            return builder(registry, typeID);
        }

    private:
        GameObjectFactory() = default; // 私有构造函数
        std::map<std::string, EntityBuilder> m_creators;
};

// 保持原有的宏定义风格（可选）
typedef GameObjectFactory TheGameObjectFactory;
